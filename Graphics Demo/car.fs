#version 460 core
out vec4 color;

in vec3 FragPos;
in vec3 Normal;

uniform vec3 Color;
uniform vec3 LightPos;
uniform vec3 LightColor;

void main() {
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * LightColor;

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(LightPos - FragPos);

    float diff = max(dot(norm, lightDir), 0.0f);
    vec3 diffuse = diff * LightColor;

    vec3 result = (ambient + diffuse) * Color;
    color = vec4(result, 1.0);
}