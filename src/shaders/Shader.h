#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <glm/fwd.hpp>
#include <glad/glad.h>

class Shader
{
public:
    GLuint Program;

    Shader(const GLchar* name);

    Shader(const GLchar* name, bool withGeom);

    void use();

    GLint getUniformLocation(std::string uniform);

    void setFloat(std::string uniform, GLfloat value);

    void setVec4(std::string uniform, GLfloat value1, GLfloat value2, GLfloat value3, GLfloat value4);

    void setVec3(std::string uniform, GLfloat value1, GLfloat value2, GLfloat value3);

    void setVec3(std::string uniform, glm::vec3& vec3);

    void setVec2(std::string uniform, glm::vec2& vec2);

    void setVec2(std::string uniform, GLfloat value1, GLfloat value2);

    void setProjectionAndView(glm::mat4& proj, glm::mat4& view);

    void setMatrix4(std::string uniform, glm::mat4& matrix);

	void setInt(std::string uniform, int value);

};

