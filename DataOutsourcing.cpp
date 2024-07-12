//
// Created by 28927 on 2024/7/10.
//

#include "DataOutsourcing.h"
#include <fstream>
#include <sstream>
#include <regex>
#include <vector>
#include <locale>
#include <codecvt>

// 解析字符串并将其转换为 DataPoint 对象
DataPoint parseDataPoint(const std::string& line) {
    DataPoint dp;
    std::stringstream ss(line);
    std::string item;

    // 解析 name
    std::getline(ss, item, ',');
    dp.name = std::stoull(item);

    // 解析 coordinate
    std::getline(ss, item, '}');
    item = item.substr(1); // 去掉前面的 '{'
    std::stringstream coordStream(item);
    std::getline(coordStream, item, ',');
    dp.coordinate.x = std::stoull(item);
    std::getline(coordStream, item, ',');
    dp.coordinate.y = std::stoull(item);
    //
    // // 解析 vector
    std::getline(ss, item, '}');
    item = item.substr(2); // 去掉前面的 ',{'
    std::stringstream vectorStream(item);
    while (std::getline(vectorStream, item, ',')) {
        dp.vector.push_back(std::stoull(item));
    }

    // 解析 distance
    std::getline(ss, item, '}');
    item = item.substr(2); // 去掉前面的 ',{'
    std::stringstream distanceStream(item);
    while (std::getline(distanceStream, item, ',')) {
        dp.distance.push_back(std::stoull(item));
    }
    return dp;
}

// 从文件中读取数据并转换为 DataPoint 对象
std::vector<DataPoint> readDataPointsFromFile(const std::string& filename) {
    std::vector<DataPoint> dataPoints;
    std::ifstream file(filename);
    std::string line;

    while (std::getline(file, line)) {
        // std::cout<<line<<std::endl;
        dataPoints.push_back(parseDataPoint(line));
    }
    file.close();
    return dataPoints;
}

static EncRTree* encryptRTree(RTree* tree){
    EncRTree* encTree = new EncRTree();
    encTree->root = encryptRec(tree->root);
    return encTree;
}
static EncNode* encryptRec(RTreeNode* rNode) {
    EncNode* enc_node = new EncNode();
    if(rNode==nullptr) {
        return nullptr;
    }
    //叶子节点，遍历叶子节点的所有数据点，并加密数据点
    if(rNode->isLeaf) {
        for (const auto& dp:rNode->data_points) {
            DataPoint* enc_data_points = encryptDP(dp);
            enc_node->data_points.push_back(enc_data_points);
        }
    }else { //非叶子节点，遍历所有子节点，加密子节点
        for (const auto& node:rNode->children) {
            RTreeNode* enc_node = encryptRec(node);
            enc_node->children.push_back(enc_node);
        }
    }
    //加密向量
    //加密当前节点的数据
    // bool isLeaf;  无需加密
    // std::vector<DataPoint> data_points; //for leaf nodes
    // std::vector<RTreeNode*> children; // for internal nodes
    // Rectangle mbr; // minimum bounding rectangle,for all nodes
    // std::vector<uint64_t> vector; //for internal nodes
    // std::vector<std::vector<uint64_t>> matrix; //for internal nodes

    enc_node->isLeaf = rNode->isLeaf;
    return enc_node;
}
static DataPoint* encryptDP(DataPoint* dp) {
    DataPoint* newdp = new DataPoint();
    // 一次加密
    // uint64_t name;
    // Point coordinate;
    // std::vector<uint64_t> vector;
    // std::vector<uint64_t> distance;
    return newdp;
}
static void encryptValue(uint64_t value) {

}
