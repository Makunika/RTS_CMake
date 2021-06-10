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
    initObjects();
    initLight();
    initFloor();
    initShadows();
}

void Game::initLight() {

//    PointLight* pointLight = new PointLight(
//            glm::vec3(0.0f, 0.0f, 0.0f),
//            glm::vec3(1.5f, 1.5f, 1.5f),
//            glm::vec3(1.0f, 1.0f, 1.0f),
//            glm::vec3(2.0f),
//            1.0f,
//            0.09f,
//            0.032f
//            );

    DirLight* dirLight = new DirLight(
            glm::vec3(0.1f),
            glm::vec3(1.0f),
            glm::vec3(1.5f),
            glm::vec3(-0.5f, -1.0f, -0.5f)
            );
    //lightState->lights.push_back(pointLight);
    lightState->lights.push_back(dirLight);

    for (const auto &tank : gameState->tanks) {
        lightState->lights.push_back(tank->lightLeft);
    }
}

void Game::initObjects() {
    gameState->addTank(new Tank(0.0f, 0.0f, 0, gameState->state));
    gameState->addTank(new Tank(1.0f, 1.0f, 0, gameState->state));
    gameState->addTank(new Tank(2.0f, 1.0f, 0, gameState->state));
    gameState->addTank(new Tank(3.0f, 1.0f, 0, gameState->state));
    gameState->addTank(new Tank(4.0f, 1.0f, 0, gameState->state));
    gameState->addTank(new Tank(5.0f, 1.0f, 0, gameState->state));
    gameState->addTank(new Tank(6.0f, 1.0f, 0, gameState->state));
    gameState->addTank(new Tank(7.0f, 1.0f, 0, gameState->state));

    gameState->addTree(new Tree(0.0f, -1.0f, gameState->state, TREE4));
    gameState->addTree(new Tree(2.0f, -1.0f, gameState->state, TREE3));
    gameState->addTree(new Tree(4.0f, -1.0f, gameState->state, TREE1));
    gameState->addTree(new Tree(6.0f, -1.0f, gameState->state, TREE2));
    gameState->addTree(new Tree(8.0f, -1.0f, gameState->state, TREE1));
    gameState->addTree(new Tree(10.0f, -1.0f, gameState->state, TREE1));
    gameState->addTree(new Tree(12.0f, -2.0f, gameState->state, TREE2));
//    gameState->addTree(new Tree(2.0f, -2.0f, gameState->state, TREE1));
//    gameState->addTree(new Tree(3.0f, -2.0f, gameState->state, TREE1));
//    gameState->addTree(new Tree(4.0f, -2.0f, gameState->state, TREE2));
//    gameState->addTree(new Tree(5.0f, -2.0f, gameState->state, TREE1));
//    gameState->addTree(new Tree(6.0f, -2.0f, gameState->state, TREE1));
}

void Game::update() {
    for (const auto &tank : gameState->tanks) {
        tank->update();
    }
}

void Game::draw() {

    floor->draw(lightState, shadow);

    glm::mat4 view = gameState->state->camera->getViewMatrix();
    glm::mat4 proj = gameState->state->getProjection();

    Tree::modelShader->use();
    Tree::modelShader->setProjectionAndView(proj, view);
    Tree::modelShader->setFloat("shininess", 64.0f);
    Tree::modelShader->setVec3("viewPos", gameState->state->camera->Position);
    lightState->allUse(Tree::modelShader);
    shadow->initShader(Tree::modelShader);

    for (const auto &tree : gameState->trees) {
        tree->draw(Tree::modelShader, nullptr);
    }


    Tank::modelShader->use();
    Tank::modelShader->setProjectionAndView(proj, view);
    Tank::modelShader->setFloat("shininess", 64.0f);
    Tank::modelShader->setVec3("viewPos", gameState->state->camera->Position);
    lightState->allUse(Tank::modelShader);
    shadow->initShader(Tank::modelShader);

    Tank::selectedShader->use();
    Tank::selectedShader->setProjectionAndView(proj, view);

    for (const auto &tank : gameState->tanks) {
        tank->draw(Tank::modelShader, Tank::selectedShader);
    }

    if (gameState->state->debug) {
        shadow->showDebugWindow();
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

void Game::initShadows() {
    // Настраиваем карту глубины FBO
    shadow = new Shadow(gameState->state);

}

void Game::updateShadow() {
    shadow->updateShadows(glm::vec3(-0.5f, -1.0f, -0.5f));

    shadow->use();

    floor->drawForShadow(shadow->shaderShadow);

    for (const auto &tree : gameState->trees) {
        tree->draw(shadow->shaderShadow, nullptr);
    }

    for (const auto &tank : gameState->tanks) {
        tank->draw(shadow->shaderShadow, nullptr);
    }

    shadow->unuse();

}


