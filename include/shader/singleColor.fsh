#version 330 core

in vec2 texCoords;
in vec3 fragPos;
in vec3 normal;

out vec4 FragColor;

uniform vec3 color;

void main() {
    FragColor = vec4(color, 1.0f);
}