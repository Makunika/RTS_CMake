
#include "window/Window.h"

// The MAIN function, from here we start the application and run the game loop
//int main2(int argc, char* argv[])
//{
//    std::cout << argv[0];
//    // Init GLFW
//    glfwInit();
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
//    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
//    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
//
//
//    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "LearnOpenGL", nullptr, nullptr);
//    glfwMakeContextCurrent(window);
//
//    glfwSetCursorPosCallback(window, mouse_callback);
//    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
//    glfwSetScrollCallback(window, scroll_callback);
//
//    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
//    {
//        std::cout << "Failed to initialize GLAD" << std::endl;
//        return -1;
//    }
//
//    glViewport(0, 0, WIDTH, HEIGHT);
//
//    glEnable(GL_DEPTH_TEST);
//
//    Shader lightingShader("test");
//    Shader skyboxShader("skybox");
//    Shader lightCubeShader("light");
//    Shader containerShader("container");
//
//
//    vector<float> vertices = ShaderUtils::getCube(1);
//    VAO_VBO vao_vbo_container = ShaderUtils::loadTextureAndCoordinate(&vertices[0], vertices.size());
//
//    vector<float> vertices_light = ShaderUtils::getCubeWithNormalAndTexture();
//    VAO_VBO vao_vbo_light = ShaderUtils::load2vec3And1vec2(&vertices_light[0], vertices_light.size());
//
//
//    glm::vec3 cubePositions[] = {
//          glm::vec3(0.0f,  0.0f,  0.0f),
//          glm::vec3(22.0f,  5.0f, 15.0f),
//          glm::vec3(-1.5f, -2.2f, -2.5f),
//          glm::vec3(-3.8f, -2.0f, -12.3f),
//          glm::vec3(2.4f, -0.4f, -3.5f),
//          glm::vec3(-1.7f,  3.0f, -7.5f),
//          glm::vec3(1.3f, -2.0f, -2.5f),
//          glm::vec3(1.5f,  2.0f, -2.5f),
//          glm::vec3(1.5f,  0.2f, -1.5f),
//          glm::vec3(-0.3f,  2.0f, -1.5f)
//    };
//
//    vector<float> vertices2
//	{
//        5.0f, -0.5f,  5.0f,  100.0f, 00.0f,
//       -5.0f, -0.5f,  5.0f,  0.0f, 0.0f,
//       -5.0f, -0.5f, -5.0f,  0.0f, 100.0f,
//
//        5.0f, -0.5f,  5.0f,  100.0f, 0.0f,
//       -5.0f, -0.5f, -5.0f,  0.0f, 100.0f,
//        5.0f, -0.5f, -5.0f,  100.0f, 100.0f
//    };
//    VAO_VBO vao_vbo_pol = ShaderUtils::loadTextureAndCoordinate(&vertices2[0], vertices2.size());
//
//    Texture textureContainer = TextureOne("container.jpg");
//    Texture textureContainer2 = TextureOne("container_2.png");
//
//    const vector<std::string> skybox
//    {
//        "sky/right.jpg",
//        "sky\\left.jpg",
//        "sky\\top.jpg",
//        "sky\\bottom.jpg",
//        "sky\\front.jpg",
//        "sky\\back.jpg"
//    };
//
//    Texture skyboxTexture = TextureCubeMap(skybox);
//
//    vector<float> skyboxVertices = ShaderUtils::getCube();
//    VAO_VBO vao_vbo_skybox = ShaderUtils::loadOnlyCoordinate(&skyboxVertices[0], skyboxVertices.size());
//
//    stbi_set_flip_vertically_on_load(true);
//
//    // Компилирование нашей шейдерной программы
//    Shader modelShader("model_test");
//
//    // Загрузка моделей
//    //Model ourModel(R"(D:\oldold\code\vs\RTS\RTS\resources\objects\backpack\backpack.obj)");
//
//
//
//
//    glm::mat4 model = glm::mat4(1.0f); // сначала инициализируем единичную матрицу
//    glm::mat4 projection = glm::mat4(1.0f);
//    projection = glm::perspective(glm::radians(45.0f), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);
//
//
//    lightingShader.use();
//    lightingShader.setInt("material.diffuse", 0);
//
//    while (!glfwWindowShouldClose(window))
//    {
//	    const float currentFrame = glfwGetTime();
//        deltaTime = currentFrame - lastFrame;
//        lastFrame = currentFrame;
//        processInput(window);
//
//
//        // Рендеринг
//        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
//        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
//        glDepthMask(GL_FALSE);
//        //skyboxShader.use();
//        glm::mat4 view = glm::mat4(glm::mat3(camera.getViewMatrix()));
//        skyboxShader.setProjectionAndView(projection, view);
//        /*glBindVertexArray(vao_vbo_skybox.VAO);
//        glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTexture.getTexture());
//        glDrawArrays(GL_TRIANGLES, 0, 36);
//        */
//        glDepthMask(GL_TRUE);
//
//        glBindTexture(GL_TEXTURE_2D, textureContainer.getTexture());
//        glBindVertexArray(vao_vbo_container.VAO);
//        containerShader.use();
//        view = camera.getViewMatrix();
//        containerShader.setProjectionAndView(projection, view);
//        for (int i = 0; i < 8; i++)
//        {
//            glm::mat4 model = glm::mat4(1.0f);
//            model = glm::translate(model, cubePositions[i]);
//            float angle = glm::radians(20.0f * i);
//            if (i == 0 || i % 2 == 0) {
//                angle = sin(glfwGetTime());
//            }
//            model = glm::rotate(model, angle, glm::vec3(1.0f, 0.3f, 0.5f));
//            containerShader.setMatrix4("model", model);
//            glDrawArrays(GL_TRIANGLES, 0, 36);
//        }
//
//
//        //ourShader.use();
//        //view = camera.getViewMatrix();
//        //ourShader.setProjectionAndView(projection, view);
//        //glBindVertexArray(vao_vbo_light.VAO);
//        //glm::mat4 model = glm::mat4(1.0f);
//        //model = glm::translate(model, cubePositions[8]);
//        ////float angle = glm::radians(20.0f * 8);
//        //// model = glm::rotate(model, angle, glm::vec3(1.0f, 0.3f, 0.5f));
//        //ourShader.setMatrix4("model", model);
//        //ourShader.setVec3("viewPos", camera.Position);
//        //ourShader.setVec3("material.ambient", 1.0f, 0.5f, 0.31f);
//        //ourShader.setVec3("material.diffuse", 1.0f, 0.5f, 0.31f);
//        //ourShader.setVec3("material.specular", 0.5f, 0.5f, 0.5f);
//        //ourShader.setFloat("material.shininess", 32.0f);
//        //ourShader.setInt("material.diffuse", 0);
//
//        lightingShader.use();
//        lightingShader.setVec3("light.position", cubePositions[9]);
//        lightingShader.setVec3("viewPos", camera.Position);
//
//        // Свойства света
//        lightingShader.setVec3("light.ambient", 0.2f, 0.2f, 0.2f);
//        lightingShader.setVec3("light.diffuse", 0.5f, 0.5f, 0.5f);
//        lightingShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
//
//        // Свойства материалов
//        lightingShader.setVec3("material.specular", 0.5f, 0.5f, 0.5f);
//        lightingShader.setFloat("material.shininess", 64.0f);
//        lightingShader.setInt("material.diffuse", 0);
//
//        // Преобразования Вида/Проекции
//        view = camera.getViewMatrix();
//        lightingShader.setProjectionAndView(projection, view);
//
//        // Мировое преобразование
//        glm::mat4 model = glm::mat4(1.0f);
//        lightingShader.setMatrix4("model", model);
//
//        // Связывание диффузной карты
//        glActiveTexture(GL_TEXTURE0);
//        glBindTexture(GL_TEXTURE_2D, textureContainer.getTexture());
//
//        // Рендеринг куба
//        glBindVertexArray(vao_vbo_light.VAO);
//        glDrawArrays(GL_TRIANGLES, 0, 36);
//
//       /* glm::vec3 lightColor;
//        lightColor.x = sin(glfwGetTime() * 2.0f);
//        lightColor.y = sin(glfwGetTime() * 0.7f);
//        lightColor.z = sin(glfwGetTime() * 1.3f);
//        cubePositions[9].x = 1.0f + sin(glfwGetTime() * 2.0f);
//
//        glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f);
//        glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f);
//
//        ourShader.setVec3("light.ambient", ambientColor);
//        ourShader.setVec3("light.diffuse", diffuseColor);
//
//        ourShader.setVec3("light.specular", 1.0f, 1.0f, 1.0f);
//        ourShader.setVec3("light.position", cubePositions[9]);
//        glDrawArrays(GL_TRIANGLES, 0, 36);*/
//
//        //glBindTexture(GL_TEXTURE_2D, textureContainer.getTexture());
//
//    	//свет
//        lightCubeShader.use();
//        view = camera.getViewMatrix();
//        lightCubeShader.setProjectionAndView(projection, view);
//        model = glm::mat4(1.0f);
//        model = glm::translate(model, cubePositions[9]);
//        //angle = glm::radians(20.0f * 9);
//        //model = glm::rotate(model, angle, glm::vec3(1.0f, 0.3f, 0.5f));
//        model = glm::scale(model, glm::vec3(0.2f));
//        lightCubeShader.setMatrix4("model", model);
//        glDrawArrays(GL_TRIANGLES, 0, 36);
//
//    	//пол
//        containerShader.use();
//        view = camera.getViewMatrix();
//        containerShader.setProjectionAndView(projection, view);
//        glBindVertexArray(vao_vbo_pol.VAO);
//        model = glm::mat4(1.0f);
//        model = glm::scale(model, glm::vec3(20.0f, 1.0f, 20.0f));
//        containerShader.setMatrix4("model", model);
//        glDrawArrays(GL_TRIANGLES, 0, 6);
//        glBindVertexArray(0);
//        /*
//        // Рендеринг загруженной модели
//        modelShader.use();
//        view = camera.getViewMatrix();
//        modelShader.setProjectionAndView(projection, view);
//        model = glm::mat4(1.0f);
//        model = glm::translate(model, glm::vec3(10.0f, 10.0f, 10.0f)); // смещаем вниз чтобы быть в центре сцены
//        model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));	// объект слишком большой для нашей сцены, поэтому немного уменьшим его
//        modelShader.setMatrix4("model", model);
//        ourModel.draw(modelShader);
//        */
//        // glfw: обмен содержимым front- и back- буферов. Отслеживание событий ввода\вывода (была ли нажата/отпущена кнопка, перемещен курсор мыши и т.п.)
//        //cubePositions[9].x = -0.3f + sin(time(NULL)) * 3;
//    	glfwSwapBuffers(window);
//        glfwPollEvents();
//    }
//    vao_vbo_light.disponse();
//    vao_vbo_pol.disponse();
//    // Terminate GLFW, clearing any resources allocated by GLFW.
//    glfwTerminate();
//    return 0;
//}

int main(int argc, char* argv[])
{
    std::cout << argv[0] << endl;
    Window* window = new Window(1300, 800, "RTS");
    window->run();
    return 0;
}
