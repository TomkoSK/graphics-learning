#include <glm/gtc/matrix_transform.hpp>
#include <physics/rigid_cube.h>

Cube::Cube(glm::vec3 position, glm::quat rotation) {
    this->position = position;
    this->model = glm::mat4(1.0f);
    this->model = glm::translate(this->model, position);
    this->rotation = rotation;
    updateModel();
    updatePoints();
}

void Cube::move(glm::vec3 offset) {
    this->position += offset;
    this->model = glm::translate(this->model, offset);
    updatePoints();
}

void Cube::setPos(glm::vec3 position) {
    this->position = position;
    updateModel();
    updatePoints();
}

void Cube::addImpulse(glm::vec3 direction) {
    this->velocityUpdate += direction;
}


void Cube::updateModel() {
    this->rotationMatrix = glm::mat3_cast(rotation);
    this->model = glm::translate(glm::mat4(1.0f), position)*glm::mat4(this->rotationMatrix);
}

void Cube::rotate(glm::quat rotation) {
    this->rotation *= rotation;
    updateModel();
    updatePoints();
}

void Cube::updatePoints(){ //Returns 8 points of a cube
    this->points[0] = position+rotationMatrix*glm::vec3(-0.5, 0.5, 0.5);
    this->points[1] = position+rotationMatrix*glm::vec3(-0.5, 0.5, -0.5);
    this->points[2] = position+rotationMatrix*glm::vec3(-0.5, -0.5, 0.5);
    this->points[3] = position+rotationMatrix*glm::vec3(-0.5, -0.5, -0.5);
    this->points[4] = position+rotationMatrix*glm::vec3(0.5, 0.5, 0.5);
    this->points[5] = position+rotationMatrix*glm::vec3(0.5, 0.5, -0.5);
    this->points[6] = position+rotationMatrix*glm::vec3(0.5, -0.5, 0.5);
    this->points[7] = position+rotationMatrix*glm::vec3(0.5, -0.5, -0.5);
    this->center = position;
}

void Cube::update(float deltaTime) {
    this->velocity += velocityUpdate;
    this->velocityUpdate = glm::vec3(0.0f);
    this->position += this->velocity*deltaTime;
    updateModel();
    updatePoints();
}


