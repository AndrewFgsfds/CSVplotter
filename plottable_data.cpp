#include "plottable_data.h"
#include <cmath>


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
    bool isFirstRow = true;
    QFile inFile{fileName};
    if (false == inFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        return false;
    }
    QTextStream fstream{&inFile};
    while (!fstream.atEnd()) {
        QStringList stringlist{fstream.readLine().split(';')};
        //stringlist.pop_back(); // избавляюсь от первода строки
        int notIgnoreCntr{0};
        auto strIt{stringlist.cbegin()};
        for(int column = 0; column < pCsvSettings_->getNumOfCsvColumns(); ++column) {
            double tmpValue{};
            if (strIt != stringlist.cend()) { //заполняю нулями отсуствующие значения
                tmpValue = strIt->toDouble();
                ++strIt;
            } else {
                tmpValue = 0.0;
            }

            if (!pCsvSettings_->isRowIgnored(column)) {
                if (pCsvSettings_->isRowBitset(column)) {
                    QStringList bitNames{pCsvSettings_->getRowMU(column).split(' ')};
                    for(int i = 0; i < bitNames.size(); ++i){
                        if (isFirstRow) {
                            plotableNames_.push_back(bitNames.value(i));
                            plotableMu_.push_back("bit");
                            dataVec_.push_back({});
                        }
                        dataVec_[notIgnoreCntr].push_back((std::lround(tmpValue) >> i) & 1);
                        ++notIgnoreCntr;
                    }
                } else {
                    if (isFirstRow) {
                        plotableNames_.push_back(pCsvSettings_->getRowName(column));
                        plotableMu_.push_back(pCsvSettings_->getRowMU(column));
                        dataVec_.push_back({});
                    }
                    dataVec_[notIgnoreCntr].push_back(tmpValue
                            * pCsvSettings_->getRowScale(column));
                    ++notIgnoreCntr;
                }
            }
        }
        isFirstRow = false;
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
