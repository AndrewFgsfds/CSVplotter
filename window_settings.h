#ifndef WINDOWSETTINGS_H
#define WINDOWSETTINGS_H
#include <QSettings>
#include <QRect>
#include <QApplication>

//сохраняю и считываю настройки из двух файлов
//seetings.ini для настроек внешнего вида
//и передаваемый извне csvFilename

class WindowSettings
{
public:
    explicit WindowSettings(const QString& screenFilename = "settings.ini");
    virtual ~WindowSettings();

    QRect getWindowGeometry() const;
    void storeWindowGeometry(const QRect&);

private:
    QSettings screenSettings_;
};

#endif // WINDOWSETTINGS_H
