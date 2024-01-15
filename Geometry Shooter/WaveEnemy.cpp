//*****************************************************************************
// WaveEnemy.cpp
// 
// Author: Kyle Manning
// 
// Brief Description: Contains the constructor for WaveEnemy objects,
//					  inherits and overrides methods from Enemy and RangedEnemy
//*****************************************************************************
#include "WaveEnemy.h"

/// <summary>
/// Constructor for WaveEnemy objects
/// </summary>
/// <param name="pos">Postion of the WaveEnemy</param>
/// <param name="size">Scalar value used when drawing</param>
/// <param name="rotation">Angle of rotation to draw WaveEnemy at</param>
/// <param name="color">Color of WaveEnemy</param>
/// <param name="player">Player object in the scene</param>
/// <param name="bullets">Vector of enemy projectiles used in the Game class</param>
WaveEnemy::WaveEnemy(glm::vec2 pos, glm::vec2 size, float rotation, glm::vec3 color, Player* player, vector<Projectile>& bullets) : RangedEnemy(pos, size, rotation, color, player, bullets) {
	this->health = 80;
	this->speed = 75.0f;
	this->pointValue = 25;
	this->bulletSize = glm::vec2(10.0f, 10.0f);
	this->followDistance = 300.0f;
	this->reloadTime = W_RELOAD_TIME;
	this->bulletSpeed = 100.0f;
	this->bulletColor = glm::vec3(0.62f, 0.005f, 0.59f);
	this->waveType = true;

	this->vertices = {
	-1.0f, -1.0f,
	1.0f, -1.0f,
	-0.6f, 1.0f,

	-0.6f, 1.0f,
	1.0f, -1.0f,
	0.6f, 1.0f
	};

	InitializeVertexObjects(vertices);
}

/// <summary>
/// Resets the value of the reload timer
/// </summary>
void WaveEnemy::ResetReload() {
	this->reloadTime = W_RELOAD_TIME;
}