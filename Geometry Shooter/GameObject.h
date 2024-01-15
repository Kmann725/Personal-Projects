//*****************************************************************************
// GameObject.cpp
// 
// Author: Kyle Manning
// 
// Brief Description: Header for GameObject objects
//*****************************************************************************
#pragma once

#include <vector>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "Shader.h"

class GameObject {
public:
	unsigned int VBO, VAO;
	float rotation;
	glm::vec2 pos, size;
	glm::vec3 color;
	glm::mat4 model, projection;
	vector<float> vertices;
	Shader objectShader;

	GameObject();
	GameObject(glm::vec2 pos, glm::vec2 size, float rotation, glm::vec3 color);

protected:
	// Handles binding vertices to the object's vertex buffer and array objects
	void InitializeVertexObjects(vector<float> vertices);
};

