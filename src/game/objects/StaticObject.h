//
// Created by pshbl on 07.06.2021.
//

#ifndef RTS_STATICOBJECT_H
#define RTS_STATICOBJECT_H


#include "../../models/Model.h"
#include "../Collider.h"

class StaticObject {
protected:
    Model* model;
    vector<Collider*> colliders;
    float x, y;
    int zLevel;
    float scale;
    float angle;
    State* state;

public:

    StaticObject(Model *model, float x, float y, int zLevel, float scale, State *state);

    virtual void draw(Shader* shader, Shader* coloringShader);
    void addCollider(Collider* collider);
    bool collide(StaticObject* staticObject);
    bool collide(glm::vec2& point);
    bool intersectionRay(glm::vec3 &direction, glm::vec3& start, float& dist);

    glm::vec3 getPosition();
    glm::vec3 getFront();

};


#endif //RTS_STATICOBJECT_H
