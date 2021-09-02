#include "window_settings.h"
#include <iostream>


WindowSettings::WindowSettings(const QString& screenFilename) :
    screenSettings_{screenFilename, QSettings::IniFormat}
{
}

WindowSettings::~WindowSettings()
{
    screenSettings_.sync();
}

QRect WindowSettings::getWindowGeometry() const
{
    int pos_x{screenSettings_.value("mainwindow/pos_X", 0).toInt()};
    int pos_y{screenSettings_.value("mainwindow/pos_Y", 0).toInt()};
    int height{screenSettings_.value("mainwindow/Height", 0).toInt()};
    int width{screenSettings_.value("mainwindow/Width", 0).toInt()};
    if (pos_x && pos_y && height && width) {
        return QRect(pos_x, pos_y, width, height);
    }
    return QRect();
}

void WindowSettings::storeWindowGeometry(const QRect& rect)
{
    screenSettings_.beginGroup("mainwindow");
    screenSettings_.setValue("pos_X", rect.x());
    screenSettings_.setValue("pos_Y", rect.y());
    screenSettings_.setValue("Height", rect.height());
    screenSettings_.setValue("Width", rect.width());
    screenSettings_.endGroup();
}

