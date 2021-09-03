#ifndef CSVSETTINGS_H
#define CSVSETTINGS_H
#include <QSettings>
#include <QRect>
//#include <QApplication>

class CsvSettings
{
public:
    explicit CsvSettings();
    ~CsvSettings();

    bool isValid();
    int getNumOfColumns() const {return numOfColumns_;}
    void storeCsvSettings();
    const QStringList& getColumnNamesList() const {return columnNamesList_;}
    const QVector<double>& getScalesVec() const {return scalesVec_;}
    const QVector<bool>& getIgnoreVec() const {return isIgnoreVec_;}
    const QVector<bool>& getBitsetVec() const {return isBitsetVec_;}
    const QStringList& getMeusreUnitsList() const {return measureUnitsList_;}


private:
    int numOfColumns_;
    QSettings csvSettings_;
    QStringList columnNamesList_;
    QVector<double> scalesVec_;
    QVector<bool> isIgnoreVec_;
    QVector<bool> isBitsetVec_;
    QStringList measureUnitsList_;
};

#endif // CSVSETTINGS_H
