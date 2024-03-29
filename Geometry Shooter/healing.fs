#version 460 core
in vec2 TexCoords;

out vec4 color;

uniform sampler2D texture;

void main() {
    color = texture(texture, TexCoords);
}