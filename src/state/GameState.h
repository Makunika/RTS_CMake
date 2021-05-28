//
// Created by pshbl on 28.05.2021.
//

#ifndef RTS_GAMESTATE_H
#define RTS_GAMESTATE_H

#include "State.h"
#include "../game/Tank.h"
#include "../window/MousePicker.h"

struct GameState {
    vector<GameObject*> gameObjects;
    vector<Tank*> tanks;
    vector<Tank*> selectedTanks;
    State* state;
    MousePicker* mousePicker;

    void addTank(Tank* tank) {
        gameObjects.push_back(tank);
        tanks.push_back(tank);
    }
};

#endif //RTS_GAMESTATE_H
