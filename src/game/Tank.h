//
// Created by pshbl on 22.05.2021.
//

#ifndef RTS_TANK_H
#define RTS_TANK_H

#include "GameObject.h"

class Tank: public GameObject {
private:
    static bool isInit;
public:
    Tank(float x, float y, int zLevel, float scale, State* state);
};

#endif //RTS_TANK_H
