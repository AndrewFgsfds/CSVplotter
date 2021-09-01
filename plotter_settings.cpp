#include "plotter_settings.h"
#include <iostream>


PlotterSettings::PlotterSettings(const QString& csvfilename,
                                 const QString& screenFilename) :
    numOfColumns_{0},
    csvSettings_{csvfilename, QSettings::IniFormat},
    screenSettings_{screenFilename, QSettings::IniFormat},
    columnNamesList_{}
{
    //storeCsvSettings(); //раскомментировать чтобы заполнить настройки в первый раз
    numOfColumns_ = csvSettings_.value("const/num_of_columns").toInt();
    csvSettings_.beginGroup("values");
    QStringList tmpList_ = csvSettings_.childGroups();
    for (auto s : tmpList_) {
        csvSettings_.beginGroup(s);
        columnNamesList_.push_back(csvSettings_.value("Name").toString());
        csvSettings_.endGroup();
    }
    csvSettings_.endGroup();
}

PlotterSettings::~PlotterSettings()
{
    //csvSettings_.sync(); пока не реализую форму правки настроек
    screenSettings_.sync();
}

QRect PlotterSettings::getWindowGeometry() const
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

void PlotterSettings::storeWindowGeometry(const QRect& rect)
{
    screenSettings_.beginGroup("mainwindow");
    screenSettings_.setValue("pos_X", rect.x());
    screenSettings_.setValue("pos_Y", rect.y());
    screenSettings_.setValue("Height", rect.height());
    screenSettings_.setValue("Width", rect.width());
    screenSettings_.endGroup();
}

void PlotterSettings::storeCsvSettings()
{
    csvSettings_.clear();
    csvSettings_.beginGroup("Const");
    csvSettings_.setValue("num_of_columns", 20);
    csvSettings_.endGroup();
    csvSettings_.beginGroup("Values");
    for (int i = 0; i < 20; ++i) {
        if (i < 10) {
            csvSettings_.beginGroup("Val0" + QString::number(i));
            csvSettings_.setValue("Name", "val0" + QString::number(i));
            csvSettings_.endGroup();
        } else {
            csvSettings_.beginGroup("Val" + QString::number(i));
            csvSettings_.setValue("Name", "val" + QString::number(i));
            csvSettings_.endGroup();
        }
    }
    csvSettings_.endGroup();
}
