#include "DataProcessor.h"
#include "Node.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <cmath>
#include <stdexcept>

DataProcessor::DataProcessor(const std::string &locFilePath, const std::string &docFilePath)
    : locFilePath(locFilePath), docFilePath(docFilePath) {}

void DataProcessor::processFiles(int maxLines,int maxDimension) {
    processLocFile(maxLines,maxDimension, dataList);
    processDocFile(maxLines,maxDimension, dataList);
    printDataList(dataList);
}
// dataPoint
void DataProcessor::processLocFile(int maxLines,int maxDimension, std::vector<DataPoint> &dataList) {
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
        data.name = std::stod(token);

        // Skip the second value
        std::getline(iss, token, ',');

        // Read the third value as x coordinate
        if (std::getline(iss, token, ',')) {
            try {
                data.coordinate.x = std::floor(std::fabs(std::stod(token) * 100));
            } catch (const std::invalid_argument &e) {
                std::cerr << "Invalid argument for x coordinate: " << token << "\n";
                continue;
            }
        }

        // Read the fourth value as y coordinate
        if (std::getline(iss, token, ',')) {
            try {
                data.coordinate.y = std::floor(std::fabs(std::stod(token) * 100));
            } catch (const std::invalid_argument &e) {
                std::cerr << "Invalid argument for y coordinate: " << token << "\n";
                continue;
            }
        }

        // Initialize array data
        data.vector.resize(maxDimension, 0);

        dataList.push_back(data);
        lineCount++;
    }
    locFile.close();
}

void DataProcessor::processDocFile(int maxLines,int maxDimension, std::vector<DataPoint> &dataList) {
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
                    if (position >= 0 && position < maxDimension) {
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

void DataProcessor::writeDataToFile(const std::string &outputFilePath) {
    std::ofstream outFile(outputFilePath);
    if (!outFile.is_open()) {
        std::cerr << "Failed to open output file: " << outputFilePath << "\n";
        return;
    }

    for (const auto &data : dataList) {
        outFile << data.name << ",{" << data.coordinate.x << "," << data.coordinate.y<<"},{";

        for (int i = 0; i < data.vector.size(); ++i) {
            if(i==0) {
                outFile<<data.vector[i];
            }else {
                outFile << "," << data.vector[i];
            }
        }
        outFile << "},{";
        for (int i = 0; i < data.distance.size(); ++i) {
            if(i==0) {
                outFile<<data.distance[i];
            }else {
                outFile << "," << data.distance[i];
            }
        }
        outFile << "}\n";
    }

    outFile.close();
}