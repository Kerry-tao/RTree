//
// Created by 28927 on 2024/7/10.
//
#ifndef ENCNODE_H
#define ENCNODE_H

#include <string>

#include "Node.h"

class EncNode:public RTreeNode{
public:
    std::string encryptedData;
    EncNode(bool isLeaf = true);
    void encryptNode();
    void decryptNode();
};



#endif //ENCNODE_H
