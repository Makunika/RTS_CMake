//
// Created by pshbl on 08.06.2021.
//

#include "Tree.h"
#include "../../../resource/ResourceManager.h"

Model* getTree(TreeType treeType) {
    std::string tree_name;
    switch (treeType) {
        case TREE1:
            tree_name = "trees\\tree2\\tree.fbx";
            break;
        case TREE2:
            tree_name = "trees\\tree2\\tree.fbx";
            break;
        case TREE3:
            tree_name = "trees\\tree3\\tree.fbx";
            break;
        case TREE4:
            tree_name = "trees\\tree4\\tree.fbx";
            break;
    }
    return ResourceManager::loadModel(tree_name);
}

float getRandom(float num, float offset) {
    double f = (double)rand() / RAND_MAX;
    float off = -offset + f * (offset + offset);
    return num + off;
}

float getScale(TreeType treeType) {
    switch (treeType) {
        case TREE1:
            return getRandom(0.008, 0.002);
        case TREE2:
            return getRandom(0.008, 0.002);
        case TREE3:
            return getRandom(0.008, 0.002);
        case TREE4:
            return getRandom(0.008, 0.002);
    }
}

TreeType getRandomTreeType() {
    int i = rand() % 4;
    return static_cast<TreeType>(i);
}


Tree::Tree(float x, float y, State *state, TreeType treeType)
: StaticObject(getTree(treeType), getRandom(x, 1.0f), getRandom(y, 0.2), 0, getScale(treeType), state) {
    addCollider(new Collider(0.0f, 0.0f, 0.5f, state));
    if (modelShader == nullptr) {
        modelShader = ResourceManager::loadShader("tree");
    }
    angleX = -AI_MATH_HALF_PI_F;
    angle = (-314 + rand() % 614) / 100.0f;
}

Shader* Tree::modelShader = nullptr;

Tree::Tree(float x, float y, State *state) : Tree(x, y, state, getRandomTreeType()) {
}
