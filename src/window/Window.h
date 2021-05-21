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

using namespace std;

class Window
{
private:

	State* state;
	Controls* controls;
    GLFWwindow* window;

public:
	Window(int width, int height, string top)
	{
		//init glfw
		glfwInit();
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
		glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
		window = glfwCreateWindow(width, height, top.c_str(), nullptr, nullptr);
		glfwMakeContextCurrent(window);

		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
		{
			throw runtime_error("Failed to initialize GLAD");
		}
		glViewport(0, 0, width, height);

		glEnable(GL_DEPTH_TEST);

		state = new State();
		state->width = width;
		state->height = height;
		state->camera = new Camera(glm::vec3(0.0f, 0.0f, 3.0f));
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

        vector<float> v = ShaderUtils::getCubeWithNormalAndTexture();
        VAO_VBO vCube = ShaderUtils::load2vec3And1vec2(&v[0], v.size());
        VAO_VBO vLight = ShaderUtils::load2vec3And1vec2(&v[0], v.size());

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
        Model* modelO = new Model(Utils::getResourcesPathString() + "objects\\backpack\\backpack.obj");

        glm::mat4 projection = glm::perspective(glm::radians(state->camera->Zoom), (float)state->width / (float)state->height, 0.1f, 100.0f);
		
        while (!glfwWindowShouldClose(window))
        {
            // Логическая часть работы со временем для каждого кадра
            float currentFrame = glfwGetTime();
            state->deltaTime = currentFrame - state->lastFrame;
            state->lastFrame = currentFrame;

            // Обработка ввода
            processInput();
        	
            // Рендеринг
            glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            // Убеждаемся, что активировали шейдер прежде, чем настраивать uniform-переменные/объекты_рисования
            lightingShader.use();
            //lightingShader.setVec3("light.direction", -0.2f, -1.0f, -0.3f);

            //lightingShader.setVec3("light.position", lightPos);
            lightingShader.setVec3("viewPos", state->camera->Position);

            // Свойства света
            //Направленный свет
            lightingShader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
            lightingShader.setVec3("dirLight.ambient", 0.1f, 0.1f, 0.1f);
            lightingShader.setVec3("dirLight.diffuse", 0.3f, 0.3f, 0.3f);
            lightingShader.setVec3("dirLight.specular", 1.0f, 1.0f, 1.0f);
            //Точечный свет
            for (int i = 0; i < 4; ++i) {
                string point = "pointLights[" + to_string(i) + "]";
                lightingShader.setVec3(point + ".ambient", 0.0f, 0.0f, 0.0f);
                lightingShader.setVec3(point + ".diffuse", 0.2f, 0.2f, 0.2f);
                lightingShader.setVec3(point + ".specular", 1.0f, 1.0f, 1.0f);
                lightingShader.setFloat(point + ".constant", 1.0f);
                lightingShader.setFloat(point + ".linear", 0.09f);
                lightingShader.setFloat(point + ".quadratic", 0.032f);
                lightingShader.setVec3(point + ".position", pointLightPositions[i]);
            }
            //Прожектор
            lightingShader.setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
            lightingShader.setVec3("spotLight.diffuse", 0.5f, 0.5f, 0.5f);
            lightingShader.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
            lightingShader.setVec3("spotLight.position", state->camera->Position);
            lightingShader.setVec3("spotLight.direction", state->camera->Front);
            lightingShader.setFloat("spotLight.constant", 1.0f);
            lightingShader.setFloat("spotLight.linear", 0.09f);
            lightingShader.setFloat("spotLight.quadratic", 0.032f);
            lightingShader.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
            lightingShader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(17.5f)));

            // Свойства материалов
            lightingShader.setVec3("material.specular", 0.5f, 0.5f, 0.5f);
            lightingShader.setFloat("material.shininess", 64.0f);

            // Преобразования Вида/Проекции
            glm::mat4 view = state->camera->getViewMatrix();
            lightingShader.setMatrix4("projection", projection);
            lightingShader.setMatrix4("view", view);


            // Связывание диффузной карты
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, diffuseMap);
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, specMap);
            glBindVertexArray(vCube.VAO);
            for(unsigned int i = 0; i < 10; i++)
            {
                glm::mat4 model = glm::mat4(1.0f);
                model = glm::translate(model, cubePositions[i]);
                float angle = 20.0f * i;
                model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
                lightingShader.setMatrix4("model", model);
                glDrawArrays(GL_TRIANGLES, 0, 36);
            }

            lightCubeShader.use();
            lightCubeShader.setMatrix4("projection", projection);
            lightCubeShader.setMatrix4("view", view);
            glBindVertexArray(vLight.VAO);
            // Также отрисовываем наш объект-"лампочку"
            for (const auto &item : pointLightPositions) {
                glm::mat4 model = glm::mat4(1.0f);
                model = glm::translate(model, item);
                model = glm::scale(model, glm::vec3(0.2f)); // куб меньшего размера
                lightCubeShader.setMatrix4("model", model);
                glDrawArrays(GL_TRIANGLES, 0, 36);
            }

            //модель
            modelShader.use();

            //настраиваем свет
            modelShader.setVec3("viewPos", state->camera->Position);
            /*modelShader.setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
            modelShader.setVec3("dirLight.ambient", 0.2f, 0.2f, 0.2f);
            modelShader.setVec3("dirLight.diffuse", 0.5f, 0.5f, 0.5f);
            modelShader.setVec3("dirLight.specular", 1.0f, 1.0f, 1.0f);
            //Точечный свет
            for (int i = 0; i < 4; ++i) {
                string point = "pointLights[" + to_string(i) + "]";
                modelShader.setVec3(point + ".ambient", 0.2f, 0.2f, 0.2f);
                modelShader.setVec3(point + ".diffuse", 0.5f, 0.5f, 0.5f);
                modelShader.setVec3(point + ".specular", 1.0f, 1.0f, 1.0f);
                modelShader.setFloat(point + ".constant", 1.0f);
                modelShader.setFloat(point + ".linear", 0.09f);
                modelShader.setFloat(point + ".quadratic", 0.032f);
                modelShader.setVec3(point + ".position", pointLightPositions[i]);
            }

            modelShader.setVec3("spotLight.ambient", 0.2f, 0.2f, 0.2f);
            modelShader.setVec3("spotLight.diffuse", 0.5f, 0.5f, 0.5f);
            modelShader.setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
            modelShader.setVec3("spotLight.position", state->camera->Position);
            modelShader.setVec3("spotLight.direction", state->camera->Front);
            modelShader.setFloat("spotLight.constant", 1.0f);
            modelShader.setFloat("spotLight.linear", 0.09f);
            modelShader.setFloat("spotLight.quadratic", 0.032f);
            modelShader.setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
            modelShader.setFloat("spotLight.outerCutOff", glm::cos(glm::radians(17.5f)));*/

            modelShader.setVec3("pointLight.ambient", 0.2f, 0.2f, 0.2f);
            modelShader.setVec3("pointLight.diffuse", 0.5f, 0.5f, 0.5f);
            modelShader.setVec3("pointLight.specular", 1.0f, 1.0f, 1.0f);
            modelShader.setFloat( "pointLight.constant", 1.0f);
            modelShader.setFloat( "pointLight.linear", 0.09f);
            modelShader.setFloat( "pointLight.quadratic", 0.032f);
            modelShader.setVec3("pointLight.position", pointLightPositions[3]);

            modelShader.setFloat("shininess", 64.0f);


            view = state->camera->getViewMatrix();
            modelShader.setMatrix4("projection", projection);
            modelShader.setMatrix4("view", view);

            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, glm::vec3(0.0f, 0.0f, 1.0f)); // смещаем вниз чтобы быть в центре сцены
            model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));	// объект слишком большой для нашей сцены, поэтому немного уменьшим его
            modelShader.setMatrix4("model", model);
            modelO->draw(modelShader);

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
    }
};