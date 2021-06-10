//
// Created by pshbl on 10.06.2021.
//

#ifndef RTS_SHADOW_H
#define RTS_SHADOW_H
#include <glm/glm.hpp>
#include "../shaders/Shader.h"
#include "../state/State.h"

class Shadow {
    const unsigned int SHADOW_WIDTH = 2048, SHADOW_HEIGHT = 2048;

    glm::mat4 lightProjection, lightView;
    glm::mat4 lightSpaceMatrix;
    unsigned int depthMapFBO;
    unsigned int depthMap;
    State* state;
    unsigned int quadVAO = 0;
    unsigned int quadVBO;

public:
    Shader* shaderShadow;

    Shadow(State* state);
    void updateShadows(glm::vec3 lightDir);
    void use();
    void initShader(Shader* shader);
    void unuse();

    void showDebugWindow();
};


#endif //RTS_SHADOW_H
