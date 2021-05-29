//
// Created by pshbl on 22.05.2021.
//
#pragma once
#include <ostream>
#include "../models/Model.h"
#include "Collider.h"

class GameObject {
protected:
    Model* model;
    vector<Collider*> colliders;
    float x, y;
    int zLevel;
    float scale;
    bool isSelected;
    glm::vec2 speed;
    glm::vec2 point;
    float speedValue;
    float angle;
    float anglePostMove;

public:
    GameObject(Model *model,float x, float y, int zLevel, float scale, float speed);
    bool collide(GameObject* gameObject);
    bool collide(glm::vec2& point);
    void addCollider(Collider* collider);
    void draw(Shader* shader, Shader* coloringShader);
    void setSelected(bool selected);
    bool getIsSelected();
    bool intersectionRay(glm::vec3 &direction, glm::vec3& start, float& dist);
    void move();
    void moveTo(glm::vec2 &point);
    void moveTo(glm::vec2 &point, float anglePostMove);

    friend ostream &operator<<(ostream &os, const GameObject &object);
};
