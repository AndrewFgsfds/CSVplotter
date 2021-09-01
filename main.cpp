#include "main_window.h"
#include "plotter_settings.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    PlotterSettings plotSettings("test.ini");
    MainWindow w(&plotSettings);
    w.show();
    return a.exec();
}
