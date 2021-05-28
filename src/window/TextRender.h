//
// Created by pshbl on 27.05.2021.
//

#ifndef RTS_TEXTRENDER_H
#define RTS_TEXTRENDER_H
#include <ft2build.h>
#include <map>
#include "../resource/ResourceManager.h"
#include FT_FREETYPE_H

struct Character {
    unsigned int textureID; // ID текстуры глифа
    glm::ivec2 size; // размер глифа
    glm::ivec2 bearing; // смещение от линии шрифта до верхнего/левого угла глифа
    unsigned int advance; // смещение до следующего глифа
};



class TextRender {
private:
    std::map<char, Character> characters;
    unsigned int VAO, VBO;
    Shader* shader;

public:
    TextRender(int width, int height, std::string font);
    void renderText(std::string text, float x, float y, float scale, glm::vec3 color = glm::vec3(1.0f));
};


#endif //RTS_TEXTRENDER_H
