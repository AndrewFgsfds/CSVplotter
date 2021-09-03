#include "csv_settings.h"
#include <iostream>


CsvSettings::CsvSettings() :
    numOfColumns_{0},
    csvSettings_{QString("test.ini"), QSettings::IniFormat},
    columnNamesList_{},
    scalesVec_{},
    isIgnoreVec_{},
    isBitsetVec_{},
    measureUnitsList_{}
{
    storeCsvSettings(); //раскомментировать чтобы заполнить настройки в первый раз
    numOfColumns_ = csvSettings_.value("const/num_of_columns").toInt();
    csvSettings_.beginGroup("values");
    QStringList tmpList_ = csvSettings_.childGroups();
    for (auto s : tmpList_) {
        csvSettings_.beginGroup(s);
        columnNamesList_.push_back(csvSettings_.value("Name").toString());
        scalesVec_.push_back(csvSettings_.value("Scale", 1).toDouble());
        isIgnoreVec_.push_back(csvSettings_.value("IsIgnore", 0).toBool());
        isBitsetVec_.push_back(csvSettings_.value("IsBitset", 0).toBool());
        measureUnitsList_.push_back(csvSettings_.value("MesureUnit").toString());
        csvSettings_.endGroup();
    }
    csvSettings_.endGroup();
}

CsvSettings::~CsvSettings()
{
    //csvSettings_.sync(); пока не реализую форму правки настроек
}

bool CsvSettings::isValid()
{
    if(!numOfColumns_)
        return false;
    if((numOfColumns_ == columnNamesList_.size())
            && (numOfColumns_ == scalesVec_.size())
            && (numOfColumns_ == isIgnoreVec_.size())
            && (numOfColumns_ == isBitsetVec_.size())) {
        return true;
    } else {
        return false;
    }
}

void CsvSettings::storeCsvSettings()
{
    csvSettings_.clear();
    csvSettings_.beginGroup("Const");
    csvSettings_.setValue("num_of_columns", 5);
    csvSettings_.endGroup();
    csvSettings_.beginGroup("Values");
    for (int i = 0; i < 5; ++i) {
        QString tmpName{"Val"};
        if (i < 10) {
            tmpName += "0";
        }
        csvSettings_.beginGroup(tmpName + QString::number(i));
        csvSettings_.setValue("Name", "val" + QString::number(i));
        csvSettings_.setValue("Scale", QString::number(1));
        csvSettings_.setValue("IsIgnore", QString::number(false));
        csvSettings_.setValue("IsBitset", QString::number(false));
        csvSettings_.setValue("MesureUnit", "t.u.");
        csvSettings_.endGroup();
    }
    csvSettings_.endGroup();
}
