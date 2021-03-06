//
// Created by pshbl on 07.06.2021.
//

#ifndef RTS_STATICOBJECT_H
#define RTS_STATICOBJECT_H


#include "../../../models/Model.h"
#include "../../Collider.h"

class StaticObject {
protected:
    Model* model;
    vector<Collider*> colliders;
    float x, y;
    int zLevel;
    float scale;
    float angle;
    float angleX = 0;
    float offsetZ = 0;
    State* state;

public:

    StaticObject(Model *model, float x, float y, int zLevel, float scale, State *state);
    StaticObject(Model *model, float x, float y, int zLevel, float scale, State *state, float offsetZ);

    virtual void draw(Shader* shader, Shader* coloringShader);
    void addCollider(Collider* collider);
    bool collide(StaticObject* staticObject);
    bool collide(StaticObject* staticObject, glm::vec3 dir);
    bool collide(glm::vec2& point);
    bool intersectionRay(glm::vec3 &direction, glm::vec3& start, float& dist);

    glm::vec3 getPosition();
    glm::vec3 getPosition(float offsetX, float offsetY, float offsetZ);
    glm::vec3 getFront();

};


#endif //RTS_STATICOBJECT_H
