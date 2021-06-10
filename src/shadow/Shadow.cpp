//
// Created by pshbl on 10.06.2021.
//

#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include "../resource/ResourceManager.h"
#include "Shadow.h"

Shadow::Shadow(State* state) {
    this->state = state;
    shaderShadow = ResourceManager::loadShader("shadow");
    glGenFramebuffers(1, &depthMapFBO);

    // Создаем текстуры глубины
    glGenTextures(1, &depthMap);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // Прикрепляем текстуру глубины в качестве буфера глубины для FBO
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
    glDrawBuffer(GL_NONE);
    glReadBuffer(GL_NONE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

}

void Shadow::updateShadows(glm::vec3 lightDir) {

    lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, -10.0f, 20.0f);

    //lightProjection =  glm::perspective(glm::radians(45.0f), (GLfloat)SHADOW_WIDTH / (GLfloat)SHADOW_HEIGHT, -10.0f, 20.0f);

    glm::vec3 pos = state->camera->Position;
    pos.y = 4.0f;
    pos.z = pos.z - 2.0f;
    //pos.x = pos.x + 1.0f;

    glm::vec3 up = glm::normalize(glm::cross(glm::normalize(glm::cross(lightDir, glm::vec3(0.0, 1.0, 0.0))), lightDir));

    lightView = glm::lookAt(pos, pos + glm::normalize(lightDir), up);
    lightSpaceMatrix = lightProjection * lightView;
}

//После use надо будет отрисовывать с новым шейдером!!!!!!
void Shadow::use() {
    shaderShadow->use();
    shaderShadow->setMatrix4("lightSpaceMatrix", lightSpaceMatrix);
    glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
    glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
    glClear(GL_DEPTH_BUFFER_BIT);
}

void Shadow::unuse() {
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glViewport(0, 0, state->width, state->height);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Shadow::initShader(Shader *shader) {
    shader->use();
    glActiveTexture(GL_TEXTURE10);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    glm::vec3 pos = state->camera->Position;
    pos.y = 4.0f;
    pos.z -= 1.0f;
    shader->setInt("shadow_map", 10);
    shader->setVec3("lightPos", pos);
    shader->setMatrix4("lightSpaceMatrix", lightSpaceMatrix);
}

void Shadow::showDebugWindow() {
    Shader* dShader = ResourceManager::loadShader("debugShadow");
    initShader(dShader);


    if (quadVAO == 0)
    {
        float quadVertices[] = {
                // координаты      // текстурные координаты
                0.0f,  0.0f, 0.0f, 0.0f, 1.0f,
                0.0f, -1.0f, 0.0f, 0.0f, 0.0f,
                1.0f,  0.0f, 0.0f, 1.0f, 1.0f,
                1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        };

        // Установка VAO пола
        glGenVertexArrays(1, &quadVAO);
        glGenBuffers(1, &quadVBO);
        glBindVertexArray(quadVAO);
        glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    }

    dShader->use();
    glBindVertexArray(quadVAO);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    glBindVertexArray(0);
}
