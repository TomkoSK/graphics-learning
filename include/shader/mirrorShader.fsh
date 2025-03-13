#version 460 core

in vec2 texCoords;

out vec4 FragColor;

uniform sampler2D mirrorTexture;

void main(){
    vec4 texColor = texture(mirrorTexture, texCoords);
    if(texColor == vec4(0.0f, 0.0f, 0.0f, 1.0f)){
        FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
    }
    else{
        FragColor = texColor;
    }
}