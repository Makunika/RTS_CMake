//
// Created by pshbl on 22.05.2021.
//

#include "GameObject.h"

void GameObject::setSelected(bool selected) {
    isSelected = selected;
}

void GameObject::move() {
    x += speed.x;
    y += speed.y;

    if (speed.x != 0 && speed.y !=0 && collide(point)) {\
        std::cout << "ok point!: " << point.x << " " << point.y << std::endl;


        speed = glm::vec2(0.0f);

        if (anglePostMove != -1234) {
            angleEnd = anglePostMove;
        }
        anglePostMove = -1234;
        std::cout << "angleEnd: " << angleEnd << std::endl;
    }
}

void GameObject::moveTo(glm::vec2 &point) {
    this->speed = glm::normalize(point - glm::vec2(x,y)) * speedValue;
    this->point = glm::vec2(point.x, point.y);
    angleEnd = glm::atan(speed.x, speed.y);
    anglePostMove = -1234;
}

bool GameObject::getIsSelected() {
    return isSelected;
}

void GameObject::moveTo(glm::vec2 &point, float anglePostMove) {
    moveTo(point);
    this->anglePostMove = anglePostMove;
}

GameObject::GameObject(Model *model, float x, float y, int zLevel, float scale, State *state, float speedValue,
                       float speedRotate)
        : StaticObject(model, x, y, zLevel, scale, state), speedValue(speedValue), speedRotate(speedRotate) {
    this->speed = glm::vec2(0.0f);
    point = glm::vec2(0.0f);
    anglePostMove = -1234;
    isSelected = false;
    angleEnd = 0.0f;
}

void GameObject::draw(Shader *shader, Shader *coloringShader) {
    StaticObject::draw(shader, isSelected ? coloringShader : nullptr);
}

void GameObject::update() {
    if (abs(angle - angleEnd) > 0.1) {
        rotate();
    } else {
        move();
    }
}

void GameObject::rotate() {
    angle = angleEnd;
}
