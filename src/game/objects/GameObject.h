//
// Created by pshbl on 22.05.2021.
//
#pragma once
#include "StaticObject.h"


class GameObject: public StaticObject {
protected:
    bool isSelected;
    glm::vec2 speed;
    glm::vec2 point;
    float speedValue;
    float anglePostMove;

public:
    GameObject(Model *model, float x, float y, int zLevel, float scale, State *state, float speedValue);

    void draw(Shader* shader, Shader* coloringShader);

    void setSelected(bool selected);
    bool getIsSelected();
    void move();
    void moveTo(glm::vec2 &point);
    void moveTo(glm::vec2 &point, float anglePostMove);
};
