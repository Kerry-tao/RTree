#include "DataProcessor.h"
#include "Node.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <cmath>
#include <stdexcept>

DataProcessor::DataProcessor(const std::string &locFilePath, const std::string &docFilePath)
    : locFilePath(locFilePath), docFilePath(docFilePath) {}

void DataProcessor::processFiles(int maxLines) {
    processLocFile(maxLines, dataList);
    processDocFile(maxLines, dataList);
    printDataList(dataList);
}
// dataPoint
void DataProcessor::processLocFile(int maxLines, std::vector<DataPoint> &dataList) {
    std::ifstream locFile(locFilePath);
    if (!locFile.is_open()) {
        std::cerr << "Failed to open loc file: " << locFilePath << "\n";
        return;
    }

    std::string line;
    int lineCount = 0;
    while (std::getline(locFile, line) && lineCount < maxLines) {
        std::istringstream iss(line);
        std::string token;
        DataPoint data;
        // Data data;

        // Read the first value as string data
        std::getline(iss, token, ',');
        data.name = token;

        // Skip the second value
        std::getline(iss, token, ',');

        // Read the third value as x coordinate
        if (std::getline(iss, token, ',')) {
            try {
                data.coordinate.x = std::floor(std::fabs(std::stod(token) * 1000));
            } catch (const std::invalid_argument &e) {
                std::cerr << "Invalid argument for x coordinate: " << token << "\n";
                continue;
            }
        }

        // Read the fourth value as y coordinate
        if (std::getline(iss, token, ',')) {
            try {
                data.coordinate.y = std::floor(std::fabs(std::stod(token) * 1000));
            } catch (const std::invalid_argument &e) {
                std::cerr << "Invalid argument for y coordinate: " << token << "\n";
                continue;
            }
        }

        // Initialize array data
        data.vector.resize(2468, 0);

        dataList.push_back(data);
        lineCount++;
    }
    locFile.close();
}

void DataProcessor::processDocFile(int maxLines, std::vector<DataPoint> &dataList) {
    std::ifstream docFile(docFilePath);
    if (!docFile.is_open()) {
        std::cerr << "Failed to open doc file: " << docFilePath << "\n";
        return;
    }

    std::string line;
    int lineCount = 0;

    while (std::getline(docFile, line) && lineCount < maxLines) {
        std::istringstream iss(line);
        std::string token;

        // Read the corresponding data from dataList
        if (lineCount < dataList.size()) {
            // Data &data = dataList[lineCount];

            DataPoint &data = dataList[lineCount];
            //跳过第一个数据
            std::getline(iss, token, ',');
            // Read the values as positions in the array
            while (std::getline(iss, token, ',')) {
                try {
                    int position = std::stoi(token);
                    if (position >= 0 && position < 2468) {
                        //第n个位置赋值为1
                        data.vector[position-1] = 1;
                    }
                } catch (const std::invalid_argument &e) {
                    std::cerr << "Invalid argument for array position: " << token << "\n";
                }
            }
        }

        lineCount++;
    }
    docFile.close();
}

void DataProcessor::printDataList(const std::vector<DataPoint> &dataList) {
    for (const auto &data : dataList) {
        std::cout << "String Data: " << data.name << ", x: " << data.coordinate.x << ", y: " << data.coordinate.y << "\n";
        // std::cout << "Array Data: ";
        // for (int val : data.vector) {
        //     std::cout << val << " ";
        // }
        std::cout << "\n";
    }
}