#ifndef PLOTTABLEDATA_H
#define PLOTTABLEDATA_H
#include <QVector>
#include <QList>
#include "plotter_settings.h"
#include <QTextStream>


class PlottableData
{
public:
    explicit PlottableData(PlotterSettings *ps);
    bool getDataFromFile(const QString& fileName);
    void cleanData();
private:
    PlotterSettings *pPlotSettings_;

    QVector<QVector<double>> dataVec_;
    QVector<double> timeVec_;
};

#endif // PLOTTABLEDATA_H
