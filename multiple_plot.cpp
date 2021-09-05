#include "multiple_plot.h"
#include <iostream>

const QList<QColor> MultiplePlot::kColors {
    Qt::red,        Qt::green,  Qt::blue,
    Qt::darkCyan,   Qt::yellow, Qt::magenta
};

MultiplePlot::MultiplePlot(PlottableData *pd,
                           QWidget *parent) :
    QWidget{parent},
    pPlotData_{pd}
{
    pMainLayout_ = new QHBoxLayout();
    pPlot_ = new QCustomPlot(this);

    //настраиваю шрифт и отображаю легенду
    QFont legendFont{font()};
    legendFont.setPointSize(10);
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
    QStringListModel *model{new QStringListModel(pPlotData_->getListColumnNames(),
                                                 this)};
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
        //рисую курсор для выбранного индекса
        if (!pPlotCursor_) {
            pPlotCursor_ = new QCPItemStraightLine(pPlot_);
        }
        double tmpCordX{pPlot_->graph()->data()->at(index)->key};
        double tmpCordY_1{pPlot_->yAxis->range().lower};
        double tmpCordY_2{pPlot_->yAxis->range().upper};
        pPlotCursor_->point1->setCoords(tmpCordX, tmpCordY_1);
        pPlotCursor_->point2->setCoords(tmpCordX, tmpCordY_2);
        std::cout << "in draw cursor" << std::endl;

        //вывожу все значения по выбранному индексу:
        if (!pPlotTextBlock_) {
            pPlotTextBlock_ = new QCPTextElement(pPlot_, "", 10);
        }
        pPlot_->axisRect()->insetLayout()->addElement(pPlotTextBlock_,
                                                      Qt::AlignLeft
                                                      | Qt::AlignTop);
        QString tmpText{"Values for x = "};
        QTextStream tmpOut(&tmpText);
        tmpOut << pPlot_->graph()->data()->at(index)->key << "\n";

        for (int i = 0; i < pPlot_->graphCount(); ++i) {
            tmpOut << pPlot_->graph(i)->name()
                   << " = "
                   << pPlot_->graph(i)->data()->at(index)->value

                   << " \n";
        }
        pPlotTextBlock_->setText(tmpText);
        pPlot_->replot();
    }

}

//------------------------------------SLOTS------------------------------------

void MultiplePlot::plotSelected()
{
    //чищу график
    pPlot_->clearGraphs();

    //получаю выбранные индексы
    QList<QModelIndex> selectedIndexList{
            poListView_->selectionModel()->selectedIndexes()};

    int nPlot{0};
    double minValue{0};
    double maxValue{0};
    for(QModelIndex iModelindex : selectedIndexList) {
        pPlot_->addGraph();
        int index{iModelindex.row()};
        //устанавливаю имя для легенды
        pPlot_->graph(nPlot)->setName(iModelindex.data().toString());
        //устанавливаю цвет
        QPen tmpPen{};
        if(nPlot < kColors.size()) {
            tmpPen.setColor(kColors[nPlot]);
        } else {
            tmpPen.setColor(Qt::gray);
        }
        tmpPen.setWidth(2);
        pPlot_->graph(nPlot)->setPen(tmpPen);
        tmpPen.setWidth(2);

        pPlot_->graph(nPlot)->setSelectable(QCP::SelectionType::stSingleData);
        connect(this->pPlot_->graph(nPlot),
                QOverload<const QCPDataSelection &>::of(
                        &QCPAbstractPlottable::selectionChanged), //! потому что не стоит перегружать сигналы https://stackoverflow.com/questions/16794695/connecting-overloaded-signals-and-slots-in-qt-5
                this, &MultiplePlot::dataSelected);

        pPlot_->graph(nPlot)->setData(pPlotData_->getTimeVec(),
                                      pPlotData_->getDataVec()[index]);
        double tmpMinValue{*std::min_element(
                pPlotData_->getDataVec()[index].begin(),
                pPlotData_->getDataVec()[index].end())};
        double tmpMaxValue{*std::max_element(
                pPlotData_->getDataVec()[index].begin(),
                pPlotData_->getDataVec()[index].end())};
        if (tmpMinValue < minValue)
            minValue = tmpMinValue;
        if (tmpMaxValue > maxValue)
            maxValue = tmpMaxValue;
        ++nPlot;
    }
    pPlot_->xAxis->setRange(0, pPlotData_->getDataVec()[0].size());
    pPlot_->yAxis->setRange(minValue, maxValue);
    pPlot_->replot();

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



