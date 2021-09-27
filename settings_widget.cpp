#include "settings_widget.h"
#include "ui_settings_widget.h"
#include <QFileDialog>
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
    if (newNum > pSettings_->getNumOfCsvColumns()) {
        pSettings_->insertRows(pSettings_->getNumOfCsvColumns(),
                               newNum - pSettings_->getNumOfCsvColumns());
    } else if (newNum < pSettings_->getNumOfCsvColumns()) {
        pSettings_->removeRows(newNum,
                               pSettings_->getNumOfCsvColumns() - newNum);
    }
}

void SettingsWidget::on_pbSaveButton_clicked()
{
    QString settingsName = QFileDialog::getSaveFileName (       //тут утечка памяти, надо переделать на создание QFileDialog()
                this, "Open File",
                "./settings",                                       //relative path
                "ini (*.ini)");                             //filtering .csv files only
    pSettings_->SaveSettings(settingsName);
}

void SettingsWidget::on_pbLoadButton_clicked()
{
    ui->csvSettingsView->setModel(nullptr);
    QString settingsName = QFileDialog::getOpenFileName (       //тут утечка памяти, надо переделать на создание QFileDialog()
                this, "Open File",
                "./settings",                                       //relative path
                "ini (*.ini)");                             //filtering .csv files only
    pSettings_->LoadSettings(settingsName);
    ui->leRowsNumber->setText(QString().number(pSettings_->rowCount()));
    ui->csvSettingsView->setModel(pSettings_);
}
