#ifndef MULTIPLEPLOT_H
#define MULTIPLEPLOT_H

#include <QObject>
#include <QWidget>
#include "qcustomplot.h"


class MultiplePlot : public QWidget
{
    Q_OBJECT

public:
    explicit MultiplePlot(const QVector<double> &timeVec_,
                          const QVector<QVector<double>> &dataVec_,
                          const QStringList &listColumnNames_,
                          QWidget *parent = nullptr);

    //при обновлении одного из графиков обновляю ось X всех
    void updateRangeAxisX(const QCPRange &newRange);
    const QCPRange getRange() const {
        return pPlot_->xAxis->range();
    }

signals:
    void axisXRangeChanged(const QCPRange &newRange); //

public slots:


private slots:
    void replot();

private:
    void createRightWidget();
    const QVector<double> &timeVec_;
    const QVector<QVector<double>> &dataVec_;
    const QStringList &listColumnNames_;

    QCustomPlot* pPlot_;
    QListView *poListView_;
    QHBoxLayout *pMainLayout_;
};

#endif // MULTIPLEPLOT_H
