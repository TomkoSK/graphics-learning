#version 330 core

in vec3 texDirection;

out vec4 FragColor;

uniform samplerCube cubeTexture;

void main() {
    FragColor = texture(cubeTexture, texDirection);
}