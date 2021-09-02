#include "plottable_data.h"

PlottableData::PlottableData(CsvSettings *ps) :
    pCsvSettings_{ps},
    dataVec_{pCsvSettings_->getNumOfColumns()},
    timeVec_{}
{
}

bool PlottableData::getDataFromFile(const QString& fileName)
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
            if (counter < pCsvSettings_->getNumOfColumns()) {
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

void PlottableData::cleanData()
{
    for (auto &i : dataVec_) {
        i.clear();
    }
    timeVec_.clear();
}
