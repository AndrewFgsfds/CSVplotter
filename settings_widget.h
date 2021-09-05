#ifndef SETTINGS_WIDGET_H
#define SETTINGS_WIDGET_H
#include "csv_settings.h"
#include <QWidget>

namespace Ui {
class SettingsWidget;
}

class SettingsWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SettingsWidget(CsvSettings *pSettings, QWidget *parent = nullptr);
    ~SettingsWidget();

private slots:
    void on_pbApplyRows_clicked();

private:
    Ui::SettingsWidget *ui;
    CsvSettings *pSettings_;
};

#endif // SETTINGS_WIDGET_H
