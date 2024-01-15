//*****************************************************************************
// Projectile.cpp
// 
// Author: Kyle Manning
// 
// Brief Description: Contains the constructor for Projectile objects, and
//					  methods for updating the object's postion and drawing it
//*****************************************************************************
#include "Projectile.h"

/// <summary>
/// Constructor for Projectiles
/// </summary>
/// <param name="pos">Starting postion of the projectile</param>
/// <param name="size">Scalar value for drawing the object</param>
/// <param name="rotation">Angle of rotation to draw object at</param>
/// <param name="velocity">Velocity vector Projectile travels at</param>
/// <param name="color">Color to draw Projectile as</param>
/// <param name="damage">Damage value of the projectile</param>
/// <param name="isWave">Whether or not the object is a wave-type projectile</param>
Projectile::Projectile(glm::vec2 pos, glm::vec2 size, float rotation, glm::vec2 velocity, glm::vec3 color, int damage, bool isWave) : GameObject(pos, size, rotation, color), velocity(velocity), lifeTime(5.0f), hasExpired(false), damage(damage), isWave(isWave), waveHealth(30) {
	this->objectShader = Shader("projectile.vs", "projectile.fs");

	this->vertices = {
	-1.0f, 1.0f,
	-1.0f, -1.0f,
	1.0f, 1.0f,

	1.0f, 1.0f,
	-1.0f, -1.0f,
	1.0f, -1.0f
	};

	InitializeVertexObjects(this->vertices);
}

/// <summary>
/// Sets the model and view matrices and draws the object to the screen
/// </summary>
/// <param name="view">View matrix used when drawing</param>
void Projectile::DrawProjectile(glm::mat4 view) {
	this->model = glm::mat4(1.0f);
	this->model = glm::translate(this->model, glm::vec3(this->pos, 0.0f));

	this->model = glm::translate(this->model, glm::vec3(0.0f, -0.1f, 0.0f));
	this->model = glm::rotate(this->model, glm::radians(this->rotation), glm::vec3(0.0f, 0.0f, 1.0f));
	this->model = glm::translate(this->model, glm::vec3(0.0f, 0.1f, 0.0f));

	this->model = glm::scale(this->model, glm::vec3(this->size, 1.0f));
	
	this->objectShader.Use();
	this->objectShader.SetMat4("model", this->model);
	this->objectShader.SetMat4("view", view);

	glBindVertexArray(this->VAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

/// <summary>
/// Updates the positon of the Projectile by adding its velocity to its position, and
/// marks itself to be erased once lifeTime hits zero
/// </summary>
/// <param name="dt">Amount of time elapsed between frames</param>
void Projectile::UpdatePosition(float dt) {
	this->pos += this->velocity * dt;
	this->lifeTime -= dt;

	if (this->lifeTime <= 0) {
		this->hasExpired = true;
	}

	// Wave-type projectiles grow in size over time
	if (isWave) {
		this->size.x += (20.0f * dt);
	}
}

/// <summary>
/// Sets the health of the projectile (only used when isWave is true)
/// </summary>
/// <param name="health">Amount of health lost</param>
void Projectile::SetWaveHealth(int health) {
	this->waveHealth = health;
}