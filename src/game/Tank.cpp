//
// Created by pshbl on 22.05.2021.
//

#include "Tank.h"
#include "../utils/Utils.h"

Model* tankModel = nullptr;

Tank::Tank(float x, float y, int zLevel, float scale, State* state): GameObject(
        !isInit ? new Model(Utils::getResourcesPathString() + "objects\\backpack\\backpack.obj") : tankModel,
        x,
        y,
        zLevel,
        scale,
        state) {
    if (!isInit) {
        tankModel = this->model;
    }
    isInit = true;
    addCollider(new Collider(0.0f, 0.0f, 0.5f, state));
}

bool Tank::isInit = false;
