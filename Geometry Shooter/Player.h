//*****************************************************************************
// Player.h
// 
// Author: Kyle Manning
// 
// Brief Description: Header for Player objects
//*****************************************************************************
#pragma once

#include "GameObject.h"
#include <GLFW/glfw3.h>

#include "Enemy.h"

// What direction of vertical movement the Player has
enum VerticalDirection {
	UP,
	DOWN,
	V_NONE
};

// What direction of horizontal movement the Player has
enum HorizontalDirection {
	LEFT,
	RIGHT,
	H_NONE
};

class Player : public GameObject {
public:
	bool knockedBack;
	int health;
	float speed, kbTimer;
	glm::vec2 bulletSpawn, knockBackVel, shiftedLeftSpawn, shiftedRightSpawn;
	glm::vec3 damageColor;
	VerticalDirection vertDrct;
	HorizontalDirection horDrct;

	Player(glm::vec2 pos, glm::vec2 size, float rotation, glm::vec3 color);

	// Draws the Player on screen
	void DrawPlayer(glm::mat4 view);

	// Updates the position and rotation of the Player
	void UpdatePosition(float dt);
	void UpdateRotation(float mouseX, float mouseY);

	// Updates the postion of the Player's projectile spawn points
	void UpdateBulletSpawnPosition();

	// The Player loses health and the direction vector for applying knockback is found
	void TakeDamage(glm::vec2 damageSource, int damage);

	// Adds health to the Player
	void AddHealth(int heal);
};

