//
// Created by pshbl on 22.05.2021.
//

#ifndef RTS_GAME_H
#define RTS_GAME_H

#include <vector>
#include "../state/GameState.h"
#include "../state/LightState.h"
#include "../controls/GameControls.h"
#include "../light/DirLight.h"
#include "../floor/Floor.h"
#include "../shadow/Shadow.h"

using namespace std;


class Game {
    //Состояние игры
    GameState* gameState;
    //Состояние света
    LightState* lightState;
    //Управление игрой
    GameControls* gameControls;
    //Пол
    Floor* floor;
    //Тени
    Shadow* shadow;
    //Флаг, что сейчас день
    bool isDay = true;

public:
    Game(State* state);

    void update();
    void draw();
    void processInput();
    void updateShadow();

    GameState *getGameState() const;

    LightState *getLightState() const;

    GameControls *getGameControls() const;

private:
    //Блок инициализаций
    void initScene();
    void initLight();
    void initObjects();
    void initFloor();
    void initShadows();
};


#endif //RTS_GAME_H
