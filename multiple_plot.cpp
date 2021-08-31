#include "multiple_plot.h"
#include "qcustomplot.h"
#include <iostream>


MultiplePlot::MultiplePlot(const QVector<double> &timeVec,
                           const QVector<QVector<double>> &dataVec,
                           const QStringList &listColumnNames,
                           QWidget *parent) :
    QWidget{parent},
    timeVec_{timeVec},
    dataVec_{dataVec},
    listColumnNames_{listColumnNames}
{
    pMainLayout_ = new QHBoxLayout();

    //настраиваю шрифт и отображаю легенду
    QFont legendFont{font()};
    legendFont.setPointSize(10);

    pPlot_ = new QCustomPlot(this);
    pPlot_->legend->setFont(legendFont);
    pPlot_->legend->setVisible(true);
    // разрешения разлчиных взаимодействий с графиком
    pPlot_->setInteractions(QCP::Interaction::iRangeDrag
                            | QCP::Interaction::iRangeZoom
                            | QCP::Interaction::iSelectPlottables);
    pPlot_->axisRect()->setRangeDrag(Qt::Horizontal); //разрешаю двигать только ось X
    pPlot_->axisRect()->setRangeZoom(Qt::Horizontal);
    //просто передаю сигнал от оси Х вверх родителю
    connect(this->pPlot_->xAxis,
                     QOverload<const QCPRange &>::of(&QCPAxis::rangeChanged), //! потому что не стоит перегружать сигналы https://stackoverflow.com/questions/16794695/connecting-overloaded-signals-and-slots-in-qt-5
                     this, &MultiplePlot::axisXRangeChanged);
    pMainLayout_->addWidget(pPlot_);

    //виджет выбора графиков с кнопкой
    createRightWidget();

    pMainLayout_->setSpacing(1);
    pMainLayout_->setMargin(5);
    this->setLayout(pMainLayout_);
}

void MultiplePlot::createRightWidget()
{
    QWidget *rightWidget{new QWidget(this)};
    rightWidget->setMaximumWidth(150);
    QVBoxLayout *pSubLayout{new QVBoxLayout(rightWidget)};
    QStringListModel *model{new QStringListModel(listColumnNames_, this)};
    poListView_ = new QListView(this);
    poListView_->setModel(model);
    poListView_->setSelectionMode(QAbstractItemView::MultiSelection);
    pSubLayout->addWidget(poListView_);
    QPushButton *pButtonPlot{new QPushButton("Plot", this)};
    connect(pButtonPlot, &QPushButton::clicked, this, &MultiplePlot::plotSelected);
    pSubLayout->addWidget(pButtonPlot);
    pSubLayout->setMargin(0);
    pMainLayout_->addWidget(rightWidget);
}

void MultiplePlot::updateRangeAxisX(const QCPRange &newRange)
{
    //добавить в код QCPAxis::setRange проверку на максимум и минимум 0, dataVec_[0].size()
    //if (pPlot_->xAxis->range() != newRange) {
    pPlot_->xAxis->setRange(newRange); //проверку от цикличного изменения осуществляет библиотека
    //}
    pPlot_->replot();
}

void MultiplePlot::drawCursor(int index) {
    selectedIndex_ = index;
    if(pPlot_->graphCount()) {
        if (!plotCursor_) {
            plotCursor_ = new QCPItemStraightLine(pPlot_);
        }
        double tmpCordX{pPlot_->graph()->data()->at(index)->key};
        double tmpCordY_1{pPlot_->yAxis->range().lower};
        double tmpCordY_2{pPlot_->yAxis->range().upper};
        plotCursor_->point1->setCoords(tmpCordX, tmpCordY_1);
        plotCursor_->point2->setCoords(tmpCordX, tmpCordY_2);
        std::cout << "in draw cursor" << std::endl;
        pPlot_->replot();
    }
}

//------------------------------------SLOTS------------------------------------

void MultiplePlot::plotSelected()
{
    //получаю выбранные индексы
    QList<QModelIndex> selectedIndexList{
            poListView_->selectionModel()->selectedIndexes()};
    int nPlot{0};
    double minValue{0};
    double maxValue{0};
    //чищу график
    pPlot_->clearGraphs();
    QColor kColors[] {
        Qt::red,        Qt::green,  Qt::blue,
        Qt::darkCyan,   Qt::yellow, Qt::magenta
    };

    for(QModelIndex iModelindex : selectedIndexList) {
        pPlot_->addGraph();
        int index{iModelindex.row()};
        //устанавливаю имя для легенды
        pPlot_->graph(nPlot)->setName(iModelindex.data().toString());
        //устанавливаю цвет
        QPen tmpPen{};
        if(nPlot < sizeof(kColors)) {
            tmpPen.setColor(kColors[nPlot]);
        } else {
            tmpPen.setColor(Qt::black);
        }
        tmpPen.setWidth(2);
        pPlot_->graph(nPlot)->setPen(tmpPen);
        tmpPen.setWidth(2);

        pPlot_->graph(nPlot)->setSelectable(QCP::SelectionType::stSingleData);
        connect(this->pPlot_->graph(nPlot),
                QOverload<const QCPDataSelection &>::of(
                        &QCPAbstractPlottable::selectionChanged), //! потому что не стоит перегружать сигналы https://stackoverflow.com/questions/16794695/connecting-overloaded-signals-and-slots-in-qt-5
                this, &MultiplePlot::dataSelected);

        pPlot_->graph(nPlot)->setData(timeVec_, dataVec_[index]);
        double tmpMinValue{*std::min_element(dataVec_[index].begin(),
                                             dataVec_[index].end())};
        double tmpMaxValue{*std::max_element(dataVec_[index].begin(),
                                             dataVec_[index].end())};
        if (tmpMinValue < minValue)
            minValue = tmpMinValue;
        if (tmpMaxValue > maxValue)
            maxValue = tmpMaxValue;
        ++nPlot;
    }
    pPlot_->xAxis->setRange(0, dataVec_[0].size());
    pPlot_->yAxis->setRange(minValue, maxValue);
    pPlot_->replot();

    nPlot = 0;
}

void MultiplePlot::dataSelected(const QCPDataSelection &selection)
{
    //foreach (QCPDataRange dataRange, selection.dataRanges())
    //{
    //  QCPGraphDataContainer::const_iterator begin = graph->data()->at(dataRange.begin()); // get range begin iterator from index
    //  QCPGraphDataContainer::const_iterator end = graph->data()->at(dataRange.end()); // get range end iterator from index
    //  for (QCPGraphDataContainer::const_iterator it=begin; it!=end; ++it)
    //  {
    //    // iterator "it" will go through all selected data points, as an example, we calculate the value average
    //    sum += it->value;
    //  }
    //}

    //пока доступ к одному элементу
    int index{0};
    if(selection.dataRangeCount()) {
        index = selection.dataRange().begin();
    }
    std::cout << "in selected data slot. Index is: " << index << std::endl;
    emit itemSelected(index);
}



