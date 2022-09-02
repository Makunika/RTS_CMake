//
// Created by pshbl on 28.05.2021.
//

#include "DirLight.h"

void DirLight::use(Shader *shader) {
    shader->use();
    shader->setVec3("dirLight.direction", direction);
    shader->setVec3("dirLight.ambient", ambient);
    shader->setVec3("dirLight.diffuse", diffuse);
    shader->setVec3("dirLight.specular", specular);
}

DirLight::DirLight(const glm::vec3 ambient, const glm::vec3 diffuse, const glm::vec3 specular,
                   const glm::vec3 direction) : Light(ambient, diffuse, specular), direction(direction) {}

void DirLight::change(glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular) {
    this->ambient = ambient;
    this->diffuse = diffuse;
    this->specular = specular;
}

void DirLight::changeDir(glm::vec3 direction) {
    this->direction = direction;
}

glm::vec3 DirLight::getDirection() {
    return this->direction;
}
