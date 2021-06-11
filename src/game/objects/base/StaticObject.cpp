//
// Created by pshbl on 07.06.2021.
//

#include "StaticObject.h"
#include <glm/gtx/rotate_vector.hpp>

void StaticObject::draw(Shader *shader, Shader *coloringShader) {

    shader->use();
    glm::mat4 modelS = glm::mat4(1.0f);
    modelS = glm::translate(modelS, glm::vec3(x, 0.0f + 1.0 * zLevel + offsetZ, y));
    modelS = glm::scale(modelS, glm::vec3(scale));
    modelS = glm::rotate(modelS, angle, glm::vec3(0.0f, 1.0f, 0.0f));
    if (angleX != 0) {
        modelS = glm::rotate(modelS, angleX, glm::vec3(1.0f, 0.0f, 0.0f));
    }
    shader->setMatrix4("model", modelS);
    if (coloringShader != nullptr) {
        coloringShader->use();
        ////если не по нормали
        modelS = glm::mat4(1.0f);
        modelS = glm::translate(modelS, glm::vec3(x, 0.0f + 1.0 * zLevel + offsetZ, y));
        modelS = glm::scale(modelS, glm::vec3(scale + scale * 0.1f));
        modelS = glm::rotate(modelS, angle, glm::vec3(0.0f, 1.0f, 0.0f));
        /////////////////////////////////
        coloringShader->setMatrix4("model", modelS);

        model->drawSelected(*shader, *coloringShader);
    } else {
        model->draw(*shader);
    }

    if (state->debug) {
        for (const auto &collider : colliders)
            collider->drawDebug(this->x, this->y, this->zLevel);
    }
}

void StaticObject::addCollider(Collider *collider) {
    colliders.push_back(collider);
}

bool StaticObject::collide(StaticObject *staticObject) {
    if (this == staticObject) {
        return false;
    }
    float dist;
    for (const auto &wCollider : colliders) {
        for (const auto &oCollider : staticObject->colliders) {
            if (wCollider->collide(
                    Offsets { x, y, zLevel },
                    Offsets {staticObject->x, staticObject->y, staticObject->zLevel },
                    oCollider,
                    dist)) {
                return true;
            }
        }
    }
    return false;
}

bool StaticObject::collide(glm::vec2 &point) {
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
        return dist <= 0.05;
    }
    return false;
}

bool StaticObject::intersectionRay(glm::vec3 &direction, glm::vec3 &start, float &dist) {
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

glm::vec3 StaticObject::getPosition() {
    return glm::vec3(x, 0.0f + zLevel * 1.0f, y);
}

glm::vec3 StaticObject::getFront() {
    glm::vec2 dir = glm::rotate(glm::vec2(0.0f, 1.0f), (float)(angle + AI_MATH_PI));
    return glm::normalize(glm::vec3(dir.x, -0.5f, -dir.y));
}

StaticObject::StaticObject(Model *model, float x, float y, int zLevel, float scale, State *state) : model(model), x(x),
                                                                                                    y(y),
                                                                                                    zLevel(zLevel),
                                                                                                    scale(scale),
                                                                                                    state(state) {
    angle = 0;
}

StaticObject::StaticObject(Model *model, float x, float y, int zLevel, float scale, State *state, float offset): StaticObject(model, x, y, zLevel, scale, state) {
    this->offsetZ = offset;
}

glm::vec3 StaticObject::getPosition(float offsetX, float offsetY, float offsetZ) {
    glm::vec3 pos = getPosition();
    glm::vec2 center = glm::vec2(pos.x, pos.z);
    glm::vec2 point = glm::vec2(offsetX, offsetY);

    glm::vec2 newPoint = glm::rotate(point, (float)(angle + AI_MATH_PI));
    newPoint.y = -newPoint.y;
    newPoint = newPoint + center;
    return glm::vec3(newPoint.x, pos.y + offsetZ, newPoint.y);
}

bool StaticObject::collide(StaticObject *staticObject, glm::vec3 dir) {
    float dist;
    glm::vec3 pos = getPosition();
    if (!staticObject->intersectionRay(dir, pos, dist)) {
        return false;
    }
    return collide(staticObject);
}
