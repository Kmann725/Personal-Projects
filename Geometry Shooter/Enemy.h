//*****************************************************************************
// Enemy.h
// 
// Author: Kyle Manning
// 
// Brief Description: Header for Enemy objects
//*****************************************************************************
#pragma once

#include "GameObject.h"

class Player;

const float COLOR_RESET_TIME = 0.1f;

class Enemy : public GameObject{
public:
	int health, pointValue, attack;
	float colorResetTime, speed;
	glm::vec2 velocity;
	glm::vec3 damageColor;
	glm::vec3 currentColor;
	Player* player;

	Enemy(glm::vec2 pos, glm::vec2 size, float rotation, glm::vec3 color, Player* player);

	// Updates the position of the Enemy
	virtual void UpdatePosition(float dt);

	// Draws the Enemy on the screen
	void DrawEnemy(glm::mat4 view);

	// Decreases the Enemy's health value
	void TakeDamage(int damage);

	// Returns the Enemy's point value
	int GetPointValue();

protected:
	// Updates the Enemy's rotation and direction vector
	void UpdateVelocity();
	void UpdateRotation();
};

