//*****************************************************************************
// Enemy.cpp
// 
// Author: Kyle Manning
// 
// Brief Description: Contains the constructor for Enemy objects and methods
//					  for updating the position of and drawing Enemy objeccts,
//					  as well as taking damage and returning its point value
//*****************************************************************************
#include "Enemy.h"
#include "Player.h"

/// <summary>
/// Constructor for Enemy objects
/// </summary>
/// <param name="pos">Position of Enemy</param>
/// <param name="size">Scalar value for drawing the Enemy</param>
/// <param name="rotation">Angle of rotation to draw Enemy at</param>
/// <param name="color">Color of Enemy</param>
/// <param name="player">Player object in the scene</param>
Enemy::Enemy(glm::vec2 pos, glm::vec2 size, float rotation, glm::vec3 color, Player* player) : GameObject(pos, size, rotation, color), player(player), health(50), pointValue(10), attack(10), damageColor(glm::vec3(0.5f, 0.18f, 0.35f)), currentColor(color), colorResetTime(COLOR_RESET_TIME), speed(165.0f) {
	this->objectShader = Shader("player.vs", "player.fs");

	this->vertices = {
	-1.0f, -1.0f,
	1.0f, 1.0f,
	-1.0f, 1.0f,

	-1.0f, -1.0f,
	1.0f, -1.0f,
	1.0f, 1.0f
	};

	InitializeVertexObjects(vertices);
}

/// <summary>
/// Updates the Enemy's position and deal's damage to the player if close enough
/// </summary>
/// <param name="dt">Time elapsed between frames</param>
void Enemy::UpdatePosition(float dt) {
	UpdateVelocity();
	UpdateRotation();

	this->pos += velocity * dt;

	// Delivers damage and knockback to the player upon "collision"
	if (sqrt(pow(player->pos.x - pos.x, 2) + pow(player->pos.y - pos.y, 2)) < 35.0f) {
		player->TakeDamage(this->pos, attack);
	}

	if (this->currentColor == damageColor) {
		this->colorResetTime -= dt;

		// Returns the enemy to its normal color a set time after taking damage
		if (this->colorResetTime <= 0) {
			this->objectShader.Use();
			this->objectShader.SetVec3("Color", this->color);
			this->currentColor = this->color;
			this->colorResetTime = COLOR_RESET_TIME;
		}
	}
}

/// <summary>
/// Transform's the Enemy's model matrix, sets the model and view matrices, and draws the
/// Enemy to the screen
/// </summary>
/// <param name="view">View matrix used in drawing</param>
void Enemy::DrawEnemy(glm::mat4 view) {
	this->model = glm::mat4(1.0f);
	this->model = glm::translate(this->model, glm::vec3(this->pos, 0.0f));

	this->model = glm::rotate(this->model, glm::radians(this->rotation), glm::vec3(0.0f, 0.0f, 1.0f));

	this->model = glm::scale(this->model, glm::vec3(this->size, 1.0f));

	this->objectShader.Use();
	this->objectShader.SetMat4("model", this->model);
	this->objectShader.SetMat4("view", view);

	glBindVertexArray(this->VAO);
	glDrawArrays(GL_TRIANGLES, 0, vertices.size() / 2);
}

/// <summary>
/// Updates the Enemy's velocity vector by getting the direction vector between its current postion
/// and the player's position
/// </summary>
void Enemy::UpdateVelocity() {
	glm::vec2 direction = glm::vec2(player->pos.x - this->pos.x, player->pos.y - this->pos.y);
	float length = sqrt(pow(direction.x, 2) + pow(direction.y, 2));
	velocity = glm::vec2(direction.x / length, direction.y / length) * glm::vec2(speed, speed);
}

/// <summary>
/// Updates the Enemy's rotation based on the angle between its current postion and the player's
/// </summary>
void Enemy::UpdateRotation() {
	this->rotation = (atan2((double)(player->pos.y - this->pos.y), (double)(player->pos.x - this->pos.x))) * (180 / 3.1415);

	if (this->rotation < 0) {
		this->rotation = 360 + this->rotation;
	}
	// Adjusts rotation so the Enemy's front matches their movement direction
	this->rotation -= 90;
}

/// <summary>
/// Subtracts damage value from the Enemy's health count
/// </summary>
/// <param name="damage">Amount of health to be lost</param>
void Enemy::TakeDamage(int damage) {
	health -= damage;

	// Changes enemy color for feedback on receiving damage
	this->objectShader.Use();
	this->objectShader.SetVec3("Color", damageColor);
	this->currentColor = damageColor;
}

/// <summary>
/// Returns the value of the Enemy's pointValue variable
/// </summary>
/// <returns>Enemy point value</returns>
int Enemy::GetPointValue() {
	return pointValue;
}