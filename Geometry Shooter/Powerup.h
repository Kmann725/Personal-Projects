//*****************************************************************************
// Powerup.h
// 
// Author: Kyle Manning
// 
// Brief Description: Header for Powerup objects
//*****************************************************************************
#pragma once

#include "GameObject.h"

// What type of Powerup each object is
enum PType {
	BETTER_BULLETS,
	MULTI_SHOT,
	TIME_STOP,
	HEALING
};

class Powerup : public GameObject {
public:
	PType pType;

	Powerup(glm::vec2 pos, glm::vec2 size, float rotation, glm::vec3 color);

	// Draws the Powerup to the screen
	void DrawPowerup(glm::mat4 view);
};

