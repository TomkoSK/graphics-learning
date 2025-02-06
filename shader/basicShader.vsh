#version 460 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec2 texCoords;
out vec3 normal;
out vec3 fragPos;
out mat4 viewMatrix;

void main() {
    gl_Position = projection * view * model * vec4(aPos, 1.0f);
    normal = normalize(mat3(transpose(inverse(view*model)))*aNormal);
    //vec3 normal = normalize(mat3(view*model)*aNormal);
    fragPos = vec3(view*model*vec4(aPos, 1.0));
    viewMatrix = view;
    texCoords = aTexCoords;
}