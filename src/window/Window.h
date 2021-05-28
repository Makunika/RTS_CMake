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
    MousePicker* mousePicker;
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

		//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			throw runtime_error("Failed to initialize GLAD");
		}
		glViewport(0, 0, width, height);

		glEnable(GL_DEPTH_TEST);
        glEnable(GL_STENCIL_TEST);
        glEnable(GL_MULTISAMPLE);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		state = new State();
		state->width = width;
		state->height = height;
		state->camera = new Camera(glm::vec3(3.0f, 10.0f, 3.0f));//3.0f, 10.0f, 3.0f
		state->window = window;
		state->lastX = width / 2.0f;
		state->lastY = height / 2.0f;
		controls = new Controls(state);
		text = new TextRender(width, height, "arial.ttf");
		
		glfwSetWindowSizeCallback(window, resizeCallback);
		glfwSetMouseButtonCallback(window, mouseButtonCallback);
		glfwSetCursorPosCallback(window, mouseCallback);
		glfwSetScrollCallback(window, scrollCallback);
	}

	void run()
	{
        Shader screenShader("screenShader");
        screenShader.use();
        screenShader.setInt("screenTexture", 0);

        vector<float> v = ShaderUtils::getCubeWithNormalAndTexture();
        VAO_VBO vCube = ShaderUtils::load2vec3And1vec2(&v[0], v.size());
        VAO_VBO vLight = ShaderUtils::load2vec3And1vec2(&v[0], v.size());

        vector<float> q = ShaderUtils::getQuad();
        VAO_VBO vScreen = ShaderUtils::load2vec2(&q[0], q.size());

        stbi_set_flip_vertically_on_load(true);

        unsigned int framebuffer;
        glGenFramebuffers(1, &framebuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

        unsigned int textureColorbuffer = initTextureFrameBuffer();
        unsigned int rbo = initObjectFramebuffer();

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << endl;
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        Game* game = new Game(state);

        while (!glfwWindowShouldClose(window))
        {
            // Логическая часть работы со временем для каждого кадра
            float currentFrame = glfwGetTime();
            state->deltaTime = currentFrame - state->lastFrame;
            state->lastFrame = currentFrame;

            //glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
            glEnable(GL_DEPTH_TEST);
            glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
            glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

            glStencilMask(0x00); // убеждаемся, что мы не обновляем буфер трафарета во время рисования пола

            processInput();

            game->processInput();

            game->update();

            game->draw();

            glBindVertexArray(0);

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

        float cameraSpeed = 2.5f * state->deltaTime;

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

    unsigned int initObjectFramebuffer() {
        unsigned int rbo;
        glGenRenderbuffers(1, &rbo);
        glBindRenderbuffer(GL_RENDERBUFFER, rbo);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, state->width, state->height); // использование одного объекта рендербуфера для буферов глубины и трафарета
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo); // теперь прикрепляем это дело
        return rbo;
	}

    unsigned int initTextureFrameBuffer() {
	    unsigned int textureColorbuffer;
        glGenTextures(1, &textureColorbuffer);
        glBindTexture(GL_TEXTURE_2D, textureColorbuffer);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, state->width, state->height, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);
        return textureColorbuffer;
    }
};

// Убеждаемся, что активировали шейдер прежде, чем настраивать uniform-переменные/объекты_рисования
//            lightingShader.use();
//            //lightingShader.setVec3("light.direction", -0.2f, -1.0f, -0.3f);
//
//            //lightingShader.setVec3("light.position", lightPos);
//            lightingShader.setVec3("viewPos", state->camera->Position);
//
//            // Свойства света
//            //Направленный свет
//            lightingShader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
//            lightingShader.setVec3("dirLight.ambient", 0.1f, 0.1f, 0.1f);
//            lightingShader.setVec3("dirLight.diffuse", 0.3f, 0.3f, 0.3f);
//            lightingShader.setVec3("dirLight.specular", 1.0f, 1.0f, 1.0f);
//            //Точечный свет
//            for (int i = 0; i < 4; ++i) {
//                string point = "pointLights[" + to_string(i) + "]";
//                lightingShader.setVec3(point + ".ambient", 0.0f, 0.0f, 0.0f);
//                lightingShader.setVec3(point + ".diffuse", 0.2f, 0.2f, 0.2f);
//                lightingShader.setVec3(point + ".specular", 1.0f, 1.0f, 1.0f);
//                lightingShader.setFloat(point + ".constant", 1.0f);
//                lightingShader.setFloat(point + ".linear", 0.09f);
//                lightingShader.setFloat(point + ".quadratic", 0.032f);
//                lightingShader.setVec3(point + ".position", pointLightPositions[i]);
//            }
//            //Прожектор
//            lightingShader.setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
//            lightingShader.setVec3("spotLight.diffuse", 0.5f, 0.5f, 0.5f);
//            lightingShader.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
//            lightingShader.setVec3("spotLight.position", state->camera->Position);
//            lightingShader.setVec3("spotLight.direction", state->camera->Front);
//            lightingShader.setFloat("spotLight.constant", 1.0f);
//            lightingShader.setFloat("spotLight.linear", 0.09f);
//            lightingShader.setFloat("spotLight.quadratic", 0.032f);
//            lightingShader.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
//            lightingShader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(17.5f)));
//
//            // Свойства материалов
//            lightingShader.setVec3("material.specular", 0.5f, 0.5f, 0.5f);
//            lightingShader.setFloat("material.shininess", 64.0f);
//
//            // Преобразования Вида/Проекции
//            glm::mat4 view = state->camera->getViewMatrix();
//            lightingShader.setMatrix4("projection", projection);
//            lightingShader.setMatrix4("view", view);
//
//
//            // Связывание диффузной карты
//            glActiveTexture(GL_TEXTURE0);
//            glBindTexture(GL_TEXTURE_2D, diffuseMap);
//            glActiveTexture(GL_TEXTURE1);
//            glBindTexture(GL_TEXTURE_2D, specMap);
//            glBindVertexArray(vCube.VAO);
//            for(unsigned int i = 0; i < 10; i++)
//            {
//                glm::mat4 model = glm::mat4(1.0f);
//                model = glm::translate(model, cubePositions[i]);
//                float angle = 20.0f * i;
//                model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
//                lightingShader.setMatrix4("model", model);
//                glDrawArrays(GL_TRIANGLES, 0, 36);
//            }
//
//
//
//            lightCubeShader.use();
//            lightCubeShader.setMatrix4("projection", projection);
//            lightCubeShader.setMatrix4("view", view);
//            glBindVertexArray(vLight.VAO);
//            // Также отрисовываем наш объект-"лампочку"
//            for (const auto &item : pointLightPositions) {
//                glm::mat4 model = glm::mat4(1.0f);
//                model = glm::translate(model, item);
//                model = glm::scale(model, glm::vec3(0.2f)); // куб меньшего размера
//                lightCubeShader.setMatrix4("model", model);
//                glDrawArrays(GL_TRIANGLES, 0, 36);
//            }
//


//            glBindFramebuffer(GL_FRAMEBUFFER, 0);
//            glDisable(GL_DEPTH_TEST);
//
//            glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // устанавливаем цвет заливки на "белый" (установите прозрачный цвет на белый (на самом деле это не обязательно, так как мы все равно не сможем видеть пространство за прямоугольником))
//            glClear(GL_COLOR_BUFFER_BIT);
//            screenShader.use();
//            glBindVertexArray(vScreen.VAO);
//            glBindTexture(GL_TEXTURE_2D, textureColorbuffer); // используем прикрепленную цветовую текстуру в качестве текстуры для прямоугольника
//            glDrawArrays(GL_TRIANGLES, 0, 6);