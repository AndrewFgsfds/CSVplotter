#include "main_window.h"

bool MainWindow::parseFile(const QString& fileName)
{
    QFile inFile{fileName};
    if (false == inFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return false;
    }
    QTextStream fstream{&inFile};
    while (!fstream.atEnd()) {
        QStringList stringlist{fstream.readLine().split(';')};
        stringlist.pop_back(); // избавляюсь от первода строки
        int counter{0}; // для контроля за уходом за границу вектора
        for (QString ent : stringlist) {
            if (counter < kColumnsInArray_) {
                dataVec_[counter].push_back(ent.toDouble());
            }
            ++counter;
        }
    }
    for (int i = 0; i < dataVec_[0].size(); ++i) {
        timeVec_.push_back(i);
    }
    inFile.close();
    return true;
}
