#ifndef NODE_H
#define NODE_H
#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <cfloat>
#include <cstdint>

static int maxDimension = 4;
//坐标点
struct Point {
    float x, y;
    Point(int x = 0, int y = 0);
};

//数据点
class DataPoint
{
public:
    int name;
    Point coordinate;
    std::vector<int> vector;
    std::vector<int> distance;
    
    DataPoint():name(0),coordinate((0,0)){};
    DataPoint(const int name,const Point &point, const std::vector<int> &vector) :name(name), coordinate(point), vector(vector) {};
    DataPoint(const int name, const Point &point, const std::vector<int> &vector, const std::vector<int> &distance)
        : name(name), coordinate(point), vector(vector), distance(distance) {};
    void print() const;
};

struct Rectangle {
    Point bottomLeft, topRight;
    Rectangle(Point bottomLeft = Point(), Point topRight = Point());

    bool contains(Point p) const;
    bool intersects(Rectangle other) const;
    float area() const;
};

class  RTreeNode {
public:
    bool isLeaf;
    std::vector<DataPoint*> data_points; //for leaf nodes
    std::vector<RTreeNode*> children; // for internal nodes
    Rectangle mbr; // minimum bounding rectangle,for all nodes
    std::vector<int> vector; //for internal nodes
    std::vector<std::vector<int>> matrix; //for internal nodes

    RTreeNode(bool isLeaf = true);
    void updateMBR();
    void updateMBR2();
    void updateVec();
    void updateMatrix();
    void printNode();
};
#endif //NODE_H
