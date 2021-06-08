//
// Created by pshbl on 28.05.2021.
//

#ifndef RTS_GAMESTATE_H
#define RTS_GAMESTATE_H

#include "State.h"
#include "../game/objects/Tank.h"
#include "../window/MousePicker.h"
#include "../game/objects/Tree.h"

struct GameState {
    vector<GameObject*> gameObjects;
    vector<Tank*> tanks;
    vector<Tank*> selectedTanks;
    vector<Tree*> trees;
    State* state;
    MousePicker* mousePicker;

    void addTank(Tank* tank) {
        gameObjects.push_back(tank);
        tanks.push_back(tank);
    }

    void addTree(Tree* tree) {
        trees.push_back(tree);
    }
};

#endif //RTS_GAMESTATE_H
