//
// Created by pshbl on 22.05.2021.
//

#include "Tank.h"
#include "../../../resource/ResourceManager.h"
#include <glm/gtx/rotate_vector.hpp>

Tank::Tank(float x, float y, int zLevel, State* state): GameObject(
        ResourceManager::loadModel("tank\\IS4.obj"),
        x,
        y,
        zLevel,
        0.15,
        state,
        0.8f, 1.0f) {
    addCollider(new Collider(0.0f, 0.0f, 0.5f, state));
    if (modelShader == nullptr) {
        modelShader = ResourceManager::loadShader("model_test");
    }
    if (selectedShader == nullptr) {
        selectedShader = ResourceManager::loadShader("singleColor");
    }
    lightLeft = new SpotLight(
            glm::vec3(0.05f),
            glm::vec3(0.5f),
            glm::vec3(1.0f),
            getPositionLightLeft(),
            getFront(),
            glm::cos(glm::radians(32.5f)),
            glm::cos(glm::radians(37.5f)),
            1.0f,
            0.09f,
            0.32f
    );
}

bool Tank::isInit = false;
Shader* Tank::modelShader = nullptr;
Shader* Tank::selectedShader = nullptr;

glm::vec3 Tank::getPositionLightLeft() {
    return getPosition(-0.12f, 0.5f, 0.1f);
}

void Tank::update(vector<StaticObject *> *objects) {
    lightLeft->setDirection(getFront());
    lightLeft->setPosition(getPositionLightLeft());
    GameObject::update(objects);
}

void Tank::draw(Shader *shader, Shader *coloringShader) {
    glDisable(GL_CULL_FACE);
    GameObject::draw(shader, coloringShader);
    glEnable(GL_CULL_FACE);
}
