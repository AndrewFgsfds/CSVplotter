#ifndef CSVSETTINGS_H
#define CSVSETTINGS_H
#include <QSettings>
#include <QFileInfo>
#include <QAbstractTableModel>
#include <QMessageBox>

class CsvSettings : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit CsvSettings(QObject *parent = nullptr);
    ~CsvSettings() override;

    enum columns {
        kName = 0,
        kScale,
        kIsIgnore,
        kIsBitset,
        kMesureUnit
    };

    QVariant headerData(int section, Qt::Orientation orientation,
                        int role = Qt::DisplayRole) const override;

    bool setHeaderData(int section, Qt::Orientation orientation,
                       const QVariant &value, int role = Qt::EditRole) override;

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index,
                  int role = Qt::DisplayRole) const override;

    // Editable:
    bool setData(const QModelIndex &index, const QVariant &value,
                 int role = Qt::EditRole) override;

    Qt::ItemFlags flags(const QModelIndex& index) const override;

    // Add data:
    bool insertRows(int row, int count,
                    const QModelIndex &parent = QModelIndex()) override;

    // Remove data:
    bool removeRows(int row, int count,
                    const QModelIndex &parent = QModelIndex()) override;

    bool isValid();
    int getNumOfCsvColumns() const {return numOfRows_;} //количество столбцов в csv файле равно количеству строк в настройках
    void SaveSettings(const QString &fileName);
    void LoadSettings(const QString &fileName);
    QString getRowName(int RowNumber) {
        return csvSettingsVec_.value(columns::kName).value(RowNumber).toString();
    }
    double getRowScale(int RowNumber) {
        return csvSettingsVec_.value(columns::kScale).value(RowNumber).toDouble();
    }
    bool isRowIgnored(int RowNumber) {
        return csvSettingsVec_.value(columns::kIsIgnore).value(RowNumber).toBool();
    }
    bool isRowBitset(int RowNumber) {
        return csvSettingsVec_.value(columns::kIsBitset).value(RowNumber).toBool();
    }
    QString getRowMU(int RowNumber) {
        return csvSettingsVec_.value(columns::kMesureUnit).value(RowNumber).toString();
    }

private:
    void applyDefaultSettings();
    int numOfRows_;
    const QString kSettingsFileName;
    const QStringList kSettingsNames_;
    QVector<QVector<QVariant>> csvSettingsVec_;
};

#endif // CSVSETTINGS_H
