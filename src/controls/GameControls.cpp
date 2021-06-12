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
    //Если нажата/зажата ЛКМ
    if (glfwGetMouseButton(gameState->state->window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
        //Если check = false, то все такни делаются не выбранными и запиываются координаты мыши в lastX, lastY.
        if (!check) {
            for (auto &item : gameState->selectedTanks) {
                item->setSelected(false);
            }
            gameState->selectedTanks.clear();
            glfwGetCursorPos(gameState->state->window, &lastX, &lastY);
        }
        check = true;

        //Получаем координаты мыши
        double mouseX, mouseY;
        glfwGetCursorPos(gameState->state->window, &mouseX, &mouseY);

        //Если разница между последними координатамы мыши и нынешними больше 2, то рисуем квадрат для множествнного выбора
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
    }
    //Если была отпущена/не нажата клаввиша ЛКМ
    else {
        //Если check - true (она была до этого нажата)
        if (check) {
            //Получаем координаты мыши
            double mouseX, mouseY;
            glfwGetCursorPos(gameState->state->window, &mouseX, &mouseY);

            //Если разница между последними координатами (lastX, lastY) и нынешними больше 2, то делаем множетсвенный выбор.
            if (!(abs(mouseX - lastX) < 10 || abs(mouseY - lastY) < 10)) {
                //множественный выбор
                //Обновляем пикер для выбора по площади
                gameState->mousePicker->updateArea(lastX, lastY);
                //Получаем массив лучей для данной площади
                vector<glm::vec3> rays = gameState->mousePicker->getCurrentRaysArea();
                float dist;
                //Если танки попадают под один из массив лучей, то выбранный танк будет выбран
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
            }
            //Если расстояние между нынешними координатами и прошлым меньше двух, то используется одиночный выбор
            else {
                //Обнволяем луч для пикера
                gameState->mousePicker->update();
                //Поулчаем луч из пикера
                glm::vec3 ray = gameState->mousePicker->getCurrentRay();
                //Выбираем танк, который соприкасается с лучом и имеет наименьшее расстояния до камеры игрока
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
    //Если нажата/зажата ПКМ
    if (glfwGetMouseButton(gameState->state->window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
        //Если это первое нажатие то обнволяем луч куда двигаться и обновляем lastX, lastY
        if (!check2) {
            rayMoveTo = glm::vec3(0.0f, 0.0f, 1.0f);
            glfwGetCursorPos(gameState->state->window, &lastX, &lastY);
        }
        check2 = true;
        //Получаем координаты мыши
        double mouseX, mouseY;
        glfwGetCursorPos(gameState->state->window, &mouseX, &mouseY);

        //Если разница между последними координатамы мыши и нынешними больше 2, то рисуем линию для направления потсроение
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

    }
    //Если была отжата/не нажата ПКМ
    else {
        //Если до этого ПКМ была нажата и есть выбарнные танки
        if (check2 && !gameState->selectedTanks.empty()) {
            //Получаем координаты мыши
            double mouseX, mouseY;
            glfwGetCursorPos(gameState->state->window, &mouseX, &mouseY);
            //Если разница между последними координатамы мыши и нынешними больше 2, то построение будет по направлению
            if (!(abs(mouseX - lastX) < 2 && abs(mouseY - lastY) < 2)) {
                //Обновляем луч и получаем  в пикере для прошлых координат
                gameState->mousePicker->update(lastX, lastY);
                glm::vec3 mouseRay1 = gameState->mousePicker->getCurrentRay();
                //Обновляем и получаем луч для нынешних координат
                gameState->mousePicker->update(mouseX, mouseY);
                glm::vec3 mouseRay2 = gameState->mousePicker->getCurrentRay();
                //Получаем координаты пересечения лучей с плоскостью XZ
                glm::vec3 center = gameState->mousePicker->calculateWithArea(mouseRay1);
                glm::vec3 dir = gameState->mousePicker->calculateWithArea(mouseRay2) - center;

                //Расставляем по точкам выбранные танки по сетке
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
                //Поворачиваем выбранные точки вокруг центра при помощи луча направления построения
                glm::vec2 centerPoint = points.at(0);
                float angle = glm::atan(dir.x, -dir.z);
                float angle2 = glm::atan(dir.x, dir.z);
                gameState->selectedTanks.at(0)->moveTo(centerPoint, angle2);
                for (int i = 1; i < points.size(); ++i) {
                    glm::vec2 newPoint = glm::rotate(points.at(i) - centerPoint, angle) + centerPoint;
                    gameState->selectedTanks.at(i)->moveTo(newPoint, angle2);
                }
            }
            //Делаем построение без направления
            else {
                //Если выбранных танков 1, то просто его туда двигаем.
                if (gameState->selectedTanks.size() < 2) {
                    gameState->mousePicker->update(mouseX, mouseY);
                    glm::vec3 mouseRay = gameState->mousePicker->getCurrentRay();
                    glm::vec3 p = gameState->mousePicker->calculateWithArea(mouseRay);
                    for (const auto &item : gameState->selectedTanks) {
                        glm::vec2 point = glm::vec2(p.x, p.z);
                        item->moveTo(point);
                    }
                }
                //Иначе делаем постреоние по аналогии с направленным построением, но без поворота точек
                else {
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
