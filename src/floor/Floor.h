//
// Created by pshbl on 01.06.2021.
//

#ifndef RTS_FLOOR_H
#define RTS_FLOOR_H


#include "../shaders/ShaderUtils.h"
#include "../shaders/Shader.h"
#include "../state/State.h"
#include "../state/LightState.h"

class Floor {
    VAO_VBO vFloorLines;
    VAO_VBO vFloorGlass;
    unsigned int ibo;
    int length;
    Shader* shaderGlass;
    Shader* shaderLines;
    State* state;
    Texture* diff;
    Texture* spec;

public:
    Floor(State* state);
    void draw(LightState* lightState);
};


#endif //RTS_FLOOR_H
