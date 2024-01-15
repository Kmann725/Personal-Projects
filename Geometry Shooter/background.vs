#version 460 core
layout (location = 0) in vec2 aVertex;
layout (location = 1) in vec2 aTexCoords;

out vec2 TexCoords;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

void main() {
	gl_Position = projection * view * model * vec4(aVertex, 0.0, 1.0);
	TexCoords = aTexCoords;
}