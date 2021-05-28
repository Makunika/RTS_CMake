//
// Created by pshbl on 28.05.2021.
//

#include "PointLight.h"

void PointLight::use(Shader *shader) {
    shader->use();
    std::string point = "pointLights[" + std::to_string(index) + "]";
    shader->setVec3(point + ".ambient", ambient);
    shader->setVec3(point + ".diffuse", diffuse);
    shader->setVec3(point + ".specular", specular);
    shader->setFloat(point + ".constant", constant);
    shader->setFloat(point + ".linear", linear);
    shader->setFloat(point + ".quadratic", quadratic);
    shader->setVec3(point + ".position", position);
}

PointLight::PointLight(const glm::vec3 ambient, const glm::vec3 diffuse, const glm::vec3 specular,
                       const glm::vec3 position, float constant, float linear, float quadratic) : Light(ambient,
                                                                                                         diffuse,
                                                                                                         specular),
                                                                                                   position(position),
                                                                                                   constant(constant),
                                                                                                   linear(linear),
                                                                                                   quadratic(
                                                                                                           quadratic) {
    index = counter++;
}

int PointLight::getCount() {
    return counter;
}

void PointLight::setPosition(const glm::vec3 position) {
    PointLight::position = position;
}

int PointLight::counter = 0;
