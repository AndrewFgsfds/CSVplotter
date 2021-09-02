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
    const QStringList& getlistColumnNames() const {return columnNamesList_;}
    const QVector<double>& getScalesVec() const {return scalesVec;}
    const QVector<bool>& getIgnoreVec() const {return isIgnoreVec;}

private:
    int numOfColumns_;
    QSettings csvSettings_;
    QStringList columnNamesList_;
    QVector<double> scalesVec;
    QVector<bool> isIgnoreVec;
};

#endif // CSVSETTINGS_H
