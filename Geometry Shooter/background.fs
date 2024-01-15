#version 460 core
in vec2 TexCoords;

out vec4 color;

uniform sampler2D texture;
uniform sampler2D texture2;
uniform float shift;

void main() {
    color = mix(texture(texture, TexCoords), texture(texture2, TexCoords), shift);
}