//
// Created by pshbl on 22.05.2021.
//

#include "Tank.h"
#include "../utils/Utils.h"

Model* tankModel = nullptr;
Collider* tankCollider = nullptr;

Tank::Tank(float x, float y, int zLevel, float scale, State* state): GameObject(
        !isInit ? new Model(Utils::getResourcesPathString() + "objects\\backpack\\backpack.obj") : tankModel,
        !isInit ? new Collider(0.0, 0.0, 1.0, 1.0, state) : tankCollider,
        x,
        y,
        zLevel,
        scale,
        state) {
    if (!isInit) {
        tankCollider = this->collider;
        tankModel = this->model;
    }
    isInit = true;
}

bool Tank::isInit = false;
