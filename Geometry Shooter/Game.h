//*****************************************************************************
// Game.h
// 
// Author: Kyle Manning
// 
// Brief Description: Header for Game objects
//*****************************************************************************
#pragma once

#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include "stb_image.h"
#include "Shader.h"
#include "Player.h"
#include "Enemy.h"
#include "RangedEnemy.h"
#include "WaveEnemy.h"
#include "Projectile.h"
#include "Powerup.h"
#include "TextRenderer.h"

#include <ft2build.h>
#include FT_FREETYPE_H

// Game states
enum GameState {
	GAME_TITLE,
	GAME_LOAD,
	GAME_ACTIVE,
	GAME_WIN,
	GAME_LOSS
};

// Powerup states the player can have
enum PowerupState {
	P_BETTER_BULLETS,
	P_MULTI_SHOT,
	P_TIME_STOP,
	P_HEALING,
	P_NONE
};

const glm::vec2 PLAYER_SIZE(30.0f, 30.0f);
const glm::vec2 PROJECTILE_SIZE(5.0f, 5.0f);
const glm::vec2 BETTER_PROJ_SIZE(7.0f, 7.0f);
const glm::vec2 ENEMY_SIZE(25.0f, 25.0f);
const glm::vec2 WAVE_ENEMY_SIZE(50.0f, 25.0f);
const glm::vec2 POWERUP_SIZE(15.0f, 15.0f);

const glm::vec3 N_ENEMY_COLOR(0.0f, 0.0f, 1.0f);
const glm::vec3 R_ENEMY_COLOR(0.98f, 0.96f, 0.18f);
const glm::vec3 W_ENEMY_COLOR(1.0f, 0.43f, 0.0f);

const float POWER_UP_TIME = 10.0f;
const float SPAWN_PAUSE = 5.0f;
const float WAVE_START_DELAY = 7.0f;

class Game {
public:
	bool Keys[1024];
	int score, comboNumber, powerupSpawnChance, waveCount;
	unsigned int Width, Height;
	unsigned int backgroundVBO, backgroundVAO, healingVBO, healingVAO, timestopVBO, timestopVAO, fbo;
	unsigned int backgroundTex, background2, background3, background4, grayscaleTex, healingTex;
	float mouseX, mouseY;
	float scoreMultiplier, powerUpTimer, waveCountDown, spawnPauseTimer, backgroundShift;
	string powerUpDisplay;
	glm::mat4 view;
	GameState State;
	PowerupState pState;
	Shader backgroundShader, grayScaleShader, healingShader;

	Game(unsigned int width, unsigned int height);
	~Game();

	// Initialzes objects and shaders needed at game start
	void Init();

	// Handles keyboard inputs from the player
	void ProcessInput();

	// Calls object updates and handles the game loop
	void Update(float dt);

	// Main render loop, handles calls for objects to be drawn
	void Render();

	// Updates variables that store mouse position
	void SetMousePos(float xPos, float yPos);

	// Spawns a player projectile
	void CreateBullet();

private:
	// Initializes shader and textures for the background
	void InitializeBackground();

	// Initializes shader, vertex objects, and texture for the healing vignette
	void InitializeHealingVignette();

	// Initializes shader, texture, vertex objects, and framebuffer for the time stop filter
	void InitializeTimeStopFilter();

	// Loads textures from image files into unsigned ints
	void LoadTexture(string imageFile, unsigned int& texture);

	// Draws text UI to the screen
	void DrawUI();

	// Draws the background on the screen
	void DrawBackground();

	// Gradually switches between two background images
	void ChangeBackground(float dt);

	// Checks for and resolves collsions between game objects
	void CheckCollisions();

	// Calculates and returns the direction vector between two points
	glm::vec2 CalculateDirectionVector(glm::vec2 pos1, glm::vec2 pos2);

	// Increases the player's score
	void IncreaseScore(int points);

	// Spawns a Powerup
	void SpawnPowerup(glm::vec2 pos);

	// Handles spawning waves of enemies
	void SpawnWave(float dt, vector<int>& wave);

	// Spawns an Enemy
	void SpawnEnemy(int enemyType);

	// Checks to see if a wave has been completed
	void CheckWaveEnd();
};

