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
    listColumnNames_{listColumnNames},
    pPlot_{new QCustomPlot(this)},
    poListView_{new QListView(this)},
    pMainLayout_{new QHBoxLayout()}
{
    pMainLayout_->addWidget(pPlot_);

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
    QObject::connect(this->pPlot_->xAxis,
                     QOverload<const QCPRange &>::of(&QCPAxis::rangeChanged), //! потому что не стоит перегружать сигналы
                     this, &MultiplePlot::axisXRangeChanged);

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
    poListView_->setModel(model);
    poListView_->setSelectionMode(QAbstractItemView::MultiSelection);
    pSubLayout->addWidget(poListView_);
    QPushButton *pButtonPlot{new QPushButton("Plot", this)};
    connect(pButtonPlot, &QPushButton::clicked, this, &MultiplePlot::replot);
    pSubLayout->addWidget(pButtonPlot);
    pSubLayout->setMargin(0);
    pMainLayout_->addWidget(rightWidget);
}

void MultiplePlot::replot()
{
    //получаю выбранные индексы
    QList<QModelIndex> selectedIndexList{poListView_->selectionModel()->selectedIndexes()};
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

        pPlot_->graph(nPlot)->setData(timeVec_, dataVec_[index]);
        double tmpMinValue{*std::min_element(dataVec_[index].begin(), dataVec_[index].end())};
        double tmpMaxValue{*std::max_element(dataVec_[index].begin(), dataVec_[index].end())};
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

void MultiplePlot::updateRangeAxisX(const QCPRange &newRange)
{
    //if (pPlot_->xAxis->range() != newRange) {
        pPlot_->xAxis->setRange(newRange); //проверку от цикличного изменения осуществляет библиотека
    //}
    pPlot_->replot();
}

