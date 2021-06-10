//
// Created by pshbl on 22.05.2021.
//

#include "GameObject.h"

void GameObject::setSelected(bool selected) {
    isSelected = selected;
}

void GameObject::move() {
    x += speed.x * state->deltaTime;
    y += speed.y * state->deltaTime;

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
    cout << endl;
    cout << endl;
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

void GameObject::update(vector<StaticObject *> *objects) {
    if (abs(angle - angleEnd) > 0.01) {
        //rotate();
        angle = angleEnd;
    } else {
        glm::vec3 dir = glm::vec3(speed.x, 0.0f, speed.y);
        for (const auto &home : *objects) {
            if (collide(home, dir)) {
                speed = glm::vec3(0.0f);
                anglePostMove = -1234;
                return;
            }
        }
        move();
    }
}

void GameObject::rotate() {

    cout << "angle: " << angle << endl;
    cout << "angleEnd: " << angleEnd << endl;


    float x = glm::sin(angle);
    float z = glm::cos(angle);

    float xEnd = glm::sin(angleEnd);
    float zEnd = glm::cos(angleEnd);

    cout << "x: " << x << "z: " << z << endl;
    cout << "xEnd: " << xEnd << "zEnd: " << zEnd << endl << "next" << endl;

    if (xEnd > x) {
        x += 0.11f;
        if (x > xEnd)
            x = xEnd;
    } else if (xEnd != x) {
        x -= 0.11f;
        if (x < xEnd)
            x = xEnd;
    }

    if (zEnd > z) {
        z += 0.1f;
        if (z > zEnd)
            z = zEnd;
    } else if (zEnd != z) {
        z -= 0.1f;
        if (z < zEnd)
            z = zEnd;
    }

    cout << "x: " << x << "z: " << z << endl;
    cout << "xEnd: " << xEnd << "zEnd: " << zEnd << endl;

    angle = glm::atan(x, z);

    cout << "angle: " << angle << endl;
    cout << "=============" << endl;
}
