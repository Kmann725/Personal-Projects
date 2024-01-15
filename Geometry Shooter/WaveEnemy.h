//*****************************************************************************
// WaveEnemy.cpp
// 
// Author: Kyle Manning
// 
// Brief Description: Header for WaveEnemy objects
//*****************************************************************************
#pragma once

#include "RangedEnemy.h"
#include "Projectile.h"

const float W_RELOAD_TIME = 2.5f;

class WaveEnemy : public RangedEnemy {
public:
	WaveEnemy(glm::vec2 pos, glm::vec2 size, float rotation, glm::vec3 color, Player* player, vector<Projectile>& bullets);

protected:
	// Resets reload timer
	void ResetReload() override;
};

