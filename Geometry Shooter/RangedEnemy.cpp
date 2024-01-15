//*****************************************************************************
// RangedEnemy.cpp
// 
// Author: Kyle Manning
// 
// Brief Description: Contains the constructor for RangedEnemy objects,
//					  inherits and overrides methods from Enemy and contains
//					  methods for firing projectiles at the player
//*****************************************************************************
#include "RangedEnemy.h"
#include "Player.h"

/// <summary>
/// Constructor for RangedEnemy objects
/// </summary>
/// <param name="pos">Positon of RangedEnemy</param>
/// <param name="size">Scalar value for drawing</param>
/// <param name="rotation">Angle of rotation to draw RangedEnemy at</param>
/// <param name="color">Color of RangedEnemy</param>
/// <param name="player">Player object in the scene</param>
/// <param name="bullets">Vector of enemy projectiles used in Game class</param>
RangedEnemy::RangedEnemy(glm::vec2 pos, glm::vec2 size, float rotation, glm::vec3 color, Player* player, vector<Projectile>& bullets) : Enemy(pos, size, rotation, color, player), reloadTime(RELOAD_TIME), bullets(bullets), bulletSize(10.0f, 10.0f), followDistance(250.0f), bulletSpeed(225.0f), waveType(false), bulletColor(0.55f, 0.075f, 0.075f) {
	this->health = 40;
	this->pointValue = 15;
	this->speed = 100.0f;

	this->vertices = {
	-1.0f, 0.25f,
	-0.65f, -1.0f,
	0.65f, -1.0f,

	-1.0f, 0.25f,
	0.65f, -1.0f,
	1.0f, 0.25f,

	-1.0f, 0.25f,
	1.0f, 0.25f,
	0.0f, 1.0f
	};

	InitializeVertexObjects(vertices);
}

/// <summary>
/// Updates the postion of the RangedEnemy, moving to within a distance of the player and firing
/// projectiles with a reload timer between shots
/// </summary>
/// <param name="dt">Time elapsed between frames</param>
void RangedEnemy::UpdatePosition(float dt) {
	UpdateVelocity();
	UpdateRotation();

	// Only moves if the player is farther away than the RangedEnemy's follow distance
	if (sqrt(pow(player->pos.x - pos.x, 2) + pow(player->pos.y - pos.y, 2)) > followDistance) {
		this->pos += velocity * dt;
	}

	// Delivers damage and knockback to the player upon "collision"
	if (sqrt(pow(player->pos.x - pos.x, 2) + pow(player->pos.y - pos.y, 2)) < 35.0f) {
		player->TakeDamage(this->pos, attack);
	}

	this->reloadTime -= dt;

	if (reloadTime <= 0) {
		CreateProjectile();
		ResetReload();
	}

	// Resets the RangedEnemy's color after taking damage
	if (this->currentColor == damageColor) {
		this->colorResetTime -= dt;

		if (this->colorResetTime <= 0) {
			this->objectShader.Use();
			this->objectShader.SetVec3("Color", this->color);
			this->currentColor = this->color;
			this->colorResetTime = COLOR_RESET_TIME;
		}
	}
}

/// <summary>
/// Updates the projectile spawn point and gets the direction vector between it and the player's postion,
/// then creates a projectile at the spawn postion with that direction vector
/// </summary>
void RangedEnemy::CreateProjectile() {
	UpdateBulletSpawn();

	glm::vec2 direction = glm::vec2(player->pos.x - this->bulletSpawn.x, player->pos.y - this->bulletSpawn.y);
	float length = sqrt(pow(direction.x, 2) + pow(direction.y, 2));
	direction = glm::vec2(direction.x / length, direction.y / length);

	// Adds projectile to Game's enemy projectile vector
	bullets.push_back(Projectile(glm::vec2(this->bulletSpawn.x, this->bulletSpawn.y), bulletSize, this->rotation, glm::vec2(bulletSpeed, bulletSpeed) * direction, bulletColor, 10,this->waveType));
}

/// <summary>
/// Updates the postion of the RangedEnemy's bullet spawn point
/// </summary>
void RangedEnemy::UpdateBulletSpawn() {
	glm::mat4 aimMatrix = glm::mat4(1.0f);
	aimMatrix = glm::translate(aimMatrix, glm::vec3(this->pos, 0.0f));

	aimMatrix = glm::rotate(aimMatrix, glm::radians(this->rotation), glm::vec3(0.0f, 0.0f, 1.0f));
	aimMatrix = glm::translate(aimMatrix, glm::vec3(0.0f, 30.0f, 0.0f));
	this->bulletSpawn = glm::vec2(aimMatrix[3][0], aimMatrix[3][1]);
}

/// <summary>
/// Resets the value of the reload timer
/// </summary>
void RangedEnemy::ResetReload() {
	this->reloadTime = RELOAD_TIME;
}