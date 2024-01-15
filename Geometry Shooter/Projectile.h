//*****************************************************************************
// Projectile.h
// 
// Author: Kyle Manning
// 
// Brief Description: Header for Projectile objects
//*****************************************************************************
#pragma once

#include "GameObject.h"
#include <GLFW/glfw3.h>

#include "Shader.h"

class Projectile : public GameObject {
public:
	bool isWave, hasExpired;
	int damage, waveHealth;
	float lifeTime;
	glm::vec2 velocity;

	Projectile(glm::vec2 pos, glm::vec2 size, float rotation, glm::vec2 velocity, glm::vec3 color, int damage, bool isWave);

	// Draws object to the screen
	void DrawProjectile(glm::mat4 view);

	// Updates the position of the object
	void UpdatePosition(float dt);

	// Sets the health of the projectile (only used for wave-type projectiles)
	void SetWaveHealth(int health);
};

