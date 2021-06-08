//
// Created by pshbl on 08.06.2021.
//

#include "Tree.h"
#include "../../resource/ResourceManager.h"

Model* getTree(TreeType treeType) {
    std::string tree_name;
    switch (treeType) {
        case TREE1:
            tree_name = "trees\\tree1\\tree.fbx";
            break;
        case TREE2:
            tree_name = "trees\\tree2\\tree.fbx";
            break;
        case TREE3:
            tree_name = "trees\\tree3\\tree.fbx";
            break;
    }
    return ResourceManager::loadModel(tree_name);
}

float getScale(TreeType treeType) {
    switch (treeType) {
        case TREE1:
            return 0.005;
        case TREE2:
            return 0.001;
        case TREE3:
            return 0.005;
    }
}


Tree::Tree(float x, float y, State *state, TreeType treeType)
: StaticObject(getTree(treeType), x, y, 0, getScale(treeType), state, -0.5f) {
    addCollider(new Collider(0.0f, 0.0f, 0.5f, state));
    if (modelShader == nullptr) {
        modelShader = ResourceManager::loadShader("tree");
    }
    angleX = -AI_MATH_HALF_PI_F;
}

Shader* Tree::modelShader = nullptr;
