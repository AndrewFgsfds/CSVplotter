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
    //рисую крусор по индексу вектора с данными
    void drawCursor(int index);
    const QCPRange getRange() const {
        return pPlot_->xAxis->range();
    }

    static const QList<QColor> kColors;

signals:
    void axisXRangeChanged(const QCPRange &newRange); //
    void itemSelected(int index);

public slots:


private slots:
    void plotSelected();
    void dataSelected(const QCPDataSelection & selection);

private:
    void createRightWidget();
    const QVector<double> &timeVec_;
    const QVector<QVector<double>> &dataVec_;
    const QStringList &listColumnNames_;

    int selectedIndex_ = 0;

    QCustomPlot* pPlot_ = nullptr;
    QListView *poListView_ = nullptr;
    QHBoxLayout *pMainLayout_ = nullptr;
    QCPItemStraightLine *pPlotCursor_ = nullptr;
    QCPTextElement *pPlotTextBlock_ = nullptr;

};

#endif // MULTIPLEPLOT_H
