//
// Created by pshbl on 22.05.2021.
//
#pragma once
#include "../shaders/ShaderUtils.h"
#include "../shaders/Shader.h"
#include "../state/State.h"
#include <glm/glm.hpp>
#include <glm/ext/matrix_transform.hpp>


class Collider {
public:
    //относительно центра игрового объекта
    float x, y, w, h;
    State* state;
    static bool isDrawable;
    static VAO_VBO vaoVbo;
    static Shader* debugShader;

    Collider(float x, float y, float w, float h, State* state) {
        this->x = x;
        this->y = y;
        this->w = w;
        this->h = h;
        this->state = state;
    }

    bool collide(float xOffset, float yOffset, Collider* collider) {
        float currentX = collider->x + xOffset;
        float currentY = collider->y + yOffset;
        float currentW = collider->w;
        float currentH = collider->h;

        if (currentX <= (x - currentW)) {
            //, то прямоугольник 2 находится слева от 1
            return false;
        } else if (currentX >= (x + w)) {
            // справа
            return false;
        } else if (currentY <= (y - currentH)) {
            //сверху
            return false;
        } else if (currentY >= (y + h)) {
            //снизу
            return false;
        }

        return true;
    }

    void drawDebug(float ownerX, float ownerY, int ownerZLevel) {
        if (debugShader == nullptr) {
            debugShader = new Shader("debugShader");
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
        model = glm::scale(model, glm::vec3(w, 1.0f, h));
        glBindVertexArray(vaoVbo.VAO);
        debugShader->setMatrix4("model", model);
        glDrawArrays(GL_TRIANGLES, 0, 36);
        glBindVertexArray(0);
    }

    vector<glm::vec3> getCurrentSize(float xOffset, float yOffset, int zLevel) {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(x + xOffset, 0.0f + 1.0 * zLevel, y + yOffset ));
        model = glm::scale(model, glm::vec3(w, 1.0f, h));
        vector<glm::vec3> points;
        points.push_back(model * glm::vec4(-0.5, -0.5, -0.5, 1.0));
        points.push_back(model * glm::vec4(0.5, 0.5, 0.5, 1.0));
        return points;
    }
};