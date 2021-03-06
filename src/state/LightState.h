//
// Created by pshbl on 28.05.2021.
//

#ifndef RTS_LIGHTSTATE_H
#define RTS_LIGHTSTATE_H
#include <vector>
#include "../light/Light.h"
#include "../light/PointLight.h"
#include "../light/SpotLight.h"
#include "../light/DirLight.h"

using namespace std;

struct LightState {
    //Весь свет
    vector<Light*> lights;
    //Направленный свет (от солнца)
    DirLight* dirLight;

    //Использовать весь свет для шейдера (если день, то только направленный свет, если ночь - то все источнийи света)
    void allUse(Shader* shader, bool isDay) {
        shader->use();
        if (isDay) {
            shader->setInt("countPointLights", 0);
            shader->setInt("countSpotLights", 0);
            dirLight->use(shader);
        } else {
            shader->setInt("countPointLights", getCountPointLight());
            shader->setInt("countSpotLights", getCountSpotLight());

            for (const auto &light : lights) {
                light->use(shader);
            }
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
