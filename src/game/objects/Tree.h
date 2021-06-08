//
// Created by pshbl on 08.06.2021.
//

#ifndef RTS_TREE_H
#define RTS_TREE_H


#include "StaticObject.h"

enum TreeType {
    TREE1,
    TREE2,
    TREE3
};



class Tree: public StaticObject {

public:
    Tree(float x, float y, State *state, TreeType treeType);
    static Shader* modelShader;
};


#endif //RTS_TREE_H
