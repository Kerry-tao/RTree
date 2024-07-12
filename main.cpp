#include "Node.h"
#include "RTree.h"
#include "DataOutsourcing.h"
// #include "RTree2.h"
#include "DataProcessor.h"
#include <iostream>
#include <fstream>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cstdint>
#include <locale>
#include <codecvt>

#include "EncRTree.h"
using namespace std;

int main() {

    // std::string locFilePath = "C:/Users/28927/Documents/RTree/yelp-loc.txt";
    // std::string docFilePath = "C:/Users/28927/Documents/RTree/yelp-doc.txt";
    // std::string outputFilePath = "C:/Users/28927/Documents/RTree/dataPoint.txt";
    // //
    // int maxLines = 10 ;  // Control the number of lines to read
    // const int maxDimension = 4;
    // DataProcessor processor(locFilePath, docFilePath);
    // processor.processFiles(maxLines,maxDimension);
    //
    // RTree rtree;
    // rtree.calculateDistances(processor.dataList);
    // processor.writeDataToFile(outputFilePath);
    //
    //
    // //构建R树
    // for(const DataPoint& data_point :processor.dataList) {
    //     rtree.insert(DataPoint(data_point));
    // }
    // rtree.printTree2();

    //加密R树
    // EncRTree enc_rtree;
    // enc_rtree = DataOutsourcing::encryptRTree(rtree.root);


    //测试读取文件
    // RTree rtree;
    // std::string filename = "C:/Users/28927/Documents/RTree/dataPoint.txt"; // 替换为你的文件名
    // std::ifstream file(filename);
    // std::string line;
    // while (std::getline(file, line)) {
    //     rtree.insert(parseDataPoint(line));
    // }
    // file.close();
    // rtree.printTree2();
    // return 0;


    // if(rtree.root) {
    //     rtree.traverseAndEncrpt(rtree.root);
    // }
    // // rtree.printTree2();
    // return 0;
    // cout<<"---------\n";
    RTree rtree2;
    std::vector<DataPoint> data_points = {
        {1,{4,1},{1, 0, 0, 1}},
        {2,{5,2},{0, 1, 0, 1}},
        {3,{4,5},{0, 0, 1, 0}},
        {4,{9,10},{1, 0, 0, 0}},
        {5,{10,9},{0, 1, 1, 0}},
        {5,{9,9},{0, 1, 0, 0}},
        {7,{2,4},{1, 0, 0, 1}},
        {8,{8,4},{0, 1, 1, 0}},
        {9,{4,7},{0, 0, 1, 1}}
    };
    rtree2.calculateDistances(data_points);
    for(const DataPoint& data_point :data_points) {
        rtree2.insert(DataPoint(data_point));
    }
    rtree2.printTree2();
}
