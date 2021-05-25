//
// Created by pshbl on 24.05.2021.
//

#ifndef RTS_MOUSEPICKER_H
#define RTS_MOUSEPICKER_H

#include <glm/glm.hpp>
#include "../state/State.h"

class MousePicker {
private:
    glm::vec3 currentRay;
    State* state;

public:
    MousePicker(State *state);
    const glm::vec3 &getCurrentRay() const;
    void update();

private:
    glm::vec3 calculateMouseRay();
    glm::vec2 getNormalizedDeviceCoord(float mouseX, float mouseY);


};


#endif //RTS_MOUSEPICKER_H
