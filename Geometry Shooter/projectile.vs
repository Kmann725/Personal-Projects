#version 460 core
layout (location = 0) in vec2 aVertex;

uniform mat4 model;
uniform mat4 projection;
uniform mat4 view;

void main() {
	gl_Position = projection * view * model * vec4(aVertex, 0.0, 1.0);
}