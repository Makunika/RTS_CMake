#pragma once
#include <iostream>
#include <string>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "../camera/Camera.h"
#include "../controls/Controls.h"
#include "../shaders/ShaderUtils.h"
#include "../state/State.h"
#include "../textures/TextureOne.h"
#include "../utils/Utils.h"
#include "../shaders/Shader.h"
#include "../models/Model.h"
#include "../game/Tank.h"
#include "MousePicker.h"

using namespace std;

class Window
{
private:
	State* state;
	Controls* controls;
    GLFWwindow* window;
    MousePicker* mousePicker;

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
		
		glfwSetWindowSizeCallback(window, resizeCallback);
		glfwSetMouseButtonCallback(window, mouseButtonCallback);
		glfwSetCursorPosCallback(window, mouseCallback);
		glfwSetScrollCallback(window, scrollCallback);
	}

	void run()
	{
        Shader lightingShader("object");
        Shader lightCubeShader("light");
        Shader modelShader("model_test");
        Shader singleColor("singleColor");
        Shader screenShader("screenShader");
        screenShader.use();
        screenShader.setInt("screenTexture", 0);

        vector<float> v = ShaderUtils::getCubeWithNormalAndTexture();
        VAO_VBO vCube = ShaderUtils::load2vec3And1vec2(&v[0], v.size());
        VAO_VBO vLight = ShaderUtils::load2vec3And1vec2(&v[0], v.size());

        vector<float> q = ShaderUtils::getQuad();
        VAO_VBO vScreen = ShaderUtils::load2vec2(&q[0], q.size());

        glm::vec3 lightPos(1.2f, 1.0f, 2.0f);

        glm::vec3 cubePositions[] = {
                glm::vec3( 0.0f,  0.0f,  0.0f),
                glm::vec3( 2.0f,  5.0f, -15.0f),
                glm::vec3(-1.5f, -2.2f, -2.5f),
                glm::vec3(-3.8f, -2.0f, -12.3f),
                glm::vec3( 2.4f, -0.4f, -3.5f),
                glm::vec3(-1.7f,  3.0f, -7.5f),
                glm::vec3( 1.3f, -2.0f, -2.5f),
                glm::vec3( 1.5f,  2.0f, -2.5f),
                glm::vec3( 1.5f,  0.2f, -1.5f),
                glm::vec3(-1.3f,  1.0f, -1.5f)
        };

        glm::vec3 pointLightPositions[] = {
                glm::vec3( 0.7f,  0.2f,  2.0f),
                glm::vec3( 2.3f, -3.3f, -4.0f),
                glm::vec3(-4.0f,  2.0f, -12.0f),
                glm::vec3( 0.0f,  0.0f, -3.0f)
        };

        // Загрузка текстур
        unsigned int diffuseMap = TextureOne("container_2.png").getTexture();
        unsigned int specMap = TextureOne("container_spec.png").getTexture();

        // Конфигурация шейдеров
        lightingShader.use();
        lightingShader.setInt("material.diffuse", 0);
        lightingShader.setInt("material.specular", 1);

//        lightingShader.setFloat("light.constant", 1.0f);
//        lightingShader.setFloat("light.linear", 0.09f);
//        lightingShader.setFloat("light.quadratic", 0.032f);

        stbi_set_flip_vertically_on_load(true);
        //Model* modelO = new Model(Utils::getResourcesPathString() + "objects\\backpack\\backpack.obj");
        vector<Tank*> tanks;
        tanks.push_back(new Tank(0.0f, 0.0f, 0, 0.2f, state));
        tanks.push_back(new Tank(1.0f, 1.0f, 0, 0.2f, state));
        glm::vec2 point = glm::vec2(5.0f, 5.0f);
        tanks.at(1)->moveTo(1.0f / 60, point);



        unsigned int framebuffer;
        glGenFramebuffers(1, &framebuffer);
        glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

        unsigned int textureColorbuffer = initTextureFrameBuffer();
        unsigned int rbo = initObjectFramebuffer();

        if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
            cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << endl;
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        mousePicker = new MousePicker(state);

        while (!glfwWindowShouldClose(window))
        {
            // Логическая часть работы со временем для каждого кадра
            float currentFrame = glfwGetTime();
            state->deltaTime = currentFrame - state->lastFrame;
            state->lastFrame = currentFrame;

            // Обработка ввода
            processInput();
            //std::cout << m.x << " " << m.y << " " << m.z <<  std::endl;

            if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
                mousePicker->update();
                glm::vec3 ray = mousePicker->getCurrentRay();
                Tank* lastTank = nullptr;
                float dist;
                float minDist = 1000000000;
                for (const auto &item : tanks)  {
                    item->setSelected(false);
                    if (item->intersectionRay(ray, state->camera->Position, dist)) {
                        if (dist < minDist) {
                            minDist = dist;
                            if (lastTank != nullptr) {
                                lastTank->setSelected(false);
                            }
                            lastTank = item;
                            item->setSelected(true);
                        }
                    }
                }
            }
            if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS) {
                mousePicker->update();
                glm::vec3 ray = mousePicker->getCurrentRay();
                glm::vec3 N = glm::vec3(0.0f, 1.0f, 0.0f);
                glm::vec3 p = glm::vec3(1.0f, 0.0f, 1.0f);
                const float eps = 1.0e-5f;
                float ratio = glm::dot(N, ray);   // косинус нормали с лучом
               // if (fabs(ratio) < eps) return -1.0f;  // луч параллелен плоскости
                float d = glm::dot(N, p - state->camera->Position);  // расстояние от плоскости до rayPos по нормали
                float t =  d / ratio;     // возвращаем расстояние по лучу

                glm::vec3 inters;
                if (t >= 0.0f) {
                    inters = state->camera->Position + ray * t; // находим точку пересечения
                    for (const auto &item : tanks) {
                        if (item->getIsSelected()) {
                            glm::vec2 point = glm::vec2(inters.x, inters.z);
                            item->moveTo(1.0f / 60, point);
                        }
                    }
                }
            }

            for (const auto &item : tanks) {
                item->move();
            }


            // Рендеринг
            //glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
            glEnable(GL_DEPTH_TEST);
            glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
            glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

            glStencilMask(0x00); // убеждаемся, что мы не обновляем буфер трафарета во время рисования пола

            glm::mat4 projection = state->getProjection();
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
            singleColor.use();
            glm::mat4 view = state->camera->getViewMatrix();
            singleColor.setMatrix4("projection", projection);
            singleColor.setMatrix4("view", view);
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(0.0f, 1.0f, -2.5f)); // смещаем вниз чтобы быть в центре сцены
            model = glm::scale(model, glm::vec3(0.21f));	// объект слишком большой для нашей сцены, поэтому немного уменьшим его
            singleColor.setMatrix4("model", model);

            //модель
            modelShader.use();

            //настраиваем свет
            modelShader.setVec3("viewPos", state->camera->Position);
            modelShader.setVec3("pointLight.ambient", 0.0f, 0.0f, 0.0f);
            modelShader.setVec3("pointLight.diffuse", 1.5f, 1.5f, 1.5f);
            modelShader.setVec3("pointLight.specular", 1.0f, 1.0f, 1.0f);
            modelShader.setFloat( "pointLight.constant", 1.0f);
            modelShader.setFloat( "pointLight.linear", 0.09f);
            modelShader.setFloat( "pointLight.quadratic", 0.032f);
            modelShader.setVec3("pointLight.position", pointLightPositions[3]);

            modelShader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
            modelShader.setVec3("dirLight.ambient", 0.2f, 0.2f, 0.2f);
            modelShader.setVec3("dirLight.diffuse", 0.5f, 0.5f, 0.5f);
            modelShader.setVec3("dirLight.specular", 1.0f, 1.0f, 1.0f);

            modelShader.setVec3("spotLight.ambient", 0.2f, 0.2f, 0.2f);
            modelShader.setVec3("spotLight.diffuse", 2.5f, 2.5f, 2.5f);
            modelShader.setVec3("spotLight.specular", 3.0f, 3.0f, 3.0f);
            modelShader.setVec3("spotLight.position", state->camera->Position);
            modelShader.setVec3("spotLight.direction", state->camera->Front);
            modelShader.setFloat("spotLight.constant", 1.0f);
            modelShader.setFloat("spotLight.linear", 0.09f);
            modelShader.setFloat("spotLight.quadratic", 0.032f);
            modelShader.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
            modelShader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(17.5f)));

            modelShader.setFloat("shininess", 64.0f);

            view = state->camera->getViewMatrix();
            modelShader.setMatrix4("projection", projection);
            modelShader.setMatrix4("view", view);
            for (const auto &item : tanks) {
                modelShader.use();
                item->draw(modelShader, singleColor);
            }


//            model = glm::mat4(1.0f);
//            model = glm::translate(model, glm::vec3(0.0f, 1.0f, -2.5f)); // смещаем вниз чтобы быть в центре сцены
//            model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));	// объект слишком большой для нашей сцены, поэтому немного уменьшим его
//            modelShader.setMatrix4("model", model);
//            modelO->drawSelected(modelShader, singleColor);
//
//            modelShader.use();
//            model = glm::mat4(1.0f);
//            model = glm::translate(model, glm::vec3(0.0f, 1.0f, -4.5f)); // смещаем вниз чтобы быть в центре сцены
//            model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));	// объект слишком большой для нашей сцены, поэтому немного уменьшим его
//            modelShader.setMatrix4("model", model);
//
//            singleColor.use();
//            model = glm::mat4(1.0f);
//            model = glm::translate(model, glm::vec3(0.0f, 1.0f, -4.5f)); // смещаем вниз чтобы быть в центре сцены
//            model = glm::scale(model, glm::vec3(0.21f));	// объект слишком большой для нашей сцены, поэтому немного уменьшим его
//            singleColor.setMatrix4("model", model);
//            modelO->drawSelected(modelShader, singleColor);
            glBindVertexArray(0);

            //все отрисовали
//            glBindFramebuffer(GL_FRAMEBUFFER, 0);
//            glDisable(GL_DEPTH_TEST);
//
//            glClearColor(1.0f, 1.0f, 1.0f, 1.0f); // устанавливаем цвет заливки на "белый" (установите прозрачный цвет на белый (на самом деле это не обязательно, так как мы все равно не сможем видеть пространство за прямоугольником))
//            glClear(GL_COLOR_BUFFER_BIT);
//            screenShader.use();
//            glBindVertexArray(vScreen.VAO);
//            glBindTexture(GL_TEXTURE_2D, textureColorbuffer); // используем прикрепленную цветовую текстуру в качестве текстуры для прямоугольника
//            glDrawArrays(GL_TRIANGLES, 0, 6);

            // glfw: обмен содержимым front- и back- буферов. Отслеживание событий ввода/вывода (была ли нажата/отпущена кнопка, перемещен курсор мыши и т.п.)
            glfwSwapBuffers(window);
            glfwPollEvents();
        }

        // Опционально: освобождаем все ресурсы, как только они выполнили свое предназначение
        vLight.disponse();
        vCube.disponse();
        // glfw: завершение, освобождение всех выделенных ранее GLFW-реcурсов
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