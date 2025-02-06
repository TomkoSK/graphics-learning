#version 460 core

in vec3 normal;
in vec3 fragPos;
in vec2 texCoords;
in mat4 viewMatrix;

out vec4 FragColor;

struct Material {
    vec3 ambient;
    sampler2D diffuse;
    sampler2D specular;
    float shininess;
};
uniform Material material;

struct PointLight {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    vec3 position;

    float constant;
    float linear;
    float quadratic;
};
#define NR_POINT_LIGHTS 4
uniform PointLight pointLights[NR_POINT_LIGHTS];

struct SpotLight {
    vec3 position;
    vec3 direction;
    float cutoff;
    float outerCutOff;
};
uniform SpotLight spotlight;

struct DirectionalLight {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
uniform DirectionalLight dirLight;

vec3 calculateDirLight(DirectionalLight light){
    vec3 ambient = light.ambient * material.ambient * vec3(texture(material.diffuse,texCoords));

    vec3 viewLightDir = vec3(viewMatrix*vec4(light.direction, 0.0));
    float diff = max(dot(-viewLightDir, normal), 0.0f);
    vec3 diffuse = diff * light.diffuse * vec3(texture(material.diffuse, texCoords));

    vec3 viewDir = normalize(-fragPos);
    vec3 reflectDir = reflect(viewLightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * vec3(texture(material.specular, texCoords)) * spec;

    return ambient+diffuse+specular;
}

vec3 calculatePointLight(PointLight light) {

    vec3 ambient = light.ambient * material.ambient * vec3(texture(material.diffuse,texCoords));

    vec3 lightPosView = vec3(viewMatrix*vec4(light.position, 1.0));
    vec3 lightDir = normalize(lightPosView-fragPos);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * light.diffuse * vec3(texture(material.diffuse, texCoords));

    vec3 viewDir = normalize(-fragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specular = light.specular * vec3(texture(material.specular, texCoords)) * spec;

    float distance = length(lightPosView-fragPos);
    float attenuation = 1/(light.constant+light.linear*distance+light.quadratic*distance*distance);

    diffuse *= attenuation;
    specular *= attenuation;

    return ambient+diffuse+specular;
}

vec3 calculateSpotLight(SpotLight light){
    vec3 viewDir = normalize(vec3(viewMatrix*vec4(light.direction, 0.0f)));
    vec3 viewPos = vec3(viewMatrix*vec4(light.position, 1.0f));
    float theta = max(dot(viewDir, normalize(fragPos-viewPos)), 0);
    if(dot(-normal, normalize(fragPos-viewPos)) < 0.01){
        theta = 0.0f;
    }
    float epsilon = light.cutoff - light.outerCutOff;
    float intensity = clamp((theta-light.outerCutOff)/epsilon, 0.0f, 1.0f);
    return intensity*vec3(texture(material.diffuse, texCoords));
}

void main()
{
    vec3 result = vec3(0.0f);

    for(int i = 0; i < NR_POINT_LIGHTS; i++){
        result += calculatePointLight(pointLights[i]);
    }

    result += calculateSpotLight(spotlight);
    result += calculateDirLight(dirLight);
    FragColor = vec4(result, 1.0);

}