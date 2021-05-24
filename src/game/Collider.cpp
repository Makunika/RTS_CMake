#include "Collider.h"
//
// Created by pshbl on 23.05.2021.
//

bool Collider::isDrawable = false;
VAO_VBO Collider::vaoVbo = {0,0};
Shader* Collider::debugShader = nullptr;

