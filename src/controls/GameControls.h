//
// Created by pshbl on 28.05.2021.
//

#ifndef RTS_GAMECONTROLS_H
#define RTS_GAMECONTROLS_H


#include "../state/GameState.h"
#include <glm/gtx/rotate_vector.hpp>

class GameControls {
    //Игровое состояние
    GameState* gameState;
    //Можно ли выбирать
    bool check;
    //Можно ли двигать к точке
    bool check2;
    //Последние координаты мыши до нажатия
    double lastX, lastY;
    //Шейдер для отрисовки рамки
    Shader* selectQuadShader;
    //Шейдер для отрисовки линии
    Shader* lineShader;
    //VAO и VBO точки
    VAO_VBO vPoint;
    //луч куда двигать выбранные объекты
    glm::vec3 rayMoveTo;

public:
    GameControls(GameState *gameState);
    void processMouse();

private:
    //Выбор танков
    void processSelecting();
    //Куда идти выбранным танкам
    void processMoved();
};


#endif //RTS_GAMECONTROLS_H
