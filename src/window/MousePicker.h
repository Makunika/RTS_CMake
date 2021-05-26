//
// Created by pshbl on 24.05.2021.
//

#ifndef RTS_MOUSEPICKER_H
#define RTS_MOUSEPICKER_H

#include <glm/glm.hpp>
#include "../state/State.h"
#include <vector>

class MousePicker {
private:
    glm::vec3 currentRay;
    std::vector<glm::vec3> currentRaysArea;
    State* state;

public:
    MousePicker(State *state);
    const glm::vec3 &getCurrentRay() const;
    const std::vector<glm::vec3> &getCurrentRaysArea() const;
    void update();
    void updateArea(double lastX, double lastY);
    glm::vec2 getNormalizedDeviceCoord(float mouseX, float mouseY);

private:
    glm::vec3 calculateMouseRay(double mouseX, double mouseY);


};


#endif //RTS_MOUSEPICKER_H
