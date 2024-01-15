//*****************************************************************************
// RangedEnemy.cpp
// 
// Author: Kyle Manning
// 
// Brief Description: Header for RangedEnemy objects
//*****************************************************************************
#pragma once

#include "Enemy.h"
#include "Projectile.h"

const float RELOAD_TIME = 1.5f;

class RangedEnemy : public Enemy {
public:
	bool waveType;
	float followDistance, reloadTime, bulletSpeed;
	glm::vec2 bulletSpawn, bulletSize;
	glm::vec3 bulletColor;
	vector<Projectile>& bullets;

	RangedEnemy(glm::vec2 pos, glm::vec2 size, float rotation, glm::vec3 color, Player* player, vector<Projectile>& bullets);

	// Updates the postion of the RangedEnemy
	void UpdatePosition(float dt) override;

protected:
	// Spawns a projectile at the RangedEnemy's projectile spawn point
	void CreateProjectile();

	// Updates the location of the projectile spawn point
	void UpdateBulletSpawn();

	// Resets the reload timer 
	virtual void ResetReload();
};

