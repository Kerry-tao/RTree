//
// Created by 28927 on 2024/7/10.
//

#ifndef ENCRTREE_H
#define ENCRTREE_H


#include <vector>
#include "EncNode.h"

class EncRTree {
public:
    EncNode* root;
    EncRTree():root(nullptr){};
    void search(const Rectangle &rect, std::vector<EncNode> &results);
};

#endif //ENCRTREE_H
