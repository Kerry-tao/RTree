#ifndef DATA_PROCESSOR_H
#define DATA_PROCESSOR_H

#include <string>
#include <vector>

#include "Node.h"

class DataProcessor {
public:
    std::vector<DataPoint> dataList;
    DataProcessor(const std::string &locFilePath, const std::string &docFilePath);
    void processFiles(int maxLines);

private:
    std::string locFilePath;
    std::string docFilePath;
    void processLocFile(int maxLines, std::vector<DataPoint> &dataList);
    void processDocFile(int maxLines, std::vector<DataPoint> &dataList);
    void printDataList(const std::vector<DataPoint> &dataList);
};

#endif // DATA_PROCESSOR_H
