#include "main_window.h"
#include <QtWidgets>
#include <QFile>
#include <algorithm>


#include <iostream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow{parent},
    vecPtrMltplPlot_{}
{
    createMenu();
    createToolBar();
    createCentralWidget();
    spWindowSettings_ = QSharedPointer<WindowSettings>(new WindowSettings());
    QRect storedRect{spWindowSettings_.get()->getWindowGeometry()};
    if(storedRect.isValid()) {
        this->setGeometry(storedRect);
    } else { // тут стандартные настройки если в .ini некорректные значения или пусто
        const QRect availableGeometry = QApplication::desktop()->availableGeometry(this);
        resize(availableGeometry.width() / 2, availableGeometry.height() / 2);
        move((availableGeometry.width() - this->width()) / 2,
             (availableGeometry.height() - this->height()) / 2);
    }

    spCsvSettings_ = QSharedPointer<CsvSettings>(new CsvSettings());
    spPlotData_ = QSharedPointer<PlottableData>(new PlottableData(spCsvSettings_.get()));
}

MainWindow::~MainWindow()
{
    spWindowSettings_.get()->storeWindowGeometry(this->geometry());
}

void MainWindow::createMenu()
{
    QMenuBar *pMenuBar{new QMenuBar(this)};
    QMenu *pFileMenu{new QMenu(this)};
    pFileMenu->setTitle("File");
    QAction* pOpenAct{pFileMenu->addAction("Open file")};
    connect(pOpenAct, &QAction::triggered, this, &MainWindow::openFile);
    QAction* pCloseAct = pFileMenu->addAction("Close file");
    connect(pCloseAct, &QAction::triggered, this, &MainWindow::cleanData);
    QAction* pQuitAct = pFileMenu->addAction("Quit");
    connect(pQuitAct, &QAction::triggered, this, &MainWindow::close);
    pMenuBar->addMenu(pFileMenu);
    QMenu *pSettingsMenu{new QMenu(this)};
    pSettingsMenu->setTitle("Settings");
    QAction* pSettings{pSettingsMenu->addAction("CSV settings")};
    connect(pSettings, &QAction::triggered, this, &MainWindow::openSettings);
    pMenuBar->addMenu(pSettingsMenu);

    this->layout()->setMenuBar(pMenuBar);
    this->statusBar()->setVisible(true);
}

void MainWindow::createToolBar()
{
    pTopToolBar_ = new QToolBar(this);
    addToolBar(Qt::TopToolBarArea, pTopToolBar_);

    //кнопка открытия файла вызывает стандартный диалог открытия,
    //файл парсим файл и заполняем вектор dataVec_, закрываю файл
    QIcon openIcon{":/logviewer/icons/document.png"};
    QAction *pOpenAct{new QAction(openIcon, "&Open...", this)};
    pOpenAct->setShortcuts(QKeySequence::Open); //make "ctrl-o" works as Open
    pOpenAct->setStatusTip("Open .CSV file");
    connect(pOpenAct, &QAction::triggered, this, &MainWindow::openFile);
    pTopToolBar_->addAction(pOpenAct);

    QIcon closeIcon{":/logviewer/icons/delete.png"};
    QAction *pCloseAct{new QAction(closeIcon, "&Close file...", this)};
    pCloseAct->setShortcuts(QKeySequence::Close); //make "Ctrl+F4, Ctrl+W" works as Open
    pCloseAct->setStatusTip("Close .CSV file");
    connect(pCloseAct, &QAction::triggered, this, &MainWindow::cleanData);
    pTopToolBar_->addAction(pCloseAct);

    pTopToolBar_->addSeparator();

    QIcon addPlotIcon{":/logviewer/icons/plus-outline.png"};
    QAction *pAddPlotAct{new QAction(addPlotIcon, "&Add new plot...", this)};
    pAddPlotAct->setStatusTip("Add new plot area");
    connect(pAddPlotAct, &QAction::triggered, this, &MainWindow::addPlot);
    pTopToolBar_->addAction(pAddPlotAct);

    QIcon removePlotIcon{":/logviewer/icons/minus-outline.png"};
    QAction *pRemovePlotAct{new QAction(removePlotIcon, "&Remove last plot...", this)};
    pRemovePlotAct->setStatusTip("Remove last plot area");
    connect(pRemovePlotAct, &QAction::triggered, this, &MainWindow::removePlot);
    pTopToolBar_->addAction(pRemovePlotAct);

}

void MainWindow::createCentralWidget()
{
    pSplit_ = new QSplitter(Qt::Vertical, this);
    pSplit_->setOpaqueResize(false); //для ускорения работы графики будут перерисованы после завершения drug_op
    QScrollArea *scrAr{new QScrollArea(this)};
    scrAr->setWidget(pSplit_);
    setCentralWidget(scrAr);
    scrAr->setWidgetResizable(true);
}

//---------------------Slots-------------------------------------

void MainWindow::updateAllRanges(const QCPRange &newRange)
{
    //много лишних вызовов т.к. каждый из графиков отправляет свой сигнал после обновления
    //контроль отправки нового сигнала на реализации xAxis->setRange()

    for(auto &it : vecPtrMltplPlot_) {
        //if (it.get()->getRange() != newRange) {
           it.get()->updateRangeAxisX(newRange);
        //}
    }
}

void MainWindow::updateCursor(int index)
{
    for(auto &it : vecPtrMltplPlot_) {
        it.get()->drawCursor(index);
    }
}


void MainWindow::openFile()
{
    cleanData();                                            //очищаю перед открытием, в случае повторного открытия файла
    QString fileName = QFileDialog::getOpenFileName (       //тут утечка памяти, надо переделать на создание QFileDialog()
                this, "Open File",
                "./",                                       //relative path
                "csv (*.csv)");                             //filtering .csv files only
    if (!fileName.isEmpty())
        statusBar()->showMessage(fileName);
    if (spPlotData_.get()->getDataFromFile(fileName)) {
        addPlot();
        statusBar()->showMessage("Success!");
    } else {
        spPlotData_.get()->cleanData();
        statusBar()->showMessage("Fail!");
    }
}

void MainWindow::cleanData()                            //файл закрывается сразу в parseFile
{
    spPlotData_.get()->cleanData();
    vecPtrMltplPlot_.clear();
    pSplit_->refresh();
    //pSplit_->setMinimumHeight(500*vecpQstmPlot_.size());
}

void MainWindow::addPlot()
{
    vecPtrMltplPlot_.push_back(QSharedPointer<MultiplePlot>(
            new MultiplePlot(spPlotData_.get(), this)));
    //синхронизация оси X
    connect(vecPtrMltplPlot_.back().get(),
            &MultiplePlot::axisXRangeChanged,
            this,
            &MainWindow::updateAllRanges);
    //выбор точки для курсора и вывода данных
    connect(vecPtrMltplPlot_.back().get(),
            &MultiplePlot::itemSelected,
            this,
            &MainWindow::updateCursor);
    pSplit_->addWidget(vecPtrMltplPlot_.back().get());
    pSplit_->refresh();

    pSplit_->setMinimumHeight(300*vecPtrMltplPlot_.size());
}

void MainWindow::removePlot()
{
    if (vecPtrMltplPlot_.size()) {
        vecPtrMltplPlot_.pop_back();
    }
    pSplit_->refresh();
    pSplit_->setMinimumHeight(300*vecPtrMltplPlot_.size());
}

void MainWindow::openSettings()
{
    cleanData();
    SettingsWidget *settingsW = new SettingsWidget(spCsvSettings_.get());
    settingsW->show();
    settingsW->setAttribute(Qt::WA_DeleteOnClose, true);
}
