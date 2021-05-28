//
// Created by pshbl on 28.05.2021.
//

#include "ResourceManager.h"

Shader* ResourceManager::loadShader(std::string name) {
    return loadShader(name, false);
}

Texture ResourceManager::loadTexture(const char *file, bool alpha, std::string name) {
    return Texture();
}

void ResourceManager::clear() {

}

Model *ResourceManager::loadModel(std::string name) {
    if (models.contains(name)) {
        return models[name];
    } else {
        Model* newModel = new Model(name);
        models[name] = newModel;
        return newModel;
    }
}

Shader *ResourceManager::loadShader(std::string name, bool withGeom) {
    if (shaders.contains(name)) {
        return shaders[name];
    } else {
        Shader* newShader = new Shader(name.c_str(), withGeom);
        shaders[name] = newShader;
        return newShader;
    }
}

std::map<std::string, Shader*> ResourceManager::shaders = std::map<std::string, Shader*>();
std::map<std::string, Texture*> ResourceManager::textures = std::map<std::string, Texture*>();
std::map<std::string, Model*> ResourceManager::models = std::map<std::string, Model*>();
