#include "csv_settings.h"
#include <iostream>


CsvSettings::CsvSettings(QObject *parent) :
    QAbstractTableModel(parent),
    numOfRows_{0},
    kSettingsFileName{"settings/test.ini"},
    kSettingsNames_{"Name", "Scale", "IsIgnore", "IsBitset", "MesureUnit"},
    csvSettingsVec_{kSettingsNames_.size()}
{
    if (!QFileInfo::exists(kSettingsFileName)) {
        applyDefaultSettings();
    } else {
        QSettings csvQSettings{kSettingsFileName, QSettings::IniFormat};

        numOfRows_ = csvQSettings.value("const/num_of_rows").toInt();
        csvQSettings.beginGroup("values");
        QStringList tmpList_ = csvQSettings.childGroups();
        for (auto s : tmpList_) {
            csvQSettings.beginGroup(s);
            for (int i = 0; i < csvSettingsVec_.size(); ++i) {
                csvSettingsVec_[i].push_back(csvQSettings.value(kSettingsNames_.at(i)));
            }
            csvQSettings.endGroup();
        }
        csvQSettings.endGroup();
    }
    if (!isValid()) {
        applyDefaultSettings();
        QMessageBox msgBox;
        msgBox.setText(".ini file is broken please check settings");
        msgBox.exec();
    }
}

CsvSettings::~CsvSettings()
{
    SaveSettings("settings/test.ini");
}

bool CsvSettings::isValid()
{
    if(numOfRows_ && numOfRows_ == csvSettingsVec_.value(0).size()) {
        for (auto i : csvSettingsVec_) {
            for (auto j : i) {
                if (j.isNull()) {
                    return false;
                }
            }
        }
    return true;
    }
    return false;
}

QVariant CsvSettings::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole) {
        if ((orientation == Qt::Horizontal)
                && (section < kSettingsNames_.size())) {
            return kSettingsNames_.at(section);
        }
        //номера строк как столбцы в excel после Z следует AA, после AZ - BA
        if (orientation == Qt::Vertical) {
            if (section < ('Z' - 'A' + 1)) {
                return QString(char('A' + section));
            } else if (section < 2 * ('Z' - 'A' + 1)){
                return QString("A") + QString(char(section - ('Z' - 'A' + 1) + 'A'));
            } else if (section < 3 * ('Z' - 'A' + 1)) {
                return QString("B") + QString(char(section - 2 * ('Z' - 'A' + 1) + 'A'));
            } else {
                return QString("C") + QString(char(section - 3 * ('Z' - 'A' + 1) + 'A'));
            }
        }
    }
    return QVariant();
}

bool CsvSettings::setHeaderData(int section, Qt::Orientation orientation, const QVariant &value, int role)
{
    if (value != headerData(section, orientation, role)) {
        // FIXME: Implement me!
        emit headerDataChanged(orientation, section, section);
        return true;
    }
    return false;
}


int CsvSettings::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    return csvSettingsVec_[columns::kName].size();
}

int CsvSettings::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;
    return csvSettingsVec_.size();
}

QVariant CsvSettings::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();
    if(index.row() < csvSettingsVec_[columns::kName].size()
            && index.column() < csvSettingsVec_.size()
            && role == Qt::DisplayRole) {
        return csvSettingsVec_.value(index.column()).value(index.row());
    }
    return QVariant();
}

bool CsvSettings::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(index.row() > csvSettingsVec_[columns::kName].size()
            || index.column() > csvSettingsVec_.size()) {
        return false;
    }

    if (data(index, role) != value) {
        if(index.column() == columns::kName) {
            csvSettingsVec_[index.column()][index.row()] = value.toString();
        }
        if(index.column() == columns::kScale) {
            csvSettingsVec_[index.column()][index.row()] = value.toDouble();
        }
        if(index.column() == columns::kIsIgnore) {
            csvSettingsVec_[index.column()][index.row()] = value.toBool();
        }
        if(index.column() == columns::kIsBitset) {
            csvSettingsVec_[index.column()][index.row()] = value.toBool();
        }
        if(index.column() == columns::kMesureUnit) {
            csvSettingsVec_[index.column()][index.row()] = value.toString();
        }

        emit dataChanged(index, index, QVector<int>() << role);
        return true;
    }
    return false;
}

Qt::ItemFlags CsvSettings::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;
    return Qt::ItemIsEditable
            | Qt::ItemIsSelectable
            | Qt::ItemIsUserCheckable
            |Qt::ItemIsEnabled;
}

bool CsvSettings::insertRows(int row, int count, const QModelIndex &parent)
{
    if (row >= 0 && row <= rowCount()) {
        beginInsertRows(parent, row, row + count - 1);
        for (int i = row; i < row + count; ++i) {
            csvSettingsVec_[columns::kName].insert(i, QString("DefaultVal")
                                                  + QString().number(i));
            csvSettingsVec_[columns::kScale].insert(i, 1);
            csvSettingsVec_[columns::kIsIgnore].insert(i, false);
            csvSettingsVec_[columns::kIsBitset].insert(i, false);
            csvSettingsVec_[columns::kMesureUnit].insert(i, QString("t.u."));
        }
        endInsertRows();
        numOfRows_ = csvSettingsVec_.value(kName).size();
        return true;
    }
    return false;
}

bool CsvSettings::removeRows(int row, int count, const QModelIndex &parent)
{
    if(row >= 0 && row < rowCount() && row + count <= rowCount()) {
        beginRemoveRows(parent, row, row + count - 1);
        for (auto &vec : csvSettingsVec_) {
            vec.remove(row, count);
        }
        endRemoveRows();
        numOfRows_ = csvSettingsVec_.value(kName).size();
        return true;
    }
    return false;
}

void CsvSettings::SaveSettings(const QString &fileName)
{
    QSettings csvQSettings{fileName, QSettings::IniFormat};
    csvQSettings.clear();
    csvQSettings.beginGroup("Const");
    csvQSettings.setValue("num_of_rows", numOfRows_);
    csvQSettings.endGroup();
    csvQSettings.beginGroup("Values");
    for (int i = 0; i < csvSettingsVec_.value(0).size(); ++i) {
        QString tmpName{"Val"};
        if (i < 10) {
            tmpName += "0";
        }
        csvQSettings.beginGroup(tmpName + QString::number(i));
        csvQSettings.setValue(kSettingsNames_.value(columns::kName),
                              csvSettingsVec_.value(columns::kName).value(i));
        csvQSettings.setValue(kSettingsNames_.value(columns::kScale),
                              csvSettingsVec_.value(columns::kScale).value(i));
        csvQSettings.setValue(kSettingsNames_.value(columns::kIsIgnore),
                              csvSettingsVec_.value(columns::kIsIgnore).value(i));
        csvQSettings.setValue(kSettingsNames_.value(columns::kIsBitset),
                              csvSettingsVec_.value(columns::kIsBitset).value(i));
        csvQSettings.setValue(kSettingsNames_.value(columns::kMesureUnit),
                              csvSettingsVec_.value(columns::kMesureUnit).value(i));
        csvQSettings.endGroup();
    }
    csvQSettings.endGroup();
}

void CsvSettings::applyDefaultSettings()
{
    numOfRows_ = 4;
    for (auto &i : csvSettingsVec_) {
        i.clear();
    }
    for (int i = 0; i < numOfRows_; ++i) {
        csvSettingsVec_[columns::kName].push_back(QString("DefaultVal")
                                                  + QString().number(i));
        csvSettingsVec_[columns::kScale].push_back(1);
        csvSettingsVec_[columns::kIsIgnore].push_back(false);
        csvSettingsVec_[columns::kIsBitset].push_back(false);
        csvSettingsVec_[columns::kMesureUnit].push_back(QString("t.u."));
    }
}
