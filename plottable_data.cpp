#include "plottable_data.h"
#include <iostream>


PlottableData::PlottableData(CsvSettings *ps) :
    pCsvSettings_{ps},
    plotableNames_{},
    dataVec_{},
    timeVec_{},
    plotableMu_{}
{
}

bool PlottableData::getDataFromFile(const QString& fileName)
{
    int cntNotIgnore{0};
    for(int i = 0; i < pCsvSettings_->getNumOfCsvColumns(); ++i) {
        if (!pCsvSettings_->isRowIgnored(i)) {
            plotableNames_.push_back(pCsvSettings_->getRowName(i));
            plotableMu_.push_back(pCsvSettings_->getRowMU(i));
            ++cntNotIgnore;
        }
    }
    dataVec_.resize(cntNotIgnore);
    QFile inFile{fileName};
    if (false == inFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return false;
    }
    QTextStream fstream{&inFile};
    while (!fstream.atEnd()) {
        QStringList stringlist{fstream.readLine().split(';')};
        //stringlist.pop_back(); // избавляюсь от первода строки
        int counter{0};
        auto strIt{stringlist.begin()};
        for(int i = 0; i < pCsvSettings_->getNumOfCsvColumns(); ++i) {
            double tmpValue{};
            if (strIt != stringlist.end()) { //заполняю нулями отсуствующие значения
                tmpValue = strIt->toDouble();
                ++strIt;
            } else {
                tmpValue = 0;
            }

            if (!pCsvSettings_->isRowIgnored(i)) {
                dataVec_[counter].push_back(tmpValue
                        * pCsvSettings_->getRowScale(i));
                ++counter;
            }
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
    plotableNames_.clear();
    plotableMu_.clear();
    timeVec_.clear();
}
