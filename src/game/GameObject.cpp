//
// Created by pshbl on 22.05.2021.
//

#include "GameObject.h"
#include <limits>
#include <glm/gtx/intersect.hpp>

GameObject::GameObject(Model *model, float x, float y, int zLevel, float scale, float speed) : model(model),
                                                                                         x(x), y(y),
                                                                                         zLevel(zLevel), scale(scale),
                                                                                         isSelected(false), speedValue(speed)
{
    this->speed = glm::vec2(0.0f);
    point = glm::vec2(0.0f);
    angle = 0;
    anglePostMove = -1234;
}

bool GameObject::collide(GameObject *gameObject) {
    if (this == gameObject) {
        return false;
    }
    float dist;
    for (const auto &wCollider : colliders) {
        for (const auto &oCollider : gameObject->colliders) {
            if (wCollider->collide(
                    Offsets { x, y, zLevel },
                    Offsets { gameObject->x, gameObject->y, gameObject->zLevel },
                    oCollider,
                    dist)) {
                return true;
            }
        }
    }
    return false;
}

void GameObject::draw(Shader* shader, Shader* coloringShader) {
   // std::cout << *this << std::endl;
    coloringShader->use();
    glm::mat4 modelS = glm::mat4(1.0f);
    modelS = glm::translate(modelS, glm::vec3(x, 0.0f + 1.0 * zLevel, y));
    modelS = glm::scale(modelS, glm::vec3(scale + scale * 0.1f));
    if (speed.x != 0 || speed.y != 0) {
        angle = glm::atan(speed.x, speed.y);
    }
    modelS = glm::rotate(modelS, angle, glm::vec3(0.0f, 1.0f, 0.0f));
    coloringShader->setMatrix4("model", modelS);

    shader->use();
    modelS = glm::mat4(1.0f);
    modelS = glm::translate(modelS, glm::vec3(x, 0.0f + 1.0 * zLevel, y));
    modelS = glm::scale(modelS, glm::vec3(scale));
    if (speed.x != 0 || speed.y != 0) {
        angle = glm::atan(speed.x, speed.y);
    }
    modelS = glm::rotate(modelS, angle, glm::vec3(0.0f, 1.0f, 0.0f));
    shader->setMatrix4("model", modelS);
    if (isSelected) {
        model->drawSelected(*shader, *coloringShader);
    } else {
        model->draw(*shader);
    }

//    for (const auto &collider : colliders)
//        collider->drawDebug(this->x, this->y, this->zLevel);
}

ostream &operator<<(ostream &os, const GameObject &object) {
    os << " x: " << object.x << " y: " << object.y
       << " zLevel: " << object.zLevel << " scale: " << object.scale;
    return os;
}

void GameObject::setSelected(bool selected) {
    isSelected = selected;
}

bool GameObject::intersectionRay(glm::vec3 &direction, glm::vec3& start, float& dist) {

    float minDist = 1000000;
    bool intersect = false;

    for (const auto &collider : colliders) {
        if (collider->intersectRay(Offsets { x, y, zLevel }, start, direction, dist)) {
            if (dist < minDist) {
                minDist = dist;
            }
            intersect = true;
        }
    }
    if (intersect) {
        dist = minDist;
        std::cout << "dist: " << dist << std::endl;
    }
    return intersect;
}

void GameObject::addCollider(Collider *collider) {
    colliders.push_back(collider);
}

void GameObject::move() {
    x += speed.x;
    y += speed.y;

    if (speed.x != 0 && speed.y !=0 && collide(point)) {\
        std::cout << "ok point!: " << point.x << " " << point.y << std::endl;
        speed = glm::vec2(0.0f);

        if (anglePostMove != -1234) {
            angle = anglePostMove;
        }
        anglePostMove = -1234;
    }
}

bool GameObject::collide(glm::vec2 &point) {
    float dist;
    glm::vec3 centerOther = glm::vec3(
            point.x,
            0.0f + 1.0f * zLevel,
            point.y
    );
    glm::vec3 centerOwner = glm::vec3(
            x,
            0.0f + 1.0f * zLevel,
            y
    );
    glm::vec3 dir = glm::normalize(centerOwner - centerOther);

    bool intersect = glm::intersectRaySphere(
            centerOther,
            dir,
            centerOwner,
            0.001 * 0.001,
            dist);

    if (intersect) {
        return dist <= 0.01;
    }
    return false;
}

void GameObject::moveTo(glm::vec2 &point) {
    this->speed = glm::normalize(point - glm::vec2(x,y)) * speedValue;
    this->point = glm::vec2(point.x, point.y);
}

bool GameObject::getIsSelected() {
    return isSelected;
}

void GameObject::moveTo(glm::vec2 &point, float anglePostMove) {
    moveTo(point);
    this->anglePostMove = anglePostMove;
}
