//
// Created by pshbl on 11.06.2021.
//

#ifndef RTS_LAMP_H
#define RTS_LAMP_H


#include "../base/StaticObject.h"
#include "../../../light/PointLight.h"

class Lamp: public StaticObject {
public:

    PointLight* pointLight;

    Lamp(float x, float y, State *state, float angle);

    static Shader* modelShader;

    const glm::vec3 getPositionLight();
};


#endif //RTS_LAMP_H
