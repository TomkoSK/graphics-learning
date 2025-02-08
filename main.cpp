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

float lastX = 0.0f, lastY = 0.0f;
glm::vec3 lightPos(1.2f, 3.0f, 2.0f);

Camera camera = Camera();

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

unsigned int textureFromFile(const char *path) {
    int x, y, nrChannels;
    unsigned char *data = stbi_load(path, &x, &y, &nrChannels, 0);
    if (!data) {
        std::cout << "PROBLEM WHILE LOADING TEXTURE FILE: " << path << std::endl;
    }
    unsigned int texture;
    glActiveTexture(GL_TEXTURE31);
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    if (nrChannels == 3)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, x, y, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    else if (nrChannels== 4)
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, x, y, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    else
        std::cout << "BAD CHANNEL COUNT WHILE LOADING TEXTURE: " << path << " CHANNELS: " << nrChannels << std::endl;
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(data);
    return texture;
}

void drawCube(glm::mat4 &model, glm::mat4 &view, glm::mat4 &projection, Shader &shader) {
    shader.use();
    shader.setMat4("model", model);
    shader.setMat4("view", view);
    shader.setMat4("projection", projection);
    glDrawArrays(GL_TRIANGLES, 0, 36);
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

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    Shader shaderProgram("./include/shader/basicShader.vsh", "./include/shader/basicShader.fsh");
    Shader singleShader("./include/shader/basicShader.vsh","./include/shader/singleColor.fsh");

    glm::mat4 projection;
    projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

    glEnable(GL_DEPTH_TEST);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouseCallback);

    float cubeVertices[] = {
    // Back face
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // Bottom-left
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f, // bottom-right
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // bottom-left
    // Front face
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // top-right
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-right
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // top-right
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f, // top-left
    // Left face
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-right
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-left
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-left
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-left
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-right
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-right
    // Right face
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-left
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-right
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-right
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-left
    // Bottom face
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // top-right
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-left
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f, // top-left
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-left
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // top-right
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-right
    // Top face
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // bottom-right
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // bottom-right
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f, // bottom-left
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f  // top-left
};


    unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5*sizeof(float), static_cast<void *>(0));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5*sizeof(float), reinterpret_cast<void *>(3 * sizeof(float)));
    glEnableVertexAttribArray(2);

    unsigned int texture = textureFromFile("container.jpg");
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);

    unsigned int textureMetal = textureFromFile("metal.png");
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, textureMetal);

    unsigned int textureMeow = textureFromFile("meow.png");
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, textureMeow);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_STENCIL_TEST);
    float deltaTime = 0.0f, lastFrame = 0.0f;

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CW);
    while(!glfwWindowShouldClose(window)) {
        deltaTime = glfwGetTime() - lastFrame;
        lastFrame = glfwGetTime();
        processInput(window, deltaTime);

        glStencilMask(0xFF);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
        glDepthFunc(GL_LESS);
        glStencilMask(0x00);
        glStencilFunc(GL_ALWAYS, 1, 0xFF);

        glm::mat4 view;
        view = camera.GetViewMatrix();

        glm::mat4 model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, -1.06f, 0.0f));
        model = glm::scale(model, glm::vec3(12.0f, 0.05f, 12.0f));

        shaderProgram.use();
        shaderProgram.setInt("textureSampler", 1);
        drawCube(model, view, projection, shaderProgram);

        //FIRST CUBE DRAWN

        glDepthFunc(GL_LESS);
        glStencilMask(0x00);
        glStencilFunc(GL_ALWAYS, 1, 0xFF);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(5.0f, 0.0f, 0.0f));
        model = glm::scale(model, glm::vec3(0.95f));
        shaderProgram.setInt("textureSampler", 0);
        drawCube(model, view, projection, shaderProgram);

        //REAL STUFF

        glDepthFunc(GL_LESS);
        glStencilMask(0xF0);
        glStencilOp(GL_KEEP, GL_REPLACE, GL_KEEP);
        glStencilFunc(GL_ALWAYS, 128, 0xF0);

        model = glm::mat4(1.0f);
        shaderProgram.setInt("textureSampler", 0);
        drawCube(model, view, projection, shaderProgram);

        glDepthFunc(GL_LESS);
        glStencilMask(0xF);
        glStencilOp(GL_KEEP, GL_REPLACE, GL_KEEP);
        glStencilFunc(GL_ALWAYS, 1, 0xF);

        model = glm::mat4(1.0f);
        model = glm::translate(model, glm::vec3(5.0f, 0.0f, 0.0f));
        shaderProgram.setInt("textureSampler", 0);
        drawCube(model, view, projection, shaderProgram);

        glDepthFunc(GL_ALWAYS);
        glStencilMask(0x00);
        glStencilFunc(GL_EQUAL, 1, 0xF);
        singleShader.use();
        singleShader.setInt("textureSampler", 2);
        drawCube(model, view, projection, singleShader);

        model = glm::mat4(1.0f);
        glDepthFunc(GL_ALWAYS);
        glStencilMask(0x00);
        glStencilFunc(GL_EQUAL, 128, 0xF0);
        singleShader.use();
        singleShader.setInt("textureSampler", 2);
        drawCube(model, view, projection, singleShader);


        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}