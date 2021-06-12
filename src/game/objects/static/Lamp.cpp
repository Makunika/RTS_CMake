//
// Created by pshbl on 11.06.2021.
//

#include "Lamp.h"
#include "../../../resource/ResourceManager.h"

Lamp::Lamp(float x, float y, State *state, float angle)
        : StaticObject(ResourceManager::loadModel("lamp\\lamp.obj"), x, y, 0, 0.4f, state)
{
    this->angle = glm::radians(angle);

    if (modelShader == nullptr) {
        modelShader = ResourceManager::loadShader("staticObjects");
    }
    pointLight = new PointLight(
            glm::vec3(0.2f),
            glm::vec3(0.5f),
            glm::vec3(1.0f),
            getPositionLight(),
            1.0f,
            0.35f,
            0.44f
    );
}

Shader* Lamp::modelShader = nullptr;

const glm::vec3 Lamp::getPositionLight() {
    return glm::vec3(x, 1.5f, y);
}
