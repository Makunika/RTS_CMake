//
// Created by pshbl on 28.05.2021.
//

#include "GameControls.h"

GameControls::GameControls(GameState *gameState) : gameState(gameState) {
    check = false;
    check2 = false;

    selectQuadShader = ResourceManager::loadShader("selectQuadShader", true);
    vector<float> pointf = { 0.0f, 0.0f };
    vPoint = ShaderUtils::loadvec2(&pointf[0], pointf.size());
    lineShader = ResourceManager::loadShader("line", true);
}

void GameControls::processMouse() {
    processSelecting();
    processMoved();
}

void GameControls::processSelecting() {
    if (glfwGetMouseButton(gameState->state->window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
        if (!check) {
            for (auto &item : gameState->selectedTanks) {
                item->setSelected(false);
            }
            gameState->selectedTanks.clear();
            glfwGetCursorPos(gameState->state->window, &lastX, &lastY);
        }
        check = true;

        double mouseX, mouseY;
        glfwGetCursorPos(gameState->state->window, &mouseX, &mouseY);

        if (!(abs(mouseX - lastX) < 2 || abs(mouseY - lastY) < 2)) {

            glm::vec2 translate = gameState->mousePicker->getNormalizedDeviceCoord(lastX, lastY);
            glm::vec2 size = translate - gameState->mousePicker->getNormalizedDeviceCoord(mouseX, mouseY);

            selectQuadShader->use();
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(translate.x, translate.y, 0.0f));
            selectQuadShader->setMatrix4("model", model);
            selectQuadShader->setVec2("size", size);
            glBindVertexArray(vPoint.VAO);
            glDrawArrays(GL_POINTS, 0, 1);
        }
    } else {
        if (check) {
            double mouseX, mouseY;
            glfwGetCursorPos(gameState->state->window, &mouseX, &mouseY);
            if (!(abs(mouseX - lastX) < 10 || abs(mouseY - lastY) < 10)) {
                //множественный выбор
                gameState->mousePicker->updateArea(lastX, lastY);
                vector<glm::vec3> rays = gameState->mousePicker->getCurrentRaysArea();
                float dist;
                for (const auto &tank : gameState->tanks) {
                    for (const auto &ray : rays) {
                        glm::vec3 r = ray;
                        if (tank->intersectionRay(r, gameState->state->camera->Position, dist)) {
                            tank->setSelected(true);
                            gameState->selectedTanks.push_back(tank);
                            break;
                        }
                    }
                }
            } else {
                //Одиночный выбор
                gameState->mousePicker->update();
                glm::vec3 ray = gameState->mousePicker->getCurrentRay();
                Tank* lastTank = nullptr;
                float dist;
                float minDist = 1000000000;
                for (const auto &item : gameState->tanks)  {
                    item->setSelected(false);
                    if (item->intersectionRay(ray, gameState->state->camera->Position, dist)) {
                        if (dist < minDist) {
                            minDist = dist;
                            if (lastTank != nullptr) {
                                lastTank->setSelected(false);
                            }
                            lastTank = item;
                            item->setSelected(true);
                        }
                    }
                }
                if (lastTank != nullptr) {
                    gameState->selectedTanks.push_back(lastTank);
                }
            }

        }
        check = false;
    }
}

void GameControls::processMoved() {
    if (glfwGetMouseButton(gameState->state->window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
        if (!check2) {
            rayMoveTo = glm::vec3(0.0f, 0.0f, 1.0f);
            glfwGetCursorPos(gameState->state->window, &lastX, &lastY);
        }
        check2 = true;

        double mouseX, mouseY;
        glfwGetCursorPos(gameState->state->window, &mouseX, &mouseY);

        if (!(abs(mouseX - lastX) < 2 && abs(mouseY - lastY) < 2)) {

            glm::vec2 translate = gameState->mousePicker->getNormalizedDeviceCoord(lastX, lastY);
            glm::vec2 size = translate - gameState->mousePicker->getNormalizedDeviceCoord(mouseX, mouseY);

            lineShader->use();
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(translate.x, translate.y, 0.0f));
            lineShader->setMatrix4("model", model);
            lineShader->setVec2("offset", size);
            glBindVertexArray(vPoint.VAO);
            glDrawArrays(GL_POINTS, 0, 1);
        }

    } else {
        if (check2 && !gameState->selectedTanks.empty()) {
            double mouseX, mouseY;
            glfwGetCursorPos(gameState->state->window, &mouseX, &mouseY);
            if (!(abs(mouseX - lastX) < 2 && abs(mouseY - lastY) < 2)) {
                gameState->mousePicker->update(lastX, lastY);
                glm::vec3 mouseRay1 = gameState->mousePicker->getCurrentRay();
                gameState->mousePicker->update(mouseX, mouseY);
                glm::vec3 mouseRay2 = gameState->mousePicker->getCurrentRay();
                glm::vec3 center = gameState->mousePicker->calculateWithArea(mouseRay1);
                glm::vec3 dir = gameState->mousePicker->calculateWithArea(mouseRay2) - center;
                vector<glm::vec2> points;
                for (int i = 0; i < gameState->selectedTanks.size(); ++i) {
                    if (i == 0 || i % 3 == 0) {
                        glm::vec2 point = glm::vec2(center.x, center.z + i / 3 * 1.5f);
                        points.push_back(point);
                    }
                    if (i % 3 == 1) {
                        glm::vec2 point = glm::vec2(center.x + 1.5f, center.z + i / 3 * 1.5f);
                        points.push_back(point);
                    }
                    if (i % 3 == 2) {
                        glm::vec2 point = glm::vec2(center.x - 1.5f, center.z + i / 3 * 1.5f);
                        points.push_back(point);
                    }
                }
                glm::vec2 centerPoint = points.at(0);
                float angle = glm::atan(dir.x, -dir.z);
                float angle2 = glm::atan(dir.x, dir.z);
                gameState->selectedTanks.at(0)->moveTo(centerPoint, angle2);
                for (int i = 1; i < points.size(); ++i) {
                    glm::vec2 newPoint = glm::rotate(points.at(i) - centerPoint, angle) + centerPoint;
                    gameState->selectedTanks.at(i)->moveTo(newPoint, angle2);
                }
            } else {
                if (gameState->selectedTanks.size() < 2) {
                    gameState->mousePicker->update(mouseX, mouseY);
                    glm::vec3 mouseRay = gameState->mousePicker->getCurrentRay();
                    glm::vec3 p = gameState->mousePicker->calculateWithArea(mouseRay);
                    for (const auto &item : gameState->selectedTanks) {
                        glm::vec2 point = glm::vec2(p.x, p.z);
                        item->moveTo(point);
                    }
                } else {
                    gameState->mousePicker->update(mouseX, mouseY);
                    glm::vec3 mouseRay = gameState->mousePicker->getCurrentRay();
                    glm::vec3 center = gameState->mousePicker->calculateWithArea(mouseRay);
                    glm::vec3 dir = glm::vec3(0.0f, 0.0f, -1.0f);
                    vector<glm::vec2> points;
                    for (int i = 0; i < gameState->selectedTanks.size(); ++i) {
                        if (i == 0 || i % 3 == 0) {
                            glm::vec2 point = glm::vec2(center.x, center.z + i / 3 * 1.5f);
                            points.push_back(point);
                        }
                        if (i % 3 == 1) {
                            glm::vec2 point = glm::vec2(center.x + 1.5f, center.z + i / 3 * 1.5f);
                            points.push_back(point);
                        }
                        if (i % 3 == 2) {
                            glm::vec2 point = glm::vec2(center.x - 1.5f, center.z + i / 3 * 1.5f);
                            points.push_back(point);
                        }
                    }
                    glm::vec2 centerPoint = points.at(0);
                    gameState->selectedTanks.at(0)->moveTo(centerPoint);
                    for (int i = 1; i < points.size(); ++i) {
                        gameState->selectedTanks.at(i)->moveTo(points.at(i));
                    }
                }
            }
        }
        check2 = false;
    }
}
