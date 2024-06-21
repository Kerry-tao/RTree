#ifndef NODE_H
#define NODE_H
#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <cfloat>

struct Point {
    float x, y;
    Point(float x = 0, float y = 0);
};

class DataPoint
{
public:
    Point coordinate;
    std::vector<int> vector;
    std::vector<int> distance;
    DataPoint(const Point &point, const std::vector<int> &vector) : coordinate(point), vector(vector) {};
    void print() const;
};

struct Rectangle {
    Point topLeft, bottomRight;
    Rectangle(Point topLeft = Point(), Point bottomRight = Point());

    bool contains(Point p) const;
    bool intersects(Rectangle other) const;
    float area() const;
};

class  RTreeNode {
public:
    bool isLeaf;
    std::vector<Point> points; // for leaf nodes
    std::vector<DataPoint> data_points;

    std::vector<RTreeNode*> children; // for internal nodes

    Rectangle mbr; // minimum bounding rectangle
    std::vector<int> vector;
    std::vector<std::vector<int>> matrix;

    RTreeNode(bool isLeaf = true);
    void updateMBR();
    void updateMBR2();
    void updateVec();
    void updateMatrix();
    void printNode();
};
#endif //NODE_H
