//
// Created by pshbl on 22.05.2021.
//

#ifndef RTS_TANK_H
#define RTS_TANK_H

#include "../base/GameObject.h"
#include "../../../shaders/Shader.h"
#include "../../../light/SpotLight.h"

class Tank: public GameObject {
private:
    static bool isInit;
public:
    static Shader* modelShader;
    static Shader* selectedShader;
    SpotLight* lightLeft;
    Tank(float x, float y, int zLevel, State* state);

    glm::vec3 getPositionLightLeft();

    void update(vector<StaticObject *> *objects) override;

    void draw(Shader *shader, Shader *coloringShader) override;

    void drawForShadow(Shader* shader);
};

#endif //RTS_TANK_H
