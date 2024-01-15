//*****************************************************************************
// GameObject.cpp
// 
// Author: Kyle Manning
// 
// Brief Description: Contains the constructor for GameObject objects, and 
//					  a method for intializing their vertex buffer and array
//					  objects
//*****************************************************************************
#include "GameObject.h"

/// <summary>
/// Default constructor for GameObjects
/// </summary>
GameObject::GameObject() : pos(0.0f, 0.0f), size(10.0f, 10.0f), rotation(0.0f), color(1.0f, 1.0f, 1.0f), model(glm::mat4(1.0f)), projection(glm::mat4(1.0f)) {
}

/// <summary>
/// Constructor for GameObjects
/// </summary>
/// <param name="pos">Position where the object should be spawned in world space</param>
/// <param name="size">Scalar values for drawing the object</param>
/// <param name="rotation">Angle of rotation the object should be drawn at</param>
/// <param name="color">Color that the GameObject should be</param>
GameObject::GameObject(glm::vec2 pos, glm::vec2 size, float rotation, glm::vec3 color) : pos(pos), size(size), rotation(rotation), color(color), model(glm::mat4(1.0f)) {
}

/// <summary>
/// Takes a vector of vertices and uses them in intializing the vertex buffer and array
/// objects, then sets initial values for the object's model and projection matrices
/// </summary>
/// <param name="vertices">Set of vertices to be used </param>
void GameObject::InitializeVertexObjects(vector<float> vertices) {
	glGenVertexArrays(1, &this->VAO);
	glGenBuffers(1, &this->VBO);

	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices.front(), GL_STATIC_DRAW);

	glBindVertexArray(this->VAO);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	this->objectShader.Use();
	this->model = glm::translate(this->model, glm::vec3(this->pos, 0.0f));
	this->model = glm::rotate(this->model, glm::radians(this->rotation), glm::vec3(0.0f, 0.0f, 1.0f));
	this->model = glm::scale(this->model, glm::vec3(this->size, 1.0f));
	this->objectShader.SetMat4("model", this->model);

	glm::mat4 projection = glm::ortho(0.0f, 800.0f, 600.0f, 0.0f, -1.0f, 1.0f);
	this->objectShader.SetMat4("projection", projection);

	this->objectShader.SetVec3("Color", this->color);
}