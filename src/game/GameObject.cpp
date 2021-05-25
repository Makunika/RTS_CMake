//
// Created by pshbl on 22.05.2021.
//

#include "GameObject.h"
#include <limits>
#include <glm/gtx/intersect.hpp>

GameObject::GameObject(Model *model, float x, float y, int zLevel, float scale, State* state) : model(model),
                                                                                         x(x), y(y),
                                                                                         zLevel(zLevel), scale(scale), state(state),
                                                                                         isSelected(false) {}

bool GameObject::collide(GameObject *gameObject) {
    if (this == gameObject) {
        return false;
    }
    return this->collider->collide(gameObject->x - this->x, gameObject->y - this->y, gameObject->collider);
}

void GameObject::draw(Shader &shader, Shader& coloringShader) {
   // std::cout << *this << std::endl;
    coloringShader.use();
    glm::mat4 modelS = glm::mat4(1.0f);
    modelS = glm::translate(modelS, glm::vec3(x, 0.0f + 1.0 * zLevel, y));
    modelS = glm::scale(modelS, glm::vec3(scale + scale * 0.1f));
    coloringShader.setMatrix4("model", modelS);

    shader.use();
    modelS = glm::mat4(1.0f);
    modelS = glm::translate(modelS, glm::vec3(x, 0.0f + 1.0 * zLevel, y));
    modelS = glm::scale(modelS, glm::vec3(scale));
    shader.setMatrix4("model", modelS);
    if (isSelected) {
        model->drawSelected(shader, coloringShader);
    } else {
        model->draw(shader);
    }
    collider->drawDebug(this->x, this->y, this->zLevel);
}

ostream &operator<<(ostream &os, const GameObject &object) {
    os << " x: " << object.x << " y: " << object.y
       << " zLevel: " << object.zLevel << " scale: " << object.scale;
    return os;
}

void GameObject::setSelected(bool selected) {
    isSelected = selected;
}

bool GameObject::intersectionRay(glm::vec3 &direction, glm::vec3& start, float& dist) {

    bool b =  glm::intersectRaySphere(start, direction, glm::vec3(x, 0.0f + 1.0f * zLevel, y), 0.5,dist);
    if (b) {
        std::cout << "dist: " << dist << std::endl;
    }
    return b;
}
