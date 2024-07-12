//
// Created by 28927 on 2024/7/10.
//

#ifndef DATAOUTSOURCING_H
#define DATAOUTSOURCING_H

#include "EncRTree.h"
#include "Node.h"
#include "RTree.h"


// DataPoint readDataPointFromLine(const std::string &line);//一次读取一条数据
DataPoint parseDataPoint(const std::string& line);
std::vector<DataPoint> readDataPointsFromFile(const std::string& filename);



static EncRTree* encryptRTree(RTree* root); //加密树
static EncNode* encryptRec(RTreeNode* rNode); //加密节点
static DataPoint* encryptDP(DataPoint* dp);
// DataPoint readDataPointFromLine(const std::wstring &wline);
class DataOutsourcing {
public:

};

#endif //DATAOUTSOURCING_H
