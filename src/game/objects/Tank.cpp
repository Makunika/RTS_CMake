//
// Created by pshbl on 22.05.2021.
//

#include "Tank.h"
#include "../../resource/ResourceManager.h"

Tank::Tank(float x, float y, int zLevel, float scale, State* state): GameObject(
        ResourceManager::loadModel("backpack\\backpack.obj"),
        x,
        y,
        zLevel,
        scale,
        state,
        1.0f / 60) {
    addCollider(new Collider(0.0f, 0.0f, 0.5f, state));
    if (modelShader == nullptr) {
        modelShader = ResourceManager::loadShader("model_test");
    }
    if (selectedShader == nullptr) {
        selectedShader = ResourceManager::loadShader("singleColor");
    }

}

bool Tank::isInit = false;
Shader* Tank::modelShader = nullptr;
Shader* Tank::selectedShader = nullptr;
