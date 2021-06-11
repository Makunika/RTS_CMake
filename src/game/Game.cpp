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

    lightState->dirLight = new DirLight(
            glm::vec3(0.2f),
            glm::vec3(0.5f),
            glm::vec3(1.0f),
            glm::vec3(-0.5f, -1.0f, -0.5f)
            );

//свет для ночи
//    DirLight* dirLight = new DirLight(
//            glm::vec3(0.0f),
//            glm::vec3(0.486, 0.419, 1) * 0.1f,
//            glm::vec3(0.486, 0.419, 1) * 0.2f,
//            glm::vec3(-0.5f, -1.0f, -0.5f)
//    );
    lightState->lights.push_back(lightState->dirLight);

    for (const auto &tank : gameState->tanks) {
        lightState->lights.push_back(tank->lightLeft);
    }

    for (const auto &home : gameState->homes) {
        if (home->pointLight != nullptr) {
            lightState->lights.push_back(home->pointLight);
        }
    }

    for (const auto &lamp : gameState->lamps) {
        lightState->lights.push_back(lamp->pointLight);
    }
}

void Game::initObjects() {
    //танки
    gameState->addTank(new Tank(0.0f, 1.0f, 0, gameState->state));
    gameState->addTank(new Tank(1.0f, 1.0f, 0, gameState->state));
    gameState->addTank(new Tank(2.0f, 1.0f, 0, gameState->state));
    gameState->addTank(new Tank(3.0f, 1.0f, 0, gameState->state));
    gameState->addTank(new Tank(4.0f, 1.0f, 0, gameState->state));
    gameState->addTank(new Tank(5.0f, 1.0f, 0, gameState->state));
    gameState->addTank(new Tank(6.0f, 1.0f, 0, gameState->state));
    gameState->addTank(new Tank(7.0f, 1.0f, 0, gameState->state));

    //деревья по периметру
    for (float i = -20; i < 20; i+=1.5f) {
        for (float j = -1; j > -6; j-=1.5f) {
            gameState->addTree(new Tree(i, j, gameState->state), j == -1);
        }
    }

    for (float i = -20; i < -13; i+=1.5f) {
        for (float j = -1; j < 20; j +=1.5f) {
            gameState->addTree(new Tree(i, j, gameState->state), i > -12);
        }
    }

    for (float i = -20; i < 20; i+=1.5f) {
        for (float j = 15; j < 20; j+=1.5f) {
            gameState->addTree(new Tree(i, j, gameState->state), j == 15);
        }
    }

    for (float i = 15; i < 20; i +=1.5f) {
        for (float j = -1; j < 20; j +=1.5f) {
            gameState->addTree(new Tree(i, j, gameState->state), j == -1);
        }
    }

    //дома
    gameState->addHome(new Home(10.0f, 8.0f, gameState->state, HOUSE3, 225.0f, true));
    gameState->addHome(new Home(5.0f, 8.0f, gameState->state, HOUSE3, 90.0f + 180.0f, true));
    gameState->addHome(new Home(-10.0f, 3.0f, gameState->state, HOUSE1, 245.0f));
    gameState->addHome(new Home(10.0f, 4.0f, gameState->state, HOUSE2, 180.0f));
    gameState->addHome(new Home(-9.0f, 9.0f, gameState->state, HOUSE3, 0.0f, true));
    gameState->addHome(new Home(-8.0f, 13.0f, gameState->state, HOUSE2, 76.0f));

    //деревья нужные
    gameState->addTree(new Tree(5, 6, gameState->state));
    gameState->addTree(new Tree(4, 5.5, gameState->state));
    gameState->addTree(new Tree(6, 6, gameState->state));
    gameState->addTree(new Tree(5, 5, gameState->state));
    gameState->addTree(new Tree(4, 5, gameState->state));

    gameState->addTree(new Tree(3, 7, gameState->state));
    gameState->addTree(new Tree(2, 8, gameState->state));

    gameState->addTree(new Tree(6, 12, gameState->state));
    gameState->addTree(new Tree(7, 13, gameState->state));
    gameState->addTree(new Tree(8, 12, gameState->state));

    gameState->addTree(new Tree(-6, 10, gameState->state));
    gameState->addTree(new Tree(-5, 13, gameState->state));
    gameState->addTree(new Tree(-5, 12, gameState->state));
    gameState->addTree(new Tree(-6, 14, gameState->state));

    gameState->addTree(new Tree(-7, 4, gameState->state));
    gameState->addTree(new Tree(-7, 5, gameState->state));
    gameState->addTree(new Tree(-6, 5, gameState->state));

    gameState->addTree(new Tree(-7, 8, gameState->state));

    //фонари
    gameState->addLamp(new Lamp(2, 5, gameState->state, 240.0f));
    gameState->addLamp(new Lamp(-10, 5, gameState->state, 0.0f));
    gameState->addLamp(new Lamp(8, 9, gameState->state, 255.0f));
    gameState->addLamp(new Lamp(-7, 9, gameState->state, 67.0f));
    gameState->addLamp(new Lamp(2, 9, gameState->state, 240.0f));
}

void Game::update() {

    if (gameState->state->isDay) {
        if (!isDay) {
            isDay = true;
            lightState->dirLight->change(
                    glm::vec3(0.2f),
                    glm::vec3(0.5f),
                    glm::vec3(1.0f));
        }
    } else {
        if (isDay) {
            lightState->dirLight->change(
                    glm::vec3(0.486, 0.419, 1) * 0.05f,
                    glm::vec3(0.486, 0.419, 1) * 0.2f,
                    glm::vec3(0.486, 0.419, 1) * 0.3f);
            isDay = false;
        }

    }

    for (const auto &tank : gameState->tanks) {
        tank->update(&gameState->staticObjects);
    }
}

void Game::draw() {

    floor->draw(lightState, shadow);

    glm::mat4 view = gameState->state->camera->getViewMatrix();
    glm::mat4 proj = gameState->state->getProjection();

    //деревья

    Tree::modelShader->use();
    Tree::modelShader->setProjectionAndView(proj, view);
    Tree::modelShader->setFloat("shininess", 64.0f);
    Tree::modelShader->setVec3("viewPos", gameState->state->camera->Position);
    lightState->allUse(Tree::modelShader);
    shadow->initShader(Tree::modelShader);

    for (const auto &tree : gameState->trees) {
        tree->draw(Tree::modelShader, nullptr);
    }

    //дома
    Home::modelShader->use();
    Home::modelShader->setProjectionAndView(proj, view);
    Home::modelShader->setFloat("shininess", 64.0f);
    Home::modelShader->setVec3("viewPos", gameState->state->camera->Position);
    lightState->allUse(Home::modelShader);
    shadow->initShader(Home::modelShader);

    for (const auto &home : gameState->homes) {
        home->draw(Home::modelShader, nullptr);
    }

    Lamp::modelShader->use();
    Lamp::modelShader->setProjectionAndView(proj, view);
    Lamp::modelShader->setFloat("shininess", 64.0f);
    Lamp::modelShader->setVec3("viewPos", gameState->state->camera->Position);
    lightState->allUse(Lamp::modelShader);
    shadow->initShader(Lamp::modelShader);

    for (const auto &lamp : gameState->lamps) {
        lamp->draw(Lamp::modelShader, nullptr);
    }

    //танки
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

    for (const auto &home : gameState->homes) {
        home->draw(shadow->shaderShadow, nullptr);
    }

    for (const auto &tank : gameState->tanks) {
        tank->draw(shadow->shaderShadow, nullptr);
    }

    for (const auto &lamp : gameState->lamps) {
        lamp->draw(shadow->shaderShadow, nullptr);
    }

    shadow->unuse();

}


