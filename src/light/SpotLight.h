//
// Created by pshbl on 28.05.2021.
//

#ifndef RTS_SPOTLIGHT_H
#define RTS_SPOTLIGHT_H


#include "Light.h"

class SpotLight : public Light {

    glm::vec3 position;
    glm::vec3 direction;
    float cutOff;
    float outerCutOff;

    float constant;
    float linear;
    float quadratic;

    int index;

    static int counter;
public:
    SpotLight(const glm::vec3 ambient, const glm::vec3 diffuse, const glm::vec3 specular, const glm::vec3 position,
              const glm::vec3 direction, float cutOff, float outerCutOff, float constant, float linear,
              float quadratic);

    void use(Shader *shader) override;

    void setPosition(const glm::vec3 position);

    void setDirection(const glm::vec3 direction);

    static int getCount();

};


#endif //RTS_SPOTLIGHT_H
