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

using namespace std;


class Game {
    GameState* gameState;
    LightState* lightState;
    GameControls* gameControls;
    SpotLight* spotLight;
    Floor* floor;

public:
    Game(State* state);

    void update();
    void draw();
    void processInput();

    GameState *getGameState() const;

    LightState *getLightState() const;

    GameControls *getGameControls() const;

private:
    void initScene();
    void initLight();
    void initObjects();
    void initShaders();
    void initFloor();
};


#endif //RTS_GAME_H
