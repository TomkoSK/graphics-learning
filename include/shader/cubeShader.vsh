#version 460 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 texDirection;

void main() {
    vec4 po = projection * view * vec4(aPos, 1.0f);
    gl_Position = po.xyww;

    texDirection = aPos;
}