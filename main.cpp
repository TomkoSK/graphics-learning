#include <iostream>
#include <glad.h>
#include <cmath>
#include <valarray>
#include <GLFW/glfw3.h>
#include <shader.h>
#define STB_IMAGE_IMPLEMENTATION//makes the stb_image library work
#include <stb_image.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/io.hpp>
#include <camera.h>

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

    float cubeVertices[] = {
        // positions // normals // texture coords
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f
        };

    unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), static_cast<void *>(0));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), reinterpret_cast<void *>(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), reinterpret_cast<void *>(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    unsigned int lightVAO, lightVBO;
    glGenVertexArrays(1, &lightVAO);
    glBindVertexArray(lightVAO);
    glGenBuffers(1, &lightVBO);
    glBindBuffer(GL_ARRAY_BUFFER, lightVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), cubeVertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), static_cast<void *>(0));
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), reinterpret_cast<void *>(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    Shader shaderProgram("./shader/basicShader.vsh", "./shader/basicShader.fsh");
    Shader lightingShader("./shader/lightShader.vsh", "./shader/lightShader.fsh");

    glm::mat4 projection;
    projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

    glEnable(GL_DEPTH_TEST);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetCursorPosCallback(window, mouseCallback);

    int width, height, nrChannels;
    unsigned char *data = stbi_load("woodBox.png", &width, &height,
    &nrChannels, 0);
    if (!data) {
        std::cout << "Failed to load image" << std::endl;
    }
    unsigned int diffuseTexture;
    glGenTextures(1, &diffuseTexture);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, diffuseTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data);

    data = stbi_load("woodBoxSpecular.png", &width, &height,&nrChannels, 0);
    if (!data) {
        std::cout << "Failed to load image" << std::endl;
    }
    unsigned int specularTexture;
    glGenTextures(1, &specularTexture);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, specularTexture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data);

    glm::vec3 cubePositions[] = {
        glm::vec3( 0.0f, 0.0f, 0.0f),
        glm::vec3( 2.0f, 5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3( 2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f, 3.0f, -7.5f),
        glm::vec3( 1.3f, -2.0f, -2.5f),
        glm::vec3( 1.5f, 2.0f, -2.5f),
        glm::vec3( 1.5f, 0.2f, -1.5f),
        glm::vec3(-1.3f, 1.0f, -1.5f)
        };

    glm::vec3 pointLightPositions[] = {
        glm::vec3( 0.7f, 0.2f, 2.0f),
        glm::vec3( 2.3f, -3.3f, -4.0f),
        glm::vec3(-4.0f, 2.0f, -12.0f),
        glm::vec3( 0.0f, 0.0f, -3.0f)
        };

    glm::vec3 lightColor = glm::vec3(0.5f, 1.0f, 0.5f);
    float deltaTime = 0.0f, lastFrame = 0.0f;
    while(!glfwWindowShouldClose(window)) {
        deltaTime = glfwGetTime() - lastFrame;
        lastFrame = glfwGetTime();
        processInput(window, deltaTime);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glm::mat4 view;
        view = camera.GetViewMatrix();
        glm::mat4 model = glm::mat4(1.0f);
        glBindVertexArray(VAO);

        shaderProgram.use();
        shaderProgram.setVec3("material.ambient", 1.0f, 1.0f, 1.0f);
        shaderProgram.setInt("material.diffuse", 0);
        shaderProgram.setInt("material.specular", 1);
        shaderProgram.setFloat("material.shininess", 32.0f);


        // point light 1
        shaderProgram.setVec3("pointLights[0].position", pointLightPositions[0]);
        shaderProgram.setVec3("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
        shaderProgram.setVec3("pointLights[0].diffuse", lightColor);
        shaderProgram.setVec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
        shaderProgram.setFloat("pointLights[0].constant", 1.0f);
        shaderProgram.setFloat("pointLights[0].linear", 0.09f);
        shaderProgram.setFloat("pointLights[0].quadratic", 0.032f);
        // point light 2
        shaderProgram.setVec3("pointLights[1].position", pointLightPositions[1]);
        shaderProgram.setVec3("pointLights[1].ambient", 0.05f, 0.05f, 0.05f);
        shaderProgram.setVec3("pointLights[1].diffuse", lightColor);
        shaderProgram.setVec3("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
        shaderProgram.setFloat("pointLights[1].constant", 1.0f);
        shaderProgram.setFloat("pointLights[1].linear", 0.09f);
        shaderProgram.setFloat("pointLights[1].quadratic", 0.032f);
        // point light 3
        shaderProgram.setVec3("pointLights[2].position", pointLightPositions[2]);
        shaderProgram.setVec3("pointLights[2].ambient", 0.05f, 0.05f, 0.05f);
        shaderProgram.setVec3("pointLights[2].diffuse", lightColor);
        shaderProgram.setVec3("pointLights[2].specular", 1.0f, 1.0f, 1.0f);
        shaderProgram.setFloat("pointLights[2].constant", 1.0f);
        shaderProgram.setFloat("pointLights[2].linear", 0.09f);
        shaderProgram.setFloat("pointLights[2].quadratic", 0.032f);
        // point light 4
        shaderProgram.setVec3("pointLights[3].position", pointLightPositions[3]);
        shaderProgram.setVec3("pointLights[3].ambient", 0.05f, 0.05f, 0.05f);
        shaderProgram.setVec3("pointLights[3].diffuse", lightColor);
        shaderProgram.setVec3("pointLights[3].specular", 1.0f, 1.0f, 1.0f);
        shaderProgram.setFloat("pointLights[3].constant", 1.0f);
        shaderProgram.setFloat("pointLights[3].linear", 0.09f);
        shaderProgram.setFloat("pointLights[3].quadratic", 0.032f);

        shaderProgram.setVec3("dirLight.direction", glm::normalize(glm::vec3(-0.5f, -0.5f, -0.5f)));
        shaderProgram.setVec3("dirLight.ambient", glm::vec3(0.05f, 0.05f, 0.05f));
        shaderProgram.setVec3("dirLight.diffuse", glm::vec3(1.0f, 1.0f, 0.4f));
        shaderProgram.setVec3("dirLight.specular", 1.0f, 1.0f, 0.5f);
        /*
        shaderProgram.setVec3("light.ambient", 0.15f, 0.15f, 0.15f);
        shaderProgram.setVec3("light.diffuse", 0.5f, 0.5f, 0.5f);
        shaderProgram.setVec3("light.specular", lightColor*0.6f);
        shaderProgram.setVec3("light.position", lightPos);


        shaderProgram.setFloat("light.constant", 1.0f);
        shaderProgram.setFloat("light.linear", 0.09f);s
        shaderProgram.setFloat("light.quadratic", 0.0032f);
        */

        shaderProgram.setVec3("spotlight.position", camera.Position);
        shaderProgram.setVec3("spotlight.direction", camera.Front);
        shaderProgram.setFloat("spotlight.cutoff", glm::cos(glm::radians(0.1f)));
        shaderProgram.setFloat("spotlight.outerCutOff", glm::cos(glm::radians(0.1f)));
        //shaderProgram.setVec3("viewPos", camera.Position);
        shaderProgram.setMat4("model", model);
        shaderProgram.setMat4("view", view);
        shaderProgram.setMat4("projection", projection);

        for (int i = 0; i < 10; i++) {
            model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            shaderProgram.setMat4("model", model);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }

        glBindVertexArray(lightVAO);
        lightingShader.use();
        lightingShader.setMat4("view", view);
        lightingShader.setMat4("projection", projection);
        for (int i = 0; i < 4; i++) {
            model = glm::mat4(1.0f);
            model = glm::translate(model, pointLightPositions[i]);
            model = glm::scale(model, glm::vec3(0.3f));
            lightingShader.setMat4("model", model);
            lightingShader.setVec3("lightColor", lightColor);
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}