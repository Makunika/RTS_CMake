//
// Created by pshbl on 22.05.2021.
//
#pragma once
#include "../shaders/ShaderUtils.h"
#include "../shaders/Shader.h"
#include "../state/State.h"
#include "../resource/ResourceManager.h"
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtx/intersect.hpp>

struct Offsets {
    float xOffset;
    float yOffset;
    int zLevel;
};

class Collider {
public:
    //относительно центра игрового объекта
    float x, y, r;
    State* state;
    static bool isDrawable;
    static VAO_VBO vaoVbo;
    static Shader* debugShader;

    Collider(float x, float y, float r, State* state) {
        this->x = x;
        this->y = y;
        this->r = r;
        this->state = state;
    }

    bool intersectRay(Offsets offsets, glm::vec3& start, glm::vec3& direction, float& dist) const {
        return glm::intersectRaySphere(
                start,
                direction,
                glm::vec3(x + offsets.xOffset, 0.0f + 1.0f * offsets.zLevel, y + offsets.yOffset),
                r * r,
                dist);
    }

    bool intersectCollider(Offsets ownerOffsets, Offsets otherOffsets, Collider* collider, float& dist) const {
        glm::vec3 centerOther = glm::vec3(
                collider->x + otherOffsets.xOffset,
                0.0f + 1.0f * otherOffsets.zLevel,
                collider->y + otherOffsets.yOffset
        );
        glm::vec3 centerOwner = glm::vec3(
                x + ownerOffsets.xOffset,
                0.0f + 1.0f * ownerOffsets.zLevel,
                y + ownerOffsets.yOffset
        );
        glm::vec3 dir = glm::normalize(centerOwner - centerOther);

        bool intersect = intersectRay(
                ownerOffsets,
                centerOther,
                dir,
                dist
        );

        return intersect;
    }

    bool collide(Offsets ownerOffsets, Offsets otherOffsets, Collider* collider, float& dist) const {
        if (intersectCollider(ownerOffsets, otherOffsets, collider, dist)) {
            return dist <= r + collider->r;
        }
        return false;
    }

    void drawDebug(float ownerX, float ownerY, int ownerZLevel) {
        if (debugShader == nullptr) {
            debugShader = ResourceManager::loadShader("debugShader");
        }

        debugShader->use();
        if (!isDrawable) {
            vector<float> v = ShaderUtils::getCube();
            for (auto &item : v) {
                item = item / 2.0f;
            }
            vaoVbo = ShaderUtils::loadOnlyCoordinate(&v[0], v.size());
            isDrawable = true;
        }

        glm::mat4 view = state->camera->getViewMatrix();
        glm::mat4 projection = state->getProjection();
        debugShader->setMatrix4("projection", projection);
        debugShader->setMatrix4("view", view);

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(x + ownerX, 0.0f + 1.0 * ownerZLevel, y + ownerY ));
        model = glm::scale(model, glm::vec3(r * 2, 1.0f, r * 2));
        glBindVertexArray(vaoVbo.VAO);
        debugShader->setMatrix4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
    }
};