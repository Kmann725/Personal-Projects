//*****************************************************************************
// Player.cpp
// 
// Author: Kyle Manning
// 
// Brief Description: Contains the constructor for Player objects and methods
//					  for updating the position of the player and projectile 
//					  spawn points, drawing the player to the screen, and
//					  taking damage
//*****************************************************************************
#include "Player.h"

/// <summary>
/// Constructor for Player objects
/// </summary>
/// <param name="pos">Postion of Player</param>
/// <param name="size">Scalar value for drawing the Player</param>
/// <param name="rotation">Angle of rotation to draw Player at</param>
/// <param name="color">Color to draw Player as</param>
Player::Player(glm::vec2 pos, glm::vec2 size, float rotation, glm::vec3 color) : GameObject(pos, size, rotation, color), speed(150.0f), vertDrct(V_NONE), horDrct(H_NONE), knockedBack(false), knockBackVel(glm::vec2(0.0f, 0.0f)), kbTimer(0.1f), health(100), damageColor(glm::vec3(0.41f, 0.39f, 0.23f)) {
	this->objectShader = Shader("player.vs", "player.fs");

	this->vertices = {
	-0.6f, -1.0f,
	0.0f, -0.5f,
	0.0f, 1.0f,

	0.0f, 1.0,
	0.0f, -0.5f,
	0.6f, -1.0f
	};

	InitializeVertexObjects(vertices);
}

/// <summary>
/// Updates the Player's postion based on what their horizontal and vertical movement
/// directions are, and whether or not they are currently experiencing knockback
/// </summary>
/// <param name="dt">Time elapsed between frames</param>
void Player::UpdatePosition(float dt) {
	if (horDrct == LEFT) {
		this->pos.x -= (speed * dt);
	}
	if (horDrct == RIGHT) {
		this->pos.x += (speed * dt);
	}
	if (vertDrct == UP) {
		this->pos.y -= (speed * dt);
	}
	if (vertDrct == DOWN) {
		this->pos.y += (speed * dt);
	}

	if (knockedBack) {
		if (kbTimer > 0) {
			this->pos += (knockBackVel * dt);
			kbTimer -= dt;
		}
		else {
			knockedBack = false;
			kbTimer = 0.1f;
			this->objectShader.Use();
			this->objectShader.SetVec3("Color", this->color);
		}
	}
}

/// <summary>
/// Sets the Player's model and view matrices and draws it to the screen
/// </summary>
/// <param name="view">View matrix used when drawing</param>
void Player::DrawPlayer(glm::mat4 view) {
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

/// <summary>
/// Updates the Player's rotation based on the positon of the mouse
/// </summary>
/// <param name="mouseX">Mouse's x in screen space</param>
/// <param name="mouseY">Mouse's y in screen space</param>
void Player::UpdateRotation(float mouseX, float mouseY) {
	this->rotation = (atan2((double)(mouseY - 300), (double)(mouseX - 400))) * (180 / 3.1415);

	if (this->rotation < 0) {
		this->rotation = 360 + rotation;
	}

	// Aligns rotation so the front of the Player points towards the mouse
	this->rotation -= 90;
}

/// <summary>
/// Updates the position of the Player's bullet spawn points
/// </summary>
void Player::UpdateBulletSpawnPosition() {
	glm::mat4 aimMatrix = glm::mat4(1.0f);
	aimMatrix = glm::translate(aimMatrix, glm::vec3(pos, 0.0f));

	aimMatrix = glm::rotate(aimMatrix, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));
	aimMatrix = glm::translate(aimMatrix, glm::vec3(0.0f, 30.0f, 0.0f));
	bulletSpawn = glm::vec2(aimMatrix[3][0], aimMatrix[3][1]);

	// shiftedMatrix and otherShift are the left and right spawn positions used for multi-shot
	glm::mat4 shiftedMatrix = glm::mat4(1.0f);
	shiftedMatrix = glm::translate(shiftedMatrix, glm::vec3(pos, 0.0f));
	glm::mat4 otherShift = shiftedMatrix;
	shiftedMatrix = glm::rotate(shiftedMatrix, glm::radians(rotation - 30), glm::vec3(0.0f, 0.0f, 1.0f));
	shiftedMatrix = glm::translate(shiftedMatrix, glm::vec3(0.0f, 30.0f, 0.0f));
	this->shiftedLeftSpawn = glm::vec2(shiftedMatrix[3][0], shiftedMatrix[3][1]);

	otherShift = glm::rotate(otherShift, glm::radians(rotation + 30), glm::vec3(0.0f, 0.0f, 1.0f));
	otherShift = glm::translate(otherShift, glm::vec3(0.0f, 30.0f, 0.0f));
	this->shiftedRightSpawn = glm::vec2(otherShift[3][0], otherShift[3][1]);
}

/// <summary>
/// Subtracts damage from the Player's health and find the direction vector between the Player
/// and damage source for knockback to be applied to
/// </summary>
/// <param name="damageSource">Position of object that caused Player damgage</param>
/// <param name="damage">Amount of damage taken</param>
void Player::TakeDamage(glm::vec2 damageSource, int damage) {
	this->health -= damage;

	glm::vec2 direction = glm::vec2(this->pos.x - damageSource.x, this->pos.y - damageSource.y);
	float length = sqrt(pow(direction.x, 2) + pow(direction.y, 2));
	knockBackVel = glm::vec2(direction.x / length, direction.y / length) * glm::vec2(350.0f, 350.0f);

	knockedBack = true;

	// Changes Player color for feedback on receiving damage
	this->objectShader.Use();
	this->objectShader.SetVec3("Color", damageColor);
}

/// <summary>
/// Adds to the Player's health value
/// </summary>
/// <param name="heal">Amount added to the Player's health</param>
void Player::AddHealth(int heal) {
	this->health += heal;
}