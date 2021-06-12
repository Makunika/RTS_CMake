#pragma once
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <glad/glad.h>

#include "../camera/Camera.h"
#include "../controls/Controls.h"
#include "../shaders/ShaderUtils.h"
#include "../state/State.h"
#include "../textures/TextureOne.h"
#include "../utils/Utils.h"
#include "MousePicker.h"
#include "TextRender.h"

#include "../game/Game.h"

using namespace std;

class Window
{
private:
	State* state;
	Controls* controls;
    GLFWwindow* window;
    TextRender* text;

public:
	Window(int width, int height, string top)
	{
		//init glfw
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
        glfwWindowHint(GLFW_SAMPLES, 4);
		window = glfwCreateWindow(width, height, top.c_str(), nullptr, nullptr);
		glfwMakeContextCurrent(window);

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			throw runtime_error("Failed to initialize GLAD");
		}
		glViewport(0, 0, width, height);

        glEnable(GL_DEPTH_TEST);
        glEnable(GL_STENCIL_TEST);
        glEnable(GL_MULTISAMPLE);
        glEnable(GL_CULL_FACE);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		state = new State();
		state->width = width;
		state->height = height;
		state->camera = new Camera(glm::vec3(3.0f, 10.0f, 3.0f));
		state->window = window;
		state->lastX = width / 2.0f;
		state->lastY = height / 2.0f;
		controls = new Controls(state);
		text = new TextRender(width, height, "arial.ttf");
		
		glfwSetWindowSizeCallback(window, resizeCallback);
		glfwSetMouseButtonCallback(window, mouseButtonCallback);
		glfwSetCursorPosCallback(window, mouseCallback);
		glfwSetScrollCallback(window, scrollCallback);
        glfwSetKeyCallback(window, keyCallback);
	}

	void run()
	{
        //Создаеи кобъект игры
        Game* game = new Game(state);

        while (!glfwWindowShouldClose(window))
        {
            // Логическая часть работы со временем для каждого кадра
            float currentFrame = glfwGetTime();
            state->deltaTime = currentFrame - state->lastFrame;
            state->lastFrame = currentFrame;

            glEnable(GL_DEPTH_TEST);
            glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
            glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

            glStencilMask(0x00);

            //Обновляем карту теней для игры
            game->updateShadow();

            //Обрабатываем нажатия и так далее
            processInput();

            //Обрабатываем нажатия для игры
            game->processInput();

            //Обновляем игру
            game->update();

            //Рисуем игру
            game->draw();

            glBindVertexArray(0);

            //Рисуем текст
            double x, y;
            glfwGetCursorPos(window, &x, &y);

            game->getGameState()->mousePicker->update();
            glm::vec3 ray = game->getGameState()->mousePicker->getCurrentRay();
            glm::vec3 pos = game->getGameState()->mousePicker->calculateWithArea(ray);
            text->renderText(std::to_string(x) + ":" + std::to_string(y) + "(" + std::to_string(pos.x) + ":" + std::to_string(pos.z) + ")", 40.0f, 40.0f, 0.5f, glm::vec3(1.0f));

            glfwSwapBuffers(window);
            glfwPollEvents();
        }
        glfwTerminate();
	}



private:
    void processInput()
    {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, GL_TRUE);

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            state->camera->processKeyboard(FORWARD, state->deltaTime);
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            state->camera->processKeyboard(BACKWARD, state->deltaTime);
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            state->camera->processKeyboard(LEFT, state->deltaTime);
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            state->camera->processKeyboard(RIGHT, state->deltaTime);
        if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
            state->camera->processKeyboard(ROTATE_LEFT, state->deltaTime);
        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
            state->camera->processKeyboard(ROTATE_RIGHT, state->deltaTime);

        double mouseX, mouseY;
        glfwGetCursorPos(window,&mouseX, &mouseY);
        int w, h;
        glfwGetWindowSize(window, &w, &h);
        state->camera->processMouse(w, h, mouseX, mouseY, state->deltaTime);
    }
};