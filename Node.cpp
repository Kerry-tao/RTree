#include "Node.h"
#include <iostream>
using namespace std;
// Point methods
Point::Point(float x, float y) : x(x), y(y) {}

//DataInfo

void DataPoint::print() const
{
    cout<<"Point:("<<coordinate.x<<","<<coordinate.y<<")";
    cout<<"vector:(";
    for (int elem : vector){
        cout << elem << ",";
    }
    cout << "),distance:(";
    for (int elem: distance) {
        cout << elem << ",";
    }
    cout<<")"<< std::endl;
}

// Rectangle methods
Rectangle::Rectangle(Point topLeft, Point bottomRight) : topLeft(topLeft), bottomRight(bottomRight) {}

bool Rectangle::contains(Point p) const {
    return (p.x >= topLeft.x && p.x <= bottomRight.x && p.y >= topLeft.y && p.y <= bottomRight.y);
}

bool Rectangle::intersects(Rectangle other) const {
    return !(topLeft.x > other.bottomRight.x || bottomRight.x < other.topLeft.x ||
             topLeft.y > other.bottomRight.y || bottomRight.y < other.topLeft.y);
}

float Rectangle::area() const {
    return (bottomRight.x - topLeft.x) * (bottomRight.y - topLeft.y);
}

// RTreeNode methods
RTreeNode::RTreeNode(bool isLeaf) : isLeaf(isLeaf) {
    matrix.assign(4, std::vector<int>(4, 0));
}

void RTreeNode::updateMBR() {
    if (isLeaf) {
        if (points.empty()) return;
        mbr = Rectangle(points[0], points[0]);
        for (const auto& p : points) {
            mbr.topLeft.x = std::min(mbr.topLeft.x, p.x);
            mbr.topLeft.y = std::min(mbr.topLeft.y, p.y);
            mbr.bottomRight.x = std::max(mbr.bottomRight.x, p.x);
            mbr.bottomRight.y = std::max(mbr.bottomRight.y, p.y);
        }
    } else {
        if (children.empty()) return;
        mbr = children[0]->mbr;
        for (const auto& child : children) {
            mbr.topLeft.x = std::min(mbr.topLeft.x, child->mbr.topLeft.x);
            mbr.topLeft.y = std::min(mbr.topLeft.y, child->mbr.topLeft.y);
            mbr.bottomRight.x = std::max(mbr.bottomRight.x, child->mbr.bottomRight.x);
            mbr.bottomRight.y = std::max(mbr.bottomRight.y, child->mbr.bottomRight.y);
        }
    }
}
void RTreeNode::updateMBR2() {
    if (isLeaf) {
        if (data_points.empty()) return;
        mbr = Rectangle(data_points[0].coordinate, data_points[0].coordinate);
        for (const auto& p : data_points) {
            mbr.topLeft.x = std::min(mbr.topLeft.x, p.coordinate.x);
            mbr.topLeft.y = std::min(mbr.topLeft.y, p.coordinate.y);
            mbr.bottomRight.x = std::max(mbr.bottomRight.x, p.coordinate.x);
            mbr.bottomRight.y = std::max(mbr.bottomRight.y, p.coordinate.y);
        }
    } else {
        if (children.empty()) return;
        mbr = children[0]->mbr;
        for (const auto& child : children) {
            mbr.topLeft.x = std::min(mbr.topLeft.x, child->mbr.topLeft.x);
            mbr.topLeft.y = std::min(mbr.topLeft.y, child->mbr.topLeft.y);
            mbr.bottomRight.x = std::max(mbr.bottomRight.x, child->mbr.bottomRight.x);
            mbr.bottomRight.y = std::max(mbr.bottomRight.y, child->mbr.bottomRight.y);
        }
    }
}

void RTreeNode::updateVec() {
    if (isLeaf) {
        if (data_points.empty()) return;
        vector ={0,0,0,0};
        for (const auto& p : data_points) {
            for(int i=0;i<p.vector.size();i++) {
                vector[i]|=p.vector[i];
            }
        }
    } else {
        if (children.empty()) return;
        vector = children[0]->vector;
        for (const auto& child : children) {
            for(int i=0;i<child->vector.size();i++) {
                vector[i]|=child->vector[i];
            }
        }
    }
}

void RTreeNode::updateMatrix() {
    if (isLeaf) {
        if (data_points.empty()) return;
            for (int j = 0; j < 4; ++j) {
                bool allZero = true;
                for (const auto& obj : data_points) {
                    if (obj.vector[j] != 0) {
                        allZero = false;
                        break;
                    }
                }
                if(allZero) {
                    for (int i = 0; i < 4; ++i) {
                        matrix[j][i] = 0;
                    }
                }
                if (!allZero) {
                    for (int i = 0; i < 4; ++i) {
                        int minDistance = numeric_limits<int>::max();
                        for (const auto& obj : data_points) {
                            if (obj.vector[j] == 1) {
                                minDistance = min(minDistance, obj.distance[i]);
                            }
                        }
                        matrix[j][i] = minDistance;
                    }
                }
            }
    } else {
        if (children.empty()) return;
        matrix = children[0]->matrix;
        for (int j = 0; j < 4; ++j) {
            for (int i = 0; i < 4; ++i) {
                int minDistance = 0;
                for (const auto& obj : children) {
                     minDistance = min(matrix[i][j], obj->matrix[i][j]);
                }
                matrix[i][j] = minDistance;
            }
        }
    }
}

void RTreeNode::printNode() {
    std::cout << "Node(" << (isLeaf ? "Leaf" : "Internal") << "): ";
    std::cout << "[" << mbr.topLeft.x << "," << mbr.topLeft.y << "] - ";
    std::cout << "[" << mbr.bottomRight.x << "," << mbr.bottomRight.y << "] (";
    for (int elem : vector){
        std::cout << elem << ",";
    }
    std::cout<<")"<<endl;
    for (const auto& row : matrix) {
        for (int val : row) {
            cout << val << " ";
        }
        cout << endl;
    }
}
