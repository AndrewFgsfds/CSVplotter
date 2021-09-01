#ifndef PLOTTERSETTINGS_H
#define PLOTTERSETTINGS_H
#include <QSettings>
#include <QRect>
#include <QApplication>

//сохраняю и считываю настройки из двух файлов
//seetings.ini для настроек внешнего вида
//и передаваемый извне csvFilename

class PlotterSettings
{
public:
    explicit PlotterSettings(const QString& csvFilename,
                             const QString& screenFilename = "settings.ini");
    virtual ~PlotterSettings();

    QRect getWindowGeometry() const;
    void storeWindowGeometry(const QRect&);

    int getNumOfColumns() const {return numOfColumns_;}
    void storeCsvSettings();
    const QStringList& getlistColumnNames() const {return columnNamesList_;}

private:
    int numOfColumns_;
    QSettings csvSettings_;
    QSettings screenSettings_;
    QStringList columnNamesList_;
};

#endif // PLOTTERSETTINGS_H
