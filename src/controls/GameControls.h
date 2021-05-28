//
// Created by pshbl on 28.05.2021.
//

#ifndef RTS_GAMECONTROLS_H
#define RTS_GAMECONTROLS_H


#include "../state/GameState.h"
#include <glm/gtx/rotate_vector.hpp>

class GameControls {
    GameState* gameState;
    bool check;
    bool check2;
    double lastX, lastY;
    Shader* selectQuadShader;
    VAO_VBO vPoint;
    glm::vec3 rayMoveTo;

public:
    GameControls(GameState *gameState);
    void processMouse();

private:
    void processSelecting();
    void processMoved();
};


#endif //RTS_GAMECONTROLS_H
