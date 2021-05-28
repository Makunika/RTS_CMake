//
// Created by pshbl on 28.05.2021.
//

#ifndef RTS_DIRLIGHT_H
#define RTS_DIRLIGHT_H


#include "Light.h"

class DirLight: public Light {
    glm::vec3 direction;

public:
    DirLight(const glm::vec3 ambient, const glm::vec3 diffuse, const glm::vec3 specular, const glm::vec3 direction);

    void use(Shader *shader) override;

};


#endif //RTS_DIRLIGHT_H
