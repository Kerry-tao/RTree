#ifndef RTREE_H
#define RTREE_H
#include "Node.h"
#include <vector>
#include <algorithm>
#include <limits>
#include <iostream>
class RTree {
public:
    RTree(int maxNodeSize = 4);

    void insert(Point p);
    void insert(DataPoint p);


    void printTree() const;
    void printTree2() const;


    void calculateDistances(std::vector<DataPoint>& dataPoints);

private:
    RTreeNode* root;
    int maxNodeSize;

    void insert(RTreeNode* node, Point p);
    void insert(RTreeNode* node,DataPoint p);


    int chooseSubtree(RTreeNode* node, Point p) const;
    int chooseSubtree(RTreeNode* node, DataPoint p) const;


    void splitNode(RTreeNode* parent, int index);
    void splitNode2(RTreeNode* parent, int index);

    void printTree(RTreeNode* node, int depth) const;
    void printTree2(RTreeNode* node, int depth) const;
};

#endif // RTREE_H
