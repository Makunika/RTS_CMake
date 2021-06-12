//
// Created by pshbl on 24.05.2021.
//

#include "MousePicker.h"

MousePicker::MousePicker(State *state) : state(state) {

}

const glm::vec3 &MousePicker::getCurrentRay() const {
    return currentRay;
}

glm::vec3 MousePicker::calculateMouseRay(double mouseX, double mouseY) {
    //Нормализуем координаты
    glm::vec2 ray_nds = getNormalizedDeviceCoord(mouseX, mouseY);
    //Переводим в clip space
    glm::vec4 ray_clip = glm::vec4(ray_nds.x, ray_nds.y, -1.0f, 1.0f);
    //Получаем инвентируемую матрице проекции
    glm::mat4 invProjMat = glm::inverse(state->getProjection());
    //Переводим в view space
    glm::vec4 eyeCoords = invProjMat * ray_clip;
    eyeCoords = glm::vec4(eyeCoords.x, eyeCoords.y, -1.0f, 0.0f);
    //Получаем инвентируемую матрицу отображения
    glm::mat4 invViewMat = glm::inverse(state->camera->getViewMatrix());
    //Переводим в мировое пространство и возвращаем луч
    glm::vec4 rayWorld = invViewMat * eyeCoords;
    glm::vec3 rayDirection = glm::normalize(glm::vec3(rayWorld));
    return rayDirection;
}

glm::vec2 MousePicker::getNormalizedDeviceCoord(float mouseX, float mouseY) {
    int w, h;
    glfwGetWindowSize(state->window, &w, &h);

    float x = (2.0f * mouseX) / w - 1;
    mouseY = -(mouseY - h);
    float y = (2.0f * mouseY) / h - 1;
    return glm::vec2(x, y);
}

void MousePicker::update() {
    double mouseX, mouseY;
    glfwGetCursorPos(state->window, &mouseX, &mouseY);
    currentRay = calculateMouseRay(mouseX, mouseY);
}

void MousePicker::updateArea(double lastX, double lastY) {
    currentRaysArea.clear();
    double mouseX, mouseY;
    glfwGetCursorPos(state->window, &mouseX, &mouseY);

    if (abs(mouseX - lastX) < 20 || abs(mouseY - lastY) < 20) {
        return;
    }

    double startX, startY, endX, endY;
    if (lastX < mouseX && lastY > mouseY) {
        //справа вверх
        startX = lastX;
        startY = mouseY;
        endX = mouseX;
        endY = lastY;
    } else if (lastX < mouseX && lastY < mouseY) {
        //справа вниз
        startX = lastX;
        startY = lastY;
        endX = mouseX;
        endY = mouseY;
    } else if (lastX > mouseX && lastY > mouseY) {
        //слева вверх
        startX = mouseX;
        startY = mouseY;
        endX = lastX;
        endY = lastY;
    } else if (lastX > mouseX && lastY < mouseY) {
        //слева вниз
        startX = mouseX;
        startY = lastY;
        endX = lastX;
        endY = mouseY;
    }

    for (int i = startX; i < endX; i+=20) {
        for (int j = startY; j < endY; j+=20) {
            currentRaysArea.push_back(calculateMouseRay(i, j));
        }
    }
}

const std::vector<glm::vec3> &MousePicker::getCurrentRaysArea() const {
    return currentRaysArea;
}

void MousePicker::update(double mouseX, double mouseY) {
    currentRay = calculateMouseRay(mouseX, mouseY);
}

glm::vec3 MousePicker::calculateWithArea(glm::vec3 &ray) {
    glm::vec3 N = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 p = glm::vec3(1.0f, 0.0f, 1.0f);
    const float eps = 1.0e-5f;
    float ratio = glm::dot(N, ray);   // косинус нормали с лучом
    float d = glm::dot(N, p - state->camera->Position);  // расстояние от плоскости до rayPos по нормали
    float t =  d / ratio;     // возвращаем расстояние по лучу

    glm::vec3 inters;
    if (t >= 0.0f) {
        inters = state->camera->Position + ray * t;
        return inters;
    }
    return glm::vec3(0.0f);
}
