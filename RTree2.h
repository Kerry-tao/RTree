#ifndef RTREE2_H
#define RTREE2_H
#include "Node.h"
#include <vector>
#include <algorithm>
#include <limits>
#include <iostream>
class RTree {
public:
    RTree(int maxNodeSize = 3);
    void insert(const DataPoint& point);
    void printTree2() const;
    void calculateDistances(std::vector<DataPoint>& dataPoints);
private:
    RTreeNode* root;
    int maxNodeSize;

    void printTree2(RTreeNode* node, int depth) const;
    RTreeNode* chooseLeaf(RTreeNode* node, const DataPoint& point);
    int getEnlargement(RTreeNode* node, const DataPoint& point);
    int getEnlargement(RTreeNode* node, const RTreeNode& point);
    RTreeNode* splitNode(RTreeNode* node);
    void quadraticSplit(std::vector<DataPoint>& data, RTreeNode* group1, RTreeNode* group2);
    void quadraticSplit(std::vector<RTreeNode*>& children, RTreeNode* group1, RTreeNode* group2);
    void adjustTree(RTreeNode* node);
    void adjustTree(RTreeNode* node, RTreeNode* newNode);
    RTreeNode* findParent(RTreeNode* current, RTreeNode* target);
};

#endif // RTREE_H
