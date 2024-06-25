#include "Node.h"
#include "RTree2.h"
// RTree methods
RTree::RTree(int maxNodeSize) : maxNodeSize(maxNodeSize) {
    root = new RTreeNode(true);
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

void RTree::printTree2() const {
    root->printNode();
    printTree2(root, 0);
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


void RTree::insert(const DataPoint& point) {
    RTreeNode* leaf = chooseLeaf(root, point);
    leaf->data_points.push_back(point);
    leaf->updateMBR2();
    leaf->updateVec();
    leaf->updateMatrix();

    if (leaf->data_points.size() > maxNodeSize) {
        RTreeNode* newNode = splitNode(leaf);
        if (newNode != nullptr && leaf == root) {
            root = newNode;
            adjustTree(root,newNode);
        }else {
            adjustTree(leaf,newNode);
        }
        // newNode->printNode();
    }else {
        adjustTree(leaf,nullptr);
    }
}
RTreeNode* RTree::chooseLeaf(RTreeNode* node, const DataPoint& point) {
    if (node->isLeaf) return node;

    RTreeNode* bestChild = nullptr;
    float bestEnlargement = std::numeric_limits<float>::max();

    for (auto child : node->children) {
        float enlargement = getEnlargement(child, point);
        if (enlargement < bestEnlargement) {
            bestEnlargement = enlargement;
            bestChild = child;
        }
    }
    return chooseLeaf(bestChild, point);
}
int RTree::getEnlargement(RTreeNode* node, const DataPoint& point) {
    float minX = std::min(node->mbr.bottomLeft.x, point.coordinate.x);
    float minY = std::min(node->mbr.bottomLeft.y, point.coordinate.y);
    float maxX = std::max(node->mbr.topRight.x, point.coordinate.x);
    float maxY = std::max(node->mbr.topRight.y, point.coordinate.y);
    return (maxX - minX) * (maxY - minY) - (node->mbr.topRight.x - node->mbr.bottomLeft.x) * (node->mbr.topRight.y - node->mbr.bottomLeft.y);
}
int RTree::getEnlargement(RTreeNode* node, const RTreeNode& point) {
    float minX = std::min(node->mbr.bottomLeft.x, point.mbr.bottomLeft.x);
    float minY = std::min(node->mbr.bottomLeft.y, point.mbr.bottomLeft.y);
    float maxX = std::max(node->mbr.topRight.x, point.mbr.topRight.x);
    float maxY = std::max(node->mbr.topRight.y, point.mbr.topRight.y);
    return (maxX - minX) * (maxY - minY) - (node->mbr.topRight.x - node->mbr.bottomLeft.x) * (node->mbr.topRight.y - node->mbr.bottomLeft.y);
}
RTreeNode* RTree::splitNode(RTreeNode* node) {
    if (node->isLeaf) {
        std::vector<DataPoint> allData = node->data_points;
        node->data_points.clear();
        RTreeNode* newNode1 = new RTreeNode(true);
        RTreeNode* newNode2 = new RTreeNode(true);
        quadraticSplit(allData, newNode1, newNode2);
        if (node == root) {
            RTreeNode* newRoot = new RTreeNode(false);
            newRoot->children.push_back(newNode1);
            newRoot->children.push_back(newNode2);
            newRoot->updateMBR();
            newRoot->updateVec();
            newRoot->updateMatrix();
            return newRoot;
        } else {
            RTreeNode* parent = findParent(root, node);
            parent->children.erase(std::remove(parent->children.begin(), parent->children.end(), node), parent->children.end());
            parent->children.push_back(newNode1);
            parent->children.push_back(newNode2);
            parent->updateMBR();
            parent->updateVec();
            parent->updateMatrix();
            if (parent->children.size() > maxNodeSize) {
                splitNode(parent);
            }
            return nullptr;
        }
    } else {
        std::vector<RTreeNode*> allChildren = node->children;
        node->children.clear();
        RTreeNode* newNode1 = new RTreeNode(false);
        RTreeNode* newNode2 = new RTreeNode(false);
        quadraticSplit(allChildren, newNode1, newNode2);
        if (node == root) {
            RTreeNode* newRoot = new RTreeNode(false);
            newRoot->children.push_back(newNode1);
            newRoot->children.push_back(newNode2);
            newRoot->updateMBR();
            newRoot->updateVec();
            newRoot->updateMatrix();
            return newRoot;
        } else {
            RTreeNode* parent = findParent(root, node);
            parent->children.erase(std::remove(parent->children.begin(), parent->children.end(), node), parent->children.end());
            parent->children.push_back(newNode1);
            parent->children.push_back(newNode2);
            parent->updateMBR();
            parent->updateVec();
            parent->updateMatrix();
            if (parent->children.size() > maxNodeSize) {
                splitNode(parent);
            }
            return nullptr;
        }
    }
}
void RTree::quadraticSplit(std::vector<DataPoint>& data, RTreeNode* group1, RTreeNode* group2) {
        // Initial seeds
        DataPoint* seed1 = nullptr;
        DataPoint* seed2 = nullptr;
        float worstD = -1.0;

        for (size_t i = 0; i < data.size(); ++i) {
            for (size_t j = i + 1; j < data.size(); ++j) {
                float d = (data[i].coordinate.x - data[j].coordinate.x) * (data[i].coordinate.x - data[j].coordinate.x) +
                          (data[i].coordinate.y - data[j].coordinate.y) * (data[i].coordinate.y - data[j].coordinate.y);
                if (d > worstD) {
                    worstD = d;
                    seed1 = &data[i];
                    seed2 = &data[j];
                }
            }
        }

        group1->data_points.push_back(*seed1);
        group2->data_points.push_back(*seed2);

        // Assign remaining points
        for (auto& point : data) {
            if (&point == seed1 || &point == seed2) continue;

            float enlarge1 = getEnlargement(group1, point);
            float enlarge2 = getEnlargement(group2, point);

            if (enlarge1 < enlarge2) {
                group1->data_points.push_back(point);
            } else if (enlarge2 < enlarge1) {
                group2->data_points.push_back(point);
            } else {
                if (group1->data_points.size() < group2->data_points.size()) {
                    group1->data_points.push_back(point);
                } else {
                    group2->data_points.push_back(point);
                }
            }
        }

        group1->updateMBR2();
        group1->updateVec();
        group1->updateMatrix();

        group2->updateMBR2();
        group2->updateVec();
        group2->updateMatrix();
    }
void RTree::quadraticSplit(std::vector<RTreeNode*>& children, RTreeNode* group1, RTreeNode* group2) {
    // Initial seeds
    RTreeNode* seed1 = nullptr;
    RTreeNode* seed2 = nullptr;
    float worstD = -1.0;

    for (size_t i = 0; i < children.size(); ++i) {
        for (size_t j = i + 1; j < children.size(); ++j) {
            float d = (children[i]->mbr.bottomLeft.x - children[j]->mbr.bottomLeft.x) * (children[i]->mbr.bottomLeft.x - children[j]->mbr.bottomLeft.x) +
                         (children[i]->mbr.bottomLeft.y - children[j]->mbr.bottomLeft.y) * (children[i]->mbr.bottomLeft.y - children[j]->mbr.bottomLeft.y) +
                         (children[i]->mbr.topRight.x - children[j]->mbr.topRight.x) * (children[i]->mbr.topRight.x - children[j]->mbr.topRight.x) +
                         (children[i]->mbr.topRight.y - children[j]->mbr.topRight.y) * (children[i]->mbr.topRight.y - children[j]->mbr.topRight.y);
            if (d > worstD) {
                worstD = d;
                seed1 = children[i];
                seed2 = children[j];
            }
        }
    }

    group1->children.push_back(seed1);
    group2->children.push_back(seed2);

    // Assign remaining points
    for (auto& child : children) {
        if (child == seed1 || child == seed2) continue;

        float enlarge1 = getEnlargement(group1, *child);
        float enlarge2 = getEnlargement(group2, *child);

        if (enlarge1 < enlarge2) {
            group1->children.push_back(child);
        } else if (enlarge2 < enlarge1) {
            group2->children.push_back(child);
        } else {
            if (group1->children.size() < group2->children.size()) {
                group1->children.push_back(child);
            } else {
                group2->children.push_back(child);
            }
        }
    }

    group1->updateMBR2();
    group1->updateVec();
    group1->updateMatrix();

    group2->updateMBR2();
    group2->updateVec();
    group2->updateMatrix();
}
void RTree::adjustTree(RTreeNode* node) {
        if (node == root) return;
        RTreeNode* parent = findParent(root, node);
        parent->updateMBR();
        if (parent->children.size() > maxNodeSize) {
            splitNode(parent);
        }
        adjustTree(parent);
    }
void RTree::adjustTree(RTreeNode* node, RTreeNode* newNode) {
    if (node == root) return;
    RTreeNode* parent = findParent(root, node);
    parent->updateMBR();
    if (newNode) {
        parent->children.push_back(newNode);
        if (parent->children.size() > maxNodeSize) {
            RTreeNode* newParentNode = splitNode(parent);
            adjustTree(parent, newParentNode);
        } else {
            adjustTree(parent, nullptr);
        }
    } else {
        adjustTree(parent, nullptr);
    }
}
RTreeNode* RTree::findParent(RTreeNode* current, RTreeNode* target) {
    if (current->isLeaf) return nullptr;
    for (auto child : current->children) {
        if (child == target) return current;
        RTreeNode* result = findParent(child, target);
        if (result) return result;
    }
    return nullptr;
}