//
// Created by pshbl on 01.06.2021.
//
#include <glm/gtc/type_ptr.hpp>
#include <glm/glm.hpp>
#include "../resource/ResourceManager.h"
#include "Floor.h"
#include "../textures/TextureOne.h"

Floor::Floor(State* state) {
    int slices = 64;
    std::vector<glm::vec3> vertices;
    std::vector<glm::uvec4> indices;

    for(int j=0; j<=slices; ++j) {
        for(int i=0; i<=slices; ++i) {
            float x = (float)i/(float)slices;
            float y = 0;
            float z = (float)j/(float)slices;
            vertices.push_back(glm::vec3(x, y, z));
        }
    }

    for(int j=0; j<slices; ++j) {
        for(int i=0; i<slices; ++i) {

            int row1 =  j    * (slices+1);
            int row2 = (j+1) * (slices+1);

            indices.push_back(glm::uvec4(row1+i, row1+i+1, row1+i+1, row2+i+1));
            indices.push_back(glm::uvec4(row2+i+1, row2+i, row2+i, row1+i));

        }
    }

    glGenVertexArrays(1, &vFloorLines.VAO );
    glBindVertexArray(vFloorLines.VAO);

    glGenBuffers( 1, &vFloorLines.VBO );
    glBindBuffer(GL_ARRAY_BUFFER, vFloorLines.VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(glm::vec3), glm::value_ptr(vertices[0]), GL_STATIC_DRAW );
    glEnableVertexAttribArray( 0 );
    glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, nullptr );

    glGenBuffers( 1, &ibo );
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo );
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(glm::uvec4), glm::value_ptr(indices[0]), GL_STATIC_DRAW);

    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    length = (GLuint)indices.size() * 4;


    vector<float> q
    {
        1.0f, -0.5f,  1.0f,  64.0f, 0.0f,
        -1.0f, -0.5f, -1.0f,  0.0f, 64.0f,
        1.0f, -0.5f, -1.0f,  64.0f, 64.0f,

        1.0f, -0.5f,  1.0f,  64.0f, 00.0f,
        -1.0f, -0.5f,  1.0f,  0.0f, 0.0f,
        -1.0f, -0.5f, -1.0f,  0.0f, 64.0f
    };
    vFloorGlass = ShaderUtils::loadTextureAndCoordinate(&q[0], q.size());


    shaderGlass = ResourceManager::loadShader("floorGrass");
    shaderLines = ResourceManager::loadShader("floorLines");

    diff = new TextureOne(R"(floor\grass-diff.jpg)");
    spec = new TextureOne(R"(floor\grass-specular.jpg)");
    this->state = state;

}

void Floor::draw(LightState* lightState) {
    glm::mat4 p = state->getProjection();
    glm::mat4 v = state->camera->getViewMatrix();

    if (state->debug) {
        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(-32.5f, -0.5, -32.5f));
        model = glm::scale(model, glm::vec3(64.0f));

        shaderLines->use();
        shaderLines->setProjectionAndView(p, v);
        shaderLines->setMatrix4("model", model);

        glBindVertexArray(vFloorLines.VAO);

        glDrawElements(GL_LINES, length, GL_UNSIGNED_INT, NULL);

        glBindVertexArray(0);
    }

    glCullFace(GL_FRONT);

    shaderGlass->use();
    shaderGlass->setProjectionAndView(p, v);
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::scale(model, glm::vec3(64.0f, 1.0f, 64.0f));
    model = glm::translate(model, glm::vec3(0.0f, 0.5f, 0.0f));
    shaderGlass->setMatrix4("model", model);
    shaderGlass->setFloat("shininess", 64.0f);
    shaderGlass->setVec3("viewPos", state->camera->Position);
    shaderGlass->setInt("texture_diffuse1", 0);
    shaderGlass->setInt("texture_specular1", 1);
    lightState->allUse(shaderGlass);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, diff->getTexture());
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, spec->getTexture());

    glBindVertexArray(vFloorGlass.VAO);

    glDrawArrays(GL_TRIANGLES, 0, 6);

    glBindVertexArray(0);

    glCullFace(GL_BACK);
}

void Floor::drawForShadow(Shader *shader) {

    shader->use();
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::scale(model, glm::vec3(64.0f, 1.0f, 64.0f));
    shader->setMatrix4("model", model);

    glBindVertexArray(vFloorGlass.VAO);

    glDrawArrays(GL_TRIANGLES, 0, 6);

    glBindVertexArray(0);
}
