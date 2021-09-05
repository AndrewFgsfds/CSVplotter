#ifndef PLOTTABLEDATA_H
#define PLOTTABLEDATA_H
#include "csv_settings.h"
#include <QVector>
#include <QList>
#include <QTextStream>
#include <QFile>


class PlottableData
{
public:
    explicit PlottableData(CsvSettings *cs);
    bool getDataFromFile(const QString& fileName);
    void cleanData();

    //interface with multiple plot
    const QVector<double>& getTimeVec() const {return timeVec_;}
    const QVector<QVector<double>>& getDataVec() const {return dataVec_;}
    const QStringList& getListColumnNames() const {return plotableNames_;}
    const QStringList& getMuNames() const {return plotableMu_;}


private:
    CsvSettings *pCsvSettings_;
    QStringList plotableNames_;
    QVector<QVector<double>> dataVec_;
    QVector<double> timeVec_;
    QStringList plotableMu_;
};

#endif // PLOTTABLEDATA_H
