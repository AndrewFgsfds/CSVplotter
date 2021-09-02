#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include "qcustomplot.h"
#include "multiple_plot.h"
#include "window_settings.h"
#include "csv_settings.h"
#include "plottable_data.h"

#include <QMainWindow>
#include <QVBoxLayout>
#include <vector>
#include <memory>




class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(CsvSettings *ps, QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    //принимаю сигнал о том что ось изменилась и применяю ко всем остальным графикам
    void updateAllRanges(const QCPRange &newRange);
    //при выборе точки на графике обновляю положение куросора и вывожу все значения по данному индексу
    void updateCursor(int index);

private slots:
    void openFile();
    void cleanData();               //реально только чищу вектора, файл закрывается сразу в parseFile
    void addPlot();
    void removePlot();

private:
    void createMenu();
    void createToolBar();
    void createCentralWidget();
    void refreshPlots();
    bool parseFile(const QString&);

    CsvSettings *pPlotSettings_;
    WindowSettings windowSettings_;
    QVBoxLayout *pQlayout_ = nullptr;
    QSplitter *pSplit_ = nullptr;
    QToolBar *pTopToolBar_ = nullptr;
    QWidget *centralWidget_ = nullptr;

    QVector<QVector<double>> dataVec_;
    std::vector<std::unique_ptr<MultiplePlot>> vecPtrMltplPlot_;
    QVector<double> timeVec_;
};
#endif // MAINWINDOW_H
