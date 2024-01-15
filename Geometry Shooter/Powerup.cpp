//*****************************************************************************
// Powerup.cpp
// 
// Author: Kyle Manning
// 
// Brief Description: Contains the constructor for Powerup objects, which
//					  are randomly set to a powerup type on construction, and
//					  the method for drawing powerups on screen
//*****************************************************************************
#include "Powerup.h"

/// <summary>
/// Constructor for the Powerup class
/// </summary>
/// <param name="pos">Position of the Powerup in worldspace</param>
/// <param name="size">Scalar values for when drawing the Powerup</param>
/// <param name="rotation">Angle of rotation that the Powerup should be drawn at</param>
/// <param name="color">Color that the Powerup will be</param>
Powerup::Powerup(glm::vec2 pos, glm::vec2 size, float rotation, glm::vec3 color) : GameObject(pos, size, rotation, color) {
	this->objectShader = Shader("player.vs", "player.fs");

	this->vertices = {
	-1.0f, 1.0f,
	-1.0f, -1.0f,
	1.0f, 1.0f,

	1.0f, 1.0f,
	-1.0f, -1.0f,
	1.0f, -1.0f
	};

	InitializeVertexObjects(vertices);

	// Sets Powerup type by randomly choosing a number between 1 and 10
	int option = rand() % 10 + 1;

	switch (option) {
	case 1:
	case 2:
	case 3:
		pType = BETTER_BULLETS;
		break;
	case 4:
	case 5:
	case 6:
		pType = MULTI_SHOT;
		break;
	case 7:
	case 8:
		pType = HEALING;
		break;
	case 9:
	case 10:
		pType = TIME_STOP;
		break;
	} 
}

/// <summary>
/// Performs transformations on the model matrix, sets the Powerup's model and view matrices, and
/// draws the powerup
/// </summary>
/// <param name="view">The view matrix to be used when drawing the Powerup</param>
void Powerup::DrawPowerup(glm::mat4 view) {
	this->model = glm::mat4(1.0f);
	this->model = glm::translate(this->model, glm::vec3(this->pos, 0.0f));

	this->model = glm::rotate(this->model, glm::radians(this->rotation), glm::vec3(0.0f, 0.0f, 1.0f));

	this->model = glm::scale(this->model, glm::vec3(this->size, 1.0f));

	this->objectShader.Use();
	this->objectShader.SetMat4("model", this->model);
	this->objectShader.SetMat4("view", view);

	glBindVertexArray(this->VAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}