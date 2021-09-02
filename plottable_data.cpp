#include "plottable_data.h"

PlottableData::PlottableData(CsvSettings *ps) :
    pCsvSettings_{ps},
    plotableNames{},
    dataVec_{},
    timeVec_{}
{
    int cntNotIgnore{0};
    for(int i = 0; i < pCsvSettings_->getlistColumnNames().size(); ++i) {
        if (!pCsvSettings_->getIgnoreVec().at(i)) {
            plotableNames.push_back(pCsvSettings_->getlistColumnNames().at(i));
            ++cntNotIgnore;
        }
    }
    dataVec_.resize(cntNotIgnore);
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
        //stringlist.pop_back(); // избавляюсь от первода строки
        int counter{0};
        auto strIt{stringlist.begin()};
        for(int i = 0; i < pCsvSettings_->getIgnoreVec().size(); ++i) {
            double tmpValue{};
            if (strIt != stringlist.end()) { //заполняю нулями отсуствующие занчения
                tmpValue = strIt->toDouble();
                ++strIt;
            } else {
                tmpValue = 0;
            }

            if (!pCsvSettings_->getIgnoreVec().at(i)) {
                dataVec_[counter].push_back(tmpValue
                        * pCsvSettings_->getScalesVec().at(i));
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
    timeVec_.clear();
}