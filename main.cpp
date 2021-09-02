#include "main_window.h"
#include "csv_settings.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CsvSettings plotSettings("test.ini");
    MainWindow w(&plotSettings);
    w.show();
    return a.exec();
}
