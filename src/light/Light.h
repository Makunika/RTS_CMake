//
// Created by pshbl on 28.05.2021.
//

#ifndef RTS_LIGHT_H
#define RTS_LIGHT_H


#include <glm/glm.hpp>
#include "../shaders/Shader.h"

class Light {
protected:
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;

public:
    Light(const glm::vec3 ambient, const glm::vec3 diffuse, const glm::vec3 specular) : ambient(ambient),
                                                                                           diffuse(diffuse),
                                                                                           specular(specular) {
    }

    virtual void use(Shader* shader) = 0;
};


#endif //RTS_LIGHT_H
