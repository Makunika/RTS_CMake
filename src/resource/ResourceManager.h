//
// Created by pshbl on 28.05.2021.
//

#ifndef RTS_RESOURCEMANAGER_H
#define RTS_RESOURCEMANAGER_H


#include <map>
#include <string>
#include "../shaders/Shader.h"
#include "../textures/Texture.h"

class ResourceManager {
    static std::map<std::string, Shader*> shaders;
    static std::map<std::string, Texture*> textures;
    static std::map<std::string, Model*> models;

public:
    static Shader* loadShader(std::string name);

    static Shader* loadShader(std::string name, bool withGeom);

    static Texture loadTexture(const char *file, bool alpha, std::string name);


    static Model* loadModel(std::string name);

    static void clear();

private:
    ResourceManager() { }
};


#endif //RTS_RESOURCEMANAGER_H
