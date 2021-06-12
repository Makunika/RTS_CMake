//
// Created by pshbl on 28.05.2021.
//

#ifndef RTS_GAMESTATE_H
#define RTS_GAMESTATE_H

#include "State.h"
#include "../game/objects/gameobjects/Tank.h"
#include "../window/MousePicker.h"
#include "../game/objects/static/Tree.h"
#include "../game/objects/static/Home.h"
#include "../game/objects/static/Lamp.h"

struct GameState {
    //Все игровые объекты
    vector<GameObject*> gameObjects;
    //Все танки
    vector<Tank*> tanks;
    //Выбранные танки
    vector<Tank*> selectedTanks;
    //Все деревья
    vector<Tree*> trees;
    //Все дома
    vector<Home*> homes;
    //Все лампы
    vector<Lamp*> lamps;
    //Все статические объекты (деревья, дома, лампы)
    vector<StaticObject*> staticObjects;

    //Основные состояние
    State* state;

    //пикер
    MousePicker* mousePicker;

    void addTank(Tank* tank) {
        gameObjects.push_back(tank);
        tanks.push_back(tank);
    }

    void addTree(Tree* tree) {
        addTree(tree, false);
    }

    void addTree(Tree* tree, bool collide) {
        trees.push_back(tree);
        if (collide) {
            staticObjects.push_back(tree);
        }
    }

    void addHome(Home* home) {
        homes.push_back(home);
        staticObjects.push_back(home);
    }

    void addLamp(Lamp* lamp) {
        lamps.push_back(lamp);
    }
};

#endif //RTS_GAMESTATE_H
