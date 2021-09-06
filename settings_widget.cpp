#include "settings_widget.h"
#include "ui_settings_widget.h"
#include <iostream>

SettingsWidget::SettingsWidget(CsvSettings *pSettings, QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SettingsWidget),
    pSettings_{pSettings}
{
    ui->setupUi(this);
    ui->csvSettingsView->setModel(pSettings_);
    ui->leRowsNumber->setText(QString().number(pSettings_->rowCount()));
}

SettingsWidget::~SettingsWidget()
{
    delete ui;
}

void SettingsWidget::on_pbApplyRows_clicked()
{
    int newNum = ui->leRowsNumber->text().toInt();
    if(newNum > 99) {
        ui->leRowsNumber->setText("99");
        newNum = 99;
    } else if (newNum < 0) {
        ui->leRowsNumber->setText("0");
        newNum = 0;
    }
    if(newNum > pSettings_->getNumOfCsvColumns()) {
        pSettings_->insertRows(pSettings_->getNumOfCsvColumns(),
                               newNum - pSettings_->getNumOfCsvColumns());
    } else if (newNum < pSettings_->getNumOfCsvColumns()) {
        pSettings_->removeRows(newNum,
                               pSettings_->getNumOfCsvColumns() - newNum);
    }
}
