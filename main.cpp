#include <iostream>
#include <glad.h>
#include <cmath>
#include <valarray>
#include <GLFW/glfw3.h>
#include <shader/shader.h>
#define STB_IMAGE_IMPLEMENTATION//makes the stb_image library work
#include <stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/io.hpp>
#include <camera.h>
#include <model/model.h>
#include <map>

#include "physics/rigid_cube.h"

float lastX = 0.0f, lastY = 0.0f;
Camera camera = Camera();
Cube firstCube = Cube(glm::vec3(0.0f));
Cube secondCube = Cube(glm::vec3(0.0f, 0.0f, -2.0f));
int movementIndex = 0;
float cubeSpeed = 2.0f;

void framebufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void mouseCallback(GLFWwindow* window, double xPos, double yPos) {
    float xOffset = xPos - lastX;
    float yOffset = lastY - yPos;
    lastX = xPos;
    lastY = yPos;
    camera.ProcessMouseMovement(xOffset, yOffset);
}

void processInput(GLFWwindow *window, float deltaTime)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (movementIndex == 0) {
        if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
            camera.ProcessKeyboard(FORWARD, deltaTime);
        }
        if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
            camera.ProcessKeyboard(BACKWARD, deltaTime);
        }
        if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
            camera.ProcessKeyboard(LEFT, deltaTime);
        }
        if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
            camera.ProcessKeyboard(RIGHT, deltaTime);
        }
    }
    else if (movementIndex == 1) {
        if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
            firstCube.move(glm::vec3(0.0f, 0.0f, 1.0f)*deltaTime*cubeSpeed);
        }
        if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
            firstCube.move(glm::vec3(0.0f, 0.0f, -1.0f)*deltaTime*cubeSpeed);
        }
        if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
            firstCube.move(glm::vec3(1.0f, 0.0f, 0.0f)*deltaTime*cubeSpeed);
        }
        if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
            firstCube.move(glm::vec3(-1.0f, 0.0f, 0.0f)*deltaTime*cubeSpeed);
        }
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
            firstCube.move(glm::vec3(0.0f, 1.0f, 0.0f)*deltaTime*cubeSpeed);
        }
        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
            firstCube.move(glm::vec3(0.0f, -1.0f, 0.0f)*deltaTime*cubeSpeed);
        }
    }
    else if (movementIndex == 2) {
        if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
            secondCube.move(glm::vec3(0.0f, 0.0f, 1.0f)*deltaTime*cubeSpeed);
        }
        if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
            secondCube.move(glm::vec3(0.0f, 0.0f, -1.0f)*deltaTime*cubeSpeed);
        }
        if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
            secondCube.move(glm::vec3(1.0f, 0.0f, 0.0f)*deltaTime*cubeSpeed);
        }
        if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
            secondCube.move(glm::vec3(-1.0f, 0.0f, 0.0f)*deltaTime*cubeSpeed);
        }
        if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
            secondCube.move(glm::vec3(0.0f, 1.0f, 0.0f)*deltaTime*cubeSpeed);
        }
        if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) {
            secondCube.move(glm::vec3(0.0f, -1.0f, 0.0f)*deltaTime*cubeSpeed);
        }
    }
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_E && action == GLFW_PRESS)
        movementIndex = (movementIndex + 1) % 3;
}

unsigned int textureFromFile(const char *path, bool rgba = false) {
    int x, y, nrChannels;
    unsigned char *data = stbi_load(path, &x, &y, &nrChannels, 0);
    if (!data) {
        std::cout << "PROBLEM WHILE LOADING TEXTURE FILE: " << path << std::endl;
    }
    unsigned int texture;
    glActiveTexture(GL_TEXTURE31);
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int format;
    if (rgba)
        format = GL_RGBA;
    else
        format = GL_RGB;

    if (nrChannels == 3)
        glTexImage2D(GL_TEXTURE_2D, 0, format, x, y, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    else if (nrChannels== 4)
        glTexImage2D(GL_TEXTURE_2D, 0, format, x, y, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    else
        std::cout << "BAD CHANNEL COUNT WHILE LOADING TEXTURE: " << path << " CHANNELS: " << nrChannels << std::endl;
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(data);
    return texture;
}

void drawQuad(glm::mat4 &model, glm::mat4 &view, glm::mat4 &projection, Shader &shader) {
    shader.setMat4("model", model);
    shader.setMat4("view", view);
    shader.setMat4("projection", projection);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void drawCubeOutline(glm::mat4 model, Shader &shader, glm::vec3 outlineColor, float outlineWidth) {
    glEnable(GL_STENCIL_TEST);
    glDepthMask(GL_FALSE);
    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
    glStencilMask(0xFF);
    glStencilOp(GL_KEEP, GL_REPLACE, GL_REPLACE);
    glStencilFunc(GL_ALWAYS, 1, 0xFF);
    shader.use();
    shader.setMat4("model", model);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    glDepthMask(GL_TRUE);
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    glStencilMask(0x00);
    glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
    model = glm::scale(model, glm::vec3(1.0f+outlineWidth));
    shader.setMat4("model", model);
    shader.setVec3("color", outlineColor);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    glStencilMask(0xFF);
    glDisable(GL_STENCIL_TEST);
}

void drawCubeAxis(Cube &cube, Shader &shader, glm::vec3 axisColor) {
    glm::mat4 model = glm::mat4(1.0f);
    model = cube.model;
    model = glm::scale(model, glm::vec3(0.05f, 8.0f, 0.05f));
    shader.use();
    shader.setMat4("model", model);
    shader.setVec3("color", axisColor);
    glDrawArrays(GL_TRIANGLES, 0, 36);
}

bool cubesOverlap(Cube &cube1, Cube &cube2, glm::vec3 &shortestAxis, float &shortestOverlap, short &aligned) {
    //the aligned variable stores which element is further along the shortest overlap axis, useful for resolving collisions by moving elements
    glm::vec3 axesToCheck[15];
    axesToCheck[0] = firstCube.rotationMatrix*glm::vec3(1.0f, 0.0f, 0.0f);
    axesToCheck[1] = firstCube.rotationMatrix*glm::vec3(0.0f, 1.0f, 0.0f);
    axesToCheck[2] = firstCube.rotationMatrix*glm::vec3(0.0f, 0.0f, 1.0f);
    axesToCheck[3] = secondCube.rotationMatrix*glm::vec3(1.0f, 0.0f, 0.0f);
    axesToCheck[4] = secondCube.rotationMatrix*glm::vec3(0.0f, 1.0f, 0.0f);
    axesToCheck[5] = secondCube.rotationMatrix*glm::vec3(0.0f, 0.0f, 1.0f);
    short axisIndex = 6;
    for (int i = 0; i < 3; i++) {
        for (int j = 3; j < 6; j++) {
            axesToCheck[axisIndex] = glm::normalize(glm::cross(axesToCheck[i], axesToCheck[j]));
            axisIndex++;
        }
    }

    for (glm::vec3 axis : axesToCheck) {
        if (glm::isnan(axis.x))
            continue;
        float maxDot1 = -std::numeric_limits<float>::max(), maxDot2 = -std::numeric_limits<float>::max();
        float minDot1 = std::numeric_limits<float>::max(), minDot2 = std::numeric_limits<float>::max();

        for (glm::vec3 point : cube1.points) {
            float dot = glm::dot(point, axis);
            if (dot > maxDot1)
                maxDot1 = dot;
            if (dot < minDot1)
                minDot1 = dot;
        }
        for (glm::vec3 point : cube2.points) {
            float dot = glm::dot(point, axis);
            if (dot > maxDot2)
                maxDot2 = dot;
            if (dot < minDot2)
                minDot2 = dot;
        }

        if (maxDot1 < minDot2 || maxDot2 < minDot1) {
            return false;
        }
        else {
            float shorterOverlap = std::min(maxDot1-minDot2, maxDot2-minDot1);
            if (shorterOverlap < shortestOverlap) {
                if (maxDot1-minDot2 < maxDot2-minDot1) {
                    aligned = 1;
                }
                else {
                    aligned = 0;
                }
                shortestAxis = axis;
                shortestOverlap = shorterOverlap;
            }
        }
    }
    return true;
}

int main() {
    stbi_set_flip_vertically_on_load(true);
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
    if (window == NULL) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
    glfwSetKeyCallback(window, key_callback);

    glm::mat4 projection;
    projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

    Shader shaderProgram("./include/shader/basicShader.vsh", "./include/shader/basicShader.fsh");
    Shader singleShader("./include/shader/basicShader.vsh","./include/shader/singleColor.fsh");
    Shader cubemapShader("./include/shader/cubeShader.vsh", "./include/shader/cubeShader.fsh");
    Shader mirrorShader("./include/shader/basicShader.vsh", "./include/shader/mirrorShader.fsh");

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouseCallback);

    float cubeVertices[] = {
        // Back face
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f, 0.0f, -1.0f, // Bottom-left
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f, 0.0f, -1.0f, // bottom-right
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f, 0.0f, -1.0f, // top-right
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f, 0.0f, -1.0f, // top-right
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f, 0.0f, -1.0f, // top-left
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, 0.0f, 0.0f, -1.0f, // bottom-left
        // Front face
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom-left
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f, 0.0f, 1.0f, // top-right
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom-right
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 0.0f, 0.0f, 1.0f, // top-right
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom-left
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f, 0.0f, 0.0f, 1.0f, // top-left
        // Left face
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, -1.0f, 0.0f, 0.0f, // top-right
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, -1.0f, 0.0f, 0.0f, // bottom-left
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, -1.0f, 0.0f, 0.0f, // top-left
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, -1.0f, 0.0f, 0.0f, // bottom-left
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, -1.0f, 0.0f, 0.0f, // top-right
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, -1.0f, 0.0f, 0.0f, // bottom-right
        // Right face
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.0f, 0.0f, 0.0f, // top-left
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 1.0f, 0.0f, 0.0f, // top-right
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0f, 0.0f, 0.0f, // bottom-right
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 1.0f, 0.0f, 0.0f, // bottom-right
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom-left
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 1.0f, 0.0f, 0.0f, // top-left
        // Bottom face
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f, -1.0f, 0.0f, // top-right
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f, -1.0f, 0.0f, // bottom-left
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f, 0.0f, -1.0f, 0.0f, // top-left
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 0.0f, -1.0f, 0.0f, // bottom-left
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f, -1.0f, 0.0f, // top-right
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, 0.0f, -1.0f, 0.0f, // bottom-right
        // Top face
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f, 1.0f, 0.0f,  // top-left
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f, 1.0f, 0.0f,  // top-right
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f, 1.0f, 0.0f,  // bottom-right
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f, 0.0f, 1.0f, 0.0f,  // bottom-right
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 0.0f, 1.0f, 0.0f,  // bottom-left
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, 0.0f, 1.0f, 0.0f   // top-left
    };

    //CUBE VAO CREATION
    unsigned int cubeVAO, cubeVBO;
    glGenVertexArrays(1, &cubeVAO);
    glBindVertexArray(cubeVAO);
    glGenBuffers(1, &cubeVBO);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), static_cast<void *>(0));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8*sizeof(float), reinterpret_cast<void *>(3 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8*sizeof(float), reinterpret_cast<void *>(5 * sizeof(float)));
    glEnableVertexAttribArray(1);
    //0 is pos, 1 is normal, 2 is texture coords
    glBindVertexArray(0);

    unsigned int uniformBuffer;
    glGenBuffers(1, &uniformBuffer);
    glBindBuffer(GL_UNIFORM_BUFFER, uniformBuffer);
    glBufferData(GL_UNIFORM_BUFFER, 2*sizeof(glm::mat4), nullptr, GL_STATIC_DRAW);
    glBindBufferBase(GL_UNIFORM_BUFFER, 0, uniformBuffer);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);

    float deltaTime = 0.0f, lastFrame = 0.0f;

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);

    while(!glfwWindowShouldClose(window)) {

        deltaTime = glfwGetTime() - lastFrame;
        lastFrame = glfwGetTime();
        processInput(window, deltaTime);

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        glm::mat4 view = camera.GetViewMatrix();
        glBindBuffer(GL_UNIFORM_BUFFER, uniformBuffer);
        glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(projection));
        glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(view));

        firstCube.rotate(glm::quat(glm::angleAxis(glm::radians(35.0f)*deltaTime, glm::vec3(0.0f, 1.0f, 0.0f))));

        glBindVertexArray(cubeVAO);
        singleShader.use();
        float overlap = std::numeric_limits<float>::max();
        glm::vec3 overlapAxis;
        short alignedIndex;
        if (cubesOverlap(firstCube, secondCube, overlapAxis, overlap, alignedIndex)) {
            singleShader.setVec3("color", glm::vec3(1.0f, 0.0f, 0.0f));
            if (alignedIndex == 0)
                firstCube.move(overlapAxis*overlap);
            else
                firstCube.move(-overlapAxis*overlap);
            std::cout << overlapAxis << overlap << std::endl;
            }
        else
            singleShader.setVec3("color", glm::vec3(0.0f, 1.0f, 0.0f));
        singleShader.setMat4("model", firstCube.model);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        singleShader.setMat4("model", secondCube.model);
        glDrawArrays(GL_TRIANGLES, 0, 36);

        if (movementIndex == 1)
        {
            drawCubeOutline(firstCube.model, singleShader, glm::vec3(1.0f), 0.05f);
            drawCubeAxis(firstCube, singleShader, glm::vec3(1.0f, 1.0f, 0.0f));
        }
        if (movementIndex == 2)
        {
            drawCubeOutline(secondCube.model, singleShader, glm::vec3(1.0f), 0.05f);
            drawCubeAxis(secondCube, singleShader, glm::vec3(1.0f, 1.0f, 0.0f));
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwTerminate();
    return 0;
}