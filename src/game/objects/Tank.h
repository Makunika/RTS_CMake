//
// Created by pshbl on 22.05.2021.
//

#ifndef RTS_TANK_H
#define RTS_TANK_H

#include "GameObject.h"
#include "../../shaders/Shader.h"
#include "../../light/SpotLight.h"

class Tank: public GameObject {
private:
    static bool isInit;
public:
    static Shader* modelShader;
    static Shader* selectedShader;
    SpotLight* lightLeft;
    Tank(float x, float y, int zLevel, State* state);

    glm::vec3 getPositionLightLeft();

    void update() override;
};

#endif //RTS_TANK_H
