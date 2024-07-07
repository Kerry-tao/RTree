#include "Node.h"
#include "RTree.h"
// RTree methods
RTree::RTree(int maxNodeSize) : maxNodeSize(maxNodeSize) {
    root = new RTreeNode(true);
}

// void RTree::insert(Point p) {
//     // insert(root, p);
//     // if (root->points.size() > maxNodeSize || root->children.size() > maxNodeSize) {
//     //     RTreeNode* newRoot = new RTreeNode(false);
//     //     newRoot->children.push_back(root);
//     //     splitNode(newRoot, 0);
//     //     root = newRoot;
//     // }
// }

void RTree::insert(DataPoint p) {
    insert(root,p);
    // std::cout<<"inser troot"<<root->data_points.size();

    if (root->data_points.size() > maxNodeSize || root->children.size() > maxNodeSize) {
        RTreeNode* newRoot = new RTreeNode(false);
        newRoot->children.push_back(root);
        splitNode2(newRoot, 0);
        root = newRoot;
    }
}

// void RTree::insert(RTreeNode* node, Point p) {
//     // if (node->isLeaf) {
//     //     node->points.push_back(p);
//     // } else {
//     //     int bestIndex = chooseSubtree(node, p);
//     //     insert(node->children[bestIndex], p);
//     //     if (node->children[bestIndex]->points.size() > maxNodeSize || node->children[bestIndex]->children.size() > maxNodeSize) {
//     //         splitNode(node, bestIndex);
//     //     }
//     // }
//     // node->updateMBR();
// }

void RTree::insert(RTreeNode* node, DataPoint p) {

    if (node->isLeaf) {
        node->data_points.push_back(p);
    } else {
        int bestIndex = chooseSubtree(node, p);
        insert(node->children[bestIndex], p);
        if (node->children[bestIndex]->data_points.size() > maxNodeSize || node->children[bestIndex]->children.size() > maxNodeSize) {
            splitNode2(node, bestIndex);
        }
    }

    node->updateMBR2();
    node->updateVec();
    node->updateMatrix();
}

void RTree::calculateDistances(std::vector<DataPoint> &dataPoints)
{
    for (auto &point : dataPoints)
    {
        point.distance.resize(point.vector.size(), INT_MAX);
        for (size_t i = 0; i < point.vector.size(); ++i)
        {
            if (point.vector[i] == 1)
            {
                point.distance[i] = 0;
            }
            else
            {
                for (const auto &other : dataPoints)
                {
                    if (other.vector[i] == 1)
                    {
                        int manhattanDist = std::abs(point.coordinate.x - other.coordinate.x) +
                                            std::abs(point.coordinate.y - other.coordinate.y);
                        point.distance[i] = std::min(point.distance[i], manhattanDist);
                    }
                }
            }
        }
    }
}

// void RTree::printTree() const {
//     // printTree(root, 0);
// }

void RTree::printTree2() const {
    root->printNode();
    printTree2(root, 0);
}

// int RTree::chooseSubtree(RTreeNode* node, Point p) const {
//     // int bestIndex = -1;
//     // float bestAreaIncrease = std::numeric_limits<float>::max();
//     // Rectangle pointRect(p, p);
//     // for (int i = 0; i < node->children.size(); ++i) {
//     //     Rectangle tempMBR = node->children[i]->mbr;
//     //     tempMBR.bottomLeft.x = std::min(tempMBR.bottomLeft.x, p.x);
//     //     tempMBR.bottomLeft.y = std::min(tempMBR.bottomLeft.y, p.y);
//     //     tempMBR.topRight.x = std::max(tempMBR.topRight.x, p.x);
//     //     tempMBR.topRight.y = std::max(tempMBR.topRight.y, p.y);
//     //     float areaIncrease = tempMBR.area() - node->children[i]->mbr.area();
//     //     if (areaIncrease < bestAreaIncrease) {
//     //         bestAreaIncrease = areaIncrease;
//     //         bestIndex = i;
//     //     }
//     // }
//     // return bestIndex;
// }

int RTree::chooseSubtree(RTreeNode* node, DataPoint p) const {
    int bestIndex = -1;
    float bestAreaIncrease = std::numeric_limits<float>::max();
    Rectangle pointRect(p.coordinate, p.coordinate);
    for (int i = 0; i < node->children.size(); ++i) {
        Rectangle tempMBR = node->children[i]->mbr;
        tempMBR.bottomLeft.x = std::min(tempMBR.bottomLeft.x, p.coordinate.x);
        tempMBR.bottomLeft.y = std::min(tempMBR.bottomLeft.y, p.coordinate.y);
        tempMBR.topRight.x = std::max(tempMBR.topRight.x, p.coordinate.x);
        tempMBR.topRight.y = std::max(tempMBR.topRight.y, p.coordinate.y);
        float areaIncrease = tempMBR.area() - node->children[i]->mbr.area();
        if (areaIncrease < bestAreaIncrease) {
            bestAreaIncrease = areaIncrease;
            bestIndex = i;
        }
    }
    return bestIndex;
}

// void RTree::splitNode(RTreeNode* parent, int index) {
//     // RTreeNode* node = parent->children[index];
//     // RTreeNode* newNode = new RTreeNode(node->isLeaf);
//     //
//     // // simple split logic: distribute half elements to new node
//     // int halfSize = node->points.size() / 2;
//     // newNode->points.assign(node->points.begin() + halfSize, node->points.end());
//     // node->points.erase(node->points.begin() + halfSize, node->points.end());
//     //
//     // if (!node->isLeaf) {
//     //     halfSize = node->children.size() / 2;
//     //     newNode->children.assign(node->children.begin() + halfSize, node->children.end());
//     //     node->children.erase(node->children.begin() + halfSize, node->children.end());
//     // }
//     //
//     // parent->children.push_back(newNode);
//     // parent->updateMBR();
//     // node->updateMBR();
//     // newNode->updateMBR();
// }

void RTree::splitNode2(RTreeNode* parent, int index) {

    // parent->printNode();

    RTreeNode* node = parent->children[index];
    RTreeNode* newNode = new RTreeNode(node->isLeaf);

    // simple split logic: distribute half elements to new node
    int halfSize = node->data_points.size() / 2;
    newNode->data_points.assign(node->data_points.begin() + halfSize, node->data_points.end());
    node->data_points.erase(node->data_points.begin() + halfSize, node->data_points.end());

    if (!node->isLeaf) {
        halfSize = node->children.size() / 2;
        newNode->children.assign(node->children.begin() + halfSize, node->children.end());
        node->children.erase(node->children.begin() + halfSize, node->children.end());
    }

    parent->children.push_back(newNode);
    parent->updateMBR2();
    parent->updateVec();
    parent->updateMatrix();

    node->updateMBR2();
    node->updateVec();
    node->updateMatrix();

    newNode->updateMBR2();
    newNode->updateVec();
    newNode->updateMatrix();
}

void RTree::printTree2(RTreeNode* node, int depth) const {
    for (int i = 0; i < depth; ++i) std::cout << "  ";
    // std::cout << "Node(" << (node->isLeaf ? "Leaf" : "Internal") << "): ";
    if (node->isLeaf) {
        std::cout<<std::endl;
        for (const auto& p : node->data_points) {
            p.print();
        }

    } else {
        // node->printNode();
        for (const auto& child : node->children) {
            child->printNode();
        }
    }
    std::cout << std::endl;
    if (!node->isLeaf) {
        for (const auto& child : node->children) {
            printTree2(child, depth + 1);
        }
    }
}
