#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>
#include "qcustomplot.h"
#include "multiple_plot.h"
#include <vector>
#include <memory>




class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
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
    void readSettings();
    void saveSettings();
    void createToolBar();
    void createCentralWidget();
    void refreshPlots();
    bool parseFile(const QString&);

    constexpr static int kColumnsInArray_{20}; //число столбцов в файле, перенести в .ini

    QVBoxLayout *pQlayout_ = nullptr;
    QSplitter *pSplit_ = nullptr;
    QToolBar *pTopToolBar_ = nullptr;
    QWidget *centralWidget_ = nullptr;

    QVector<QVector<double>> dataVec_;
    std::vector<std::unique_ptr<MultiplePlot>> vecPtrMltplPlot_;
    QVector<double> timeVec_;
    QStringList listColumnNames_;

};
#endif // MAINWINDOW_H
