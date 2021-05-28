//
// Created by pshbl on 28.05.2021.
//

#ifndef RTS_LIGHTSTATE_H
#define RTS_LIGHTSTATE_H
#include <vector>
#include "../light/Light.h"
#include "../light/PointLight.h"
#include "../light/SpotLight.h"

using namespace std;

//Хранить только не двигающийся свет
struct LightState {
    vector<Light*> lights;

    void allUse(Shader* shader) {
        shader->use();

        shader->setInt("countPointLights", getCountPointLight());
        shader->setInt("countSpotLights", getCountSpotLight());

        for (const auto &light : lights) {
            light->use(shader);
        }
    }

    int getCountPointLight() {
        return PointLight::getCount();
    }

    int getCountSpotLight() {
        return SpotLight::getCount();
    }

};


#endif //RTS_LIGHTSTATE_H
