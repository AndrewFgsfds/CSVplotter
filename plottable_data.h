#ifndef PLOTTABLEDATA_H
#define PLOTTABLEDATA_H
#include <QVector>
#include <QList>
#include "csv_settings.h"
#include <QTextStream>


class PlottableData
{
public:
    explicit PlottableData(CsvSettings *ps);
    bool getDataFromFile(const QString& fileName);
    void cleanData();
private:
    CsvSettings *pPlotSettings_;

    QVector<QVector<double>> dataVec_;
    QVector<double> timeVec_;
};

#endif // PLOTTABLEDATA_H
