#include "csv_settings.h"
#include <iostream>


CsvSettings::CsvSettings() :
    numOfColumns_{0},
    csvSettings_{QString("test.ini"), QSettings::IniFormat},
    columnNamesList_{}
{
    storeCsvSettings(); //раскомментировать чтобы заполнить настройки в первый раз
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

CsvSettings::~CsvSettings()
{
    //csvSettings_.sync(); пока не реализую форму правки настроек
}

void CsvSettings::storeCsvSettings()
{
    csvSettings_.clear();
    csvSettings_.beginGroup("Const");
    csvSettings_.setValue("num_of_columns", 60);
    csvSettings_.endGroup();
    csvSettings_.beginGroup("Values");
    for (int i = 0; i < 60; ++i) {
        QString tmpName{"Val"};
        if (i < 10) {
            tmpName += "0";
        }
        csvSettings_.beginGroup(tmpName + QString::number(i));
        csvSettings_.setValue("Name", "val" + QString::number(i));
        csvSettings_.endGroup();
    }
    csvSettings_.endGroup();
}
