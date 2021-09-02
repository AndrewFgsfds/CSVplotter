#ifndef CSVSETTINGS_H
#define CSVSETTINGS_H
#include <QSettings>
#include <QRect>
#include <QApplication>

//сохраняю и считываю настройки из двух файлов
//seetings.ini для настроек внешнего вида
//и передаваемый извне csvFilename

class CsvSettings
{
public:
    explicit CsvSettings(const QString& csvFilename);
    virtual ~CsvSettings();

    int getNumOfColumns() const {return numOfColumns_;}
    void storeCsvSettings();
    const QStringList& getlistColumnNames() const {return columnNamesList_;}

private:
    int numOfColumns_;
    QSettings csvSettings_;
    QStringList columnNamesList_;
};

#endif // CSVSETTINGS_H
