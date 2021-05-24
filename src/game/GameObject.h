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
    Collider* collider;
    State* state;
    float x, y;
    int zLevel;
    float scale;
    bool isSelected;

public:
    GameObject(Model *model, Collider *collider, float x, float y, int zLevel, float scale, State* state);
    bool collide(GameObject* gameObject);
    void draw(Shader& shader, Shader& coloringShader);
    void setSelected(bool selected);
    bool intersectionRay(glm::vec3 &direction, glm::vec3& start, float& dist);

    friend ostream &operator<<(ostream &os, const GameObject &object);
};
