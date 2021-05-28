//
// Created by pshbl on 22.05.2021.
//

#ifndef RTS_TANK_H
#define RTS_TANK_H

#include "GameObject.h"
#include "../resource/ResourceManager.h"
#include "../shaders/Shader.h"

class Tank: public GameObject {
private:
    static bool isInit;
public:
    static Shader* modelShader;
    static Shader* selectedShader;
    Tank(float x, float y, int zLevel, float scale, State* state);
};

#endif //RTS_TANK_H
