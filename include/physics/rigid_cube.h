#ifndef RIGID_CUBE_H
#define RIGID_CUBE_H
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

class Cube {
public:
    explicit Cube(glm::vec3 position, glm::quat rotation = glm::quat(glm::angleAxis(glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f))));
    void move(glm::vec3 offset);
    void setPos(glm::vec3 position);
    void rotate(glm::quat rotation);
    glm::vec3 position;
    glm::quat rotation;
    glm::mat4 model;
    glm::vec3 points[8];
    glm::mat3 rotationMatrix;
    glm::vec3 velocity;
    void addImpulse(glm::vec3 direction);
    void update(float deltaTime);
private:
    void updateModel();
    void updatePoints();
    glm::vec3 velocityUpdate;//if 2 objects collide and affect each other's velocity, the one that's affected first will then have an incorrect velocity for the
    //other one to read, so the real velocity is only updated during the update function

};

#endif
