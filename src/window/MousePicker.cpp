//
// Created by pshbl on 24.05.2021.
//

#include "MousePicker.h"

MousePicker::MousePicker(State *state) : state(state) {

}

const glm::vec3 &MousePicker::getCurrentRay() const {
    return currentRay;
}

glm::vec3 MousePicker::calculateMouseRay() {
    double mouseX, mouseY;
    glfwGetCursorPos(state->window, &mouseX, &mouseY);

    glm::vec2 ray_nds = getNormalizedDeviceCoord(mouseX, mouseY);
    glm::vec4 ray_clip = glm::vec4(ray_nds.x, ray_nds.y, -1.0f, 1.0f);
    glm::mat4 invProjMat = glm::inverse(state->getProjection());
    glm::vec4 eyeCoords = invProjMat * ray_clip;
    eyeCoords = glm::vec4(eyeCoords.x, eyeCoords.y, -1.0f, 0.0f);
    glm::mat4 invViewMat = glm::inverse(state->camera->getViewMatrix());
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

glm::vec4 MousePicker::toEyeCoords(glm::vec4 &clipCoords) {
    glm::mat4 invertedProjection = glm::inverse(state->getProjection());
    glm::vec4 eyeCoords = invertedProjection * clipCoords;
    return glm::vec4(eyeCoords.x, eyeCoords.y, -1.0f, 0.0f);
}

glm::vec3 MousePicker::toWorldCoords(glm::vec4 &eyeCoords) {
    glm::mat4 invertedView = glm::inverse(state->camera->getViewMatrix());
    glm::vec4 rayWorld = invertedView * eyeCoords;
    glm::vec3 mouseRay = glm::vec3(rayWorld.x, rayWorld.y, rayWorld.z);
    return glm::normalize(mouseRay);
}

void MousePicker::update() {
    currentRay = calculateMouseRay();
}
