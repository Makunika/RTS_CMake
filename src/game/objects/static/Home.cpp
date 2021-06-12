//
// Created by pshbl on 10.06.2021.
//

#include "Home.h"
#include "../../../resource/ResourceManager.h"

Shader* Home::modelShader = nullptr;

Model* getHome(HouseType homeType) {
    std::string path;
    switch (homeType) {
        case HOUSE1:
            path = "home\\home1\\home.obj";
            break;
        case HOUSE2:
            path = "home\\home2\\home.obj";
            break;
        case HOUSE3:
            path = "home\\home3\\home.obj";
            break;
    }
    return ResourceManager::loadModel(path);
}

float getScale(HouseType houseType) {
    switch (houseType) {
        case HOUSE1:
            return 0.07f;
        case HOUSE2:
            return 0.2f;
        case HOUSE3:
            return 0.17f;
    }
}

Home::Home(float x, float y, State *state, HouseType houseType, float angle)
: Home(x, y, state, houseType, angle, false)
{
}

Home::Home(float x, float y, State *state, HouseType houseType, float angle, bool light)
        : StaticObject(getHome(houseType), x, y, 0, getScale(houseType), state)
{
    addCollider(new Collider(0.0f, -0.5f, 1.8f, state));
    if (modelShader == nullptr) {
        modelShader = ResourceManager::loadShader("staticObjects");
    }
    //angleX = -AI_MATH_HALF_PI_F;
    this->angle = glm::radians(angle);
    this->isLight = light;
    if (isLight) {
        pointLight = new PointLight(
                glm::vec3(0.1f),
                glm::vec3(0.5f),
                glm::vec3(1.0f),
                glm::vec3(x, 0.5f, y),
                1.0f,
                0.5f,
                0.20f
        );
    }
}
