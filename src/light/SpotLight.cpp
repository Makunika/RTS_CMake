//
// Created by pshbl on 28.05.2021.
//

#include "SpotLight.h"

void SpotLight::use(Shader *shader) {
    shader->use();
    std::string spot = "spotLights[" + std::to_string(index) + "]";
    shader->setVec3(spot + ".ambient", ambient);
    shader->setVec3(spot + ".diffuse", diffuse);
    shader->setVec3(spot + ".specular", specular);
    shader->setVec3(spot + ".position", position);
    shader->setVec3(spot + ".direction", direction);
    shader->setFloat(spot + ".constant", constant);
    shader->setFloat(spot + ".linear", linear);
    shader->setFloat(spot + ".quadratic", quadratic);
    shader->setFloat(spot + ".cutOff", cutOff);
    shader->setFloat(spot + ".outerCutOff", outerCutOff);

}

SpotLight::SpotLight(const glm::vec3 ambient, const glm::vec3 diffuse, const glm::vec3 specular,
                     const glm::vec3 position, const glm::vec3 direction, float cutOff, float outerCutOff,
                     float constant, float linear, float quadratic) : Light(ambient, diffuse, specular),
                                                                      position(position), direction(direction),
                                                                      cutOff(cutOff), outerCutOff(outerCutOff),
                                                                      constant(constant), linear(linear),
                                                                      quadratic(quadratic) {
    index = counter++;
}

int SpotLight::getCount() {
    return counter;
}

void SpotLight::setPosition(const glm::vec3 position) {
    SpotLight::position = position;
}

void SpotLight::setDirection(const glm::vec3 direction) {
    SpotLight::direction = direction;
}

int SpotLight::counter = 0;
