#include "main_window.h"

void MainWindow::readSettings()
{
    QString tmpstr{"settings.ini"};
    //QFile file(tmpstr);
    //file.open(QIODevice::ReadWrite);
    //file.close();
    QSettings settings(tmpstr, QSettings::IniFormat);
    int pos_x = settings.value("pos_X", -1).toInt();
    int pos_y = settings.value("pos_Y", -1).toInt();
    int tmp_height = settings.value("Height", -1).toInt();
    int tmp_width = settings.value("Width", -1).toInt();
    if((pos_x > 0) && (pos_y > 0) && (tmp_height > 0) && (tmp_width > 0)) {
        this->setGeometry(pos_x, pos_y, tmp_width, tmp_height);
    } else { // тут стандартные настройки если в .ini некорректные значения или пусто
        const QRect availableGeometry = QApplication::desktop()->availableGeometry(this);
        resize(availableGeometry.width() / 2, availableGeometry.height() / 2);
        move((availableGeometry.width() - this->width()) / 2,
        (availableGeometry.height() - this->height()) / 2);
    }

    for (int i = 0; i < kColumnsInArray_; ++i){
        listColumnNames_.push_back("val" + QString::number(i));
    }
}

void MainWindow::saveSettings() //перезаписываю файл целиком при выходе - исправить чтобы сохранять другие настройки и комменты
{
    QSettings settings("settings.ini", QSettings::IniFormat);
    settings.setValue("pos_X", this->pos().x());
    settings.setValue("pos_Y", this->pos().y());
    settings.setValue("Height", this->size().height());
    settings.setValue("Width", this->size().width());
}
