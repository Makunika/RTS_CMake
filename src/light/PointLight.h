//
// Created by pshbl on 28.05.2021.
//

#ifndef RTS_POINTLIGHT_H
#define RTS_POINTLIGHT_H


#include "Light.h"

class PointLight : public Light {
    glm::vec3 position;

    float constant;
    float linear;
    float quadratic;

    int index;

    static int counter;

public:
    PointLight(const glm::vec3 ambient, const glm::vec3 diffuse, const glm::vec3 specular, const glm::vec3 position,
               float constant, float linear, float quadratic);

    void use(Shader *shader) override;

    void setPosition(const glm::vec3 position);

    static int getCount();
};


#endif //RTS_POINTLIGHT_H
