//
// Created by pshbl on 22.05.2021.
//
#include "Game.h"

Game::Game(State *state) {
    gameState = new GameState();
    gameState->state = state;
    gameState->mousePicker = new MousePicker(state);
    gameControls = new GameControls(gameState);
    lightState = new LightState();
    initScene();
}

void Game::initScene() {
    initLight();
    initObjects();
    initFloor();
}

void Game::initLight() {

    PointLight* pointLight = new PointLight(
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(1.5f, 1.5f, 1.5f),
            glm::vec3(1.0f, 1.0f, 1.0f),
            glm::vec3(2.0f),
            1.0f,
            0.09f,
            0.032f
            );
    spotLight = new SpotLight(
            glm::vec3(0.2f, 0.2f, 0.2f),
            glm::vec3(2.5f, 2.5f, 2.5f),
            glm::vec3(3.0f, 3.0f, 3.0f),
            gameState->state->camera->Position,
            gameState->state->camera->Front,
            glm::cos(glm::radians(12.5f)),
            glm::cos(glm::radians(17.5f)),
            1.0f,
            0.09f,
            0.032f
            );

    DirLight* dirLight = new DirLight(
            glm::vec3(0.1f, 0.1f, 0.1f),
            glm::vec3(0.2f, 0.2f, 0.2f),
            glm::vec3(0.5f, 0.5f, 0.5f),
            glm::vec3(-0.2f, -1.0f, -0.3f)
            );
    lightState->lights.push_back(pointLight);
    lightState->lights.push_back(dirLight);
    lightState->lights.push_back(spotLight);
}

void Game::initObjects() {
    gameState->addTank(new Tank(0.0f, 0.0f, 0, 0.2f, gameState->state));
    gameState->addTank(new Tank(1.0f, 1.0f, 0, 0.2f, gameState->state));
    gameState->addTank(new Tank(2.0f, 1.0f, 0, 0.2f, gameState->state));
    gameState->addTank(new Tank(3.0f, 1.0f, 0, 0.2f, gameState->state));
    gameState->addTank(new Tank(4.0f, 1.0f, 0, 0.2f, gameState->state));
    gameState->addTank(new Tank(5.0f, 1.0f, 0, 0.2f, gameState->state));
    gameState->addTank(new Tank(6.0f, 1.0f, 0, 0.2f, gameState->state));
    gameState->addTank(new Tank(7.0f, 1.0f, 0, 0.2f, gameState->state));
}

void Game::update() {
    spotLight->setDirection(gameState->tanks.at(0)->getFront());
    spotLight->setPosition(gameState->tanks.at(0)->getPosition());
    for (const auto &tank : gameState->tanks) {
        tank->move();
    }
}

void Game::draw() {

    floor->draw(lightState);

    glm::mat4 view = gameState->state->camera->getViewMatrix();
    glm::mat4 proj = gameState->state->getProjection();

    Tank::modelShader->use();
    Tank::modelShader->setProjectionAndView(proj, view);
    Tank::modelShader->setFloat("shininess", 64.0f);
    Tank::modelShader->setVec3("viewPos", gameState->state->camera->Position);
    lightState->allUse(Tank::modelShader);

    Tank::selectedShader->use();
    Tank::selectedShader->setProjectionAndView(proj, view);

    for (const auto &tank : gameState->tanks) {
        tank->draw(Tank::modelShader, Tank::selectedShader);
    }


}

void Game::initShaders() {

}

void Game::processInput() {
    gameControls->processMouse();
}

GameState *Game::getGameState() const {
    return gameState;
}

LightState *Game::getLightState() const {
    return lightState;
}

GameControls *Game::getGameControls() const {
    return gameControls;
}

void Game::initFloor() {
    floor = new Floor(gameState->state);
}


