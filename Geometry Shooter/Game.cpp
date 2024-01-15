//*****************************************************************************
// Game.cpp
// 
// Author: Kyle Manning
// 
// Brief Description: Contains the constructor for Game objects and methods
//					  making update and draw calls to all objects in the scene,
//					  handling collsions, drawing UI, managing powerup states,
//					  and handling game flow
//*****************************************************************************
#include "Game.h"

Player* player;
TextRenderer* titleRenderer;
TextRenderer* uiRenderer;

vector<Projectile*> playerBullets;
vector<Enemy*> enemies;
vector<Projectile> enemyBullets;
vector<Powerup*> powerups;

float backGroundVerts[] = {
	-1.0f, 1.0f, 0.0f, 30.0f,
	1.0f, -1.0f, 30.0f, 0.0f,
	-1.0f, -1.0f, 0.0f, 0.0f,

	-1.0f, 1.0f, 0.0f, 30.0f,
	1.0f, 1.0f, 30.0f, 30.0f,
	1.0f, -1.0f, 30.0f, 0.0f
};

float grayScaleVerts[]{
	-1.0f,  1.0f,  0.0f, 1.0f,
	-1.0f, -1.0f,  0.0f, 0.0f,
	1.0f, -1.0f,  1.0f, 0.0f,

	-1.0f,  1.0f,  0.0f, 1.0f,
	1.0f, -1.0f,  1.0f, 0.0f,
	1.0f,  1.0f,  1.0f, 1.0f
};

float loadTime = 1.0f;
float winTime = 2.0f;
float comboResetTime = 5.0f;

// Vector sets used in drawing UI
vector<string> healthStrings = {
	"Health: ",
	"100"
};

vector<glm::vec3> healthColors = {
	glm::vec3(1.0f, 0.0f, 0.0f),
	glm::vec3(1.0f, 1.0f, 1.0f)
};

vector<string> scoreStrings = {
	"Score: ",
	"0"
};

vector<glm::vec3> scoreColors = {
	glm::vec3(0.95f, 0.43f, 0.09f),
	glm::vec3(1.0f, 1.0f, 1.0f)
};

vector<string> comboStrings = {
	"Combo: ",
	"0x"
};

vector<glm::vec3> comboColors = {
	glm::vec3(0.95f, 0.43f, 0.09f),
	glm::vec3(1.0f, 1.0f, 1.0f)
};

/* Vectors that define compostion of each wave
   1 = Normal enemy
   2 = Ranged enemy
   3 = Wave enemy
   0 = 5 second pause in spawning
*/
vector<int> wave1 = {
	1, 1, 1,
	0,
	1, 2, 1, 2, 1,
	0,
	1, 1, 1,
	0,
	0,
	3, 1, 1, 2, 1, 1, 3
};

vector<int> wave2 = {
	2, 1, 2,
	0,
	2, 2, 2, 2,
	0,
	0,
	3, 3, 3, 3,
	0,
	0,
	1, 3, 2, 3, 1, 3
};

vector<int> wave3 = {
	1, 1, 1,
	0,
	1, 1, 1, 1,
	0,
	0,
	2, 2, 2,
	0,
	2, 2, 2, 2,
	0,
	0,
	3, 3, 3,
	0,
	3, 3, 3, 3
};

vector<int> wave4 = {
	1, 3, 3, 1,
	0,
	0,
	2, 2, 2,
	0,
	1, 1, 3, 1, 1,
	0,
	0,
	1, 2, 3, 2, 1, 2, 3, 2, 1
};

/// <summary>
/// Default constructor for Game objects
/// </summary>
/// <param name="width">Width of the window</param>
/// <param name="height">Height of the window</param>
Game::Game(unsigned int width, unsigned int height) : State(GAME_TITLE), pState(P_NONE), Keys(), Width(width), Height(height), score(0), comboNumber(0), scoreMultiplier(1.0f), powerUpTimer(POWER_UP_TIME), powerupSpawnChance(20), waveCount(0), waveCountDown(5.0f), spawnPauseTimer(SPAWN_PAUSE), backgroundShift(0.0f) {

}

Game::~Game() {
	delete player;
}

/// <summary>
/// Initializes data for the game by creating the player and TextRenderers, as well as
/// calling to initialize the background
/// </summary>
void Game::Init() {
	player = new Player(glm::vec2(400.0f, 350.0f), PLAYER_SIZE, 0.0f, glm::vec3(0.0f, 0.8f, 0.0f));

	uiRenderer = new TextRenderer("bahnschrift.ttf", 48);
	titleRenderer = new TextRenderer("arial.ttf", 72);

	InitializeBackground();
	InitializeHealingVignette();
	InitializeTimeStopFilter();

	view = glm::mat4(1.0f);
}

/// <summary>
/// Creates a background Shader object, sets the vertex buffer and array for the background,
/// loads the background textures, and sets the initial uniform values for the background
/// </summary>
void Game::InitializeBackground() {
	backgroundShader = Shader("background.vs", "background.fs");

	glGenVertexArrays(1, &backgroundVAO);
	glGenBuffers(1, &backgroundVBO);

	glBindBuffer(GL_ARRAY_BUFFER, backgroundVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(backGroundVerts), backGroundVerts, GL_STATIC_DRAW);

	glBindVertexArray(backgroundVAO);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);

	// Loads the background textures that will be switched between as rounds progress
	LoadTexture("background.jpg", backgroundTex);
	LoadTexture("purple_background.png", background2);
	LoadTexture("green_background.jpg", background3);
	LoadTexture("red_background.jpg", background4);
	backgroundShader.Use();

	glm::mat4 backModel = glm::mat4(1.0f);
	backModel = glm::translate(backModel, glm::vec3(0.0f, 0.0f, 0.0f));
	backModel = glm::scale(backModel, glm::vec3(15000.0f, 15000.0f, 0.0f));

	backgroundShader.SetInt("texture", 0);
	backgroundShader.SetInt("texture2", 1);
	backgroundShader.SetFloat("shift", backgroundShift);
	backgroundShader.SetMat4("model", backModel);
	glm::mat4 projection = glm::ortho(0.0f, 800.0f, 600.0f, 0.0f, -1.0f, 1.0f);
	backgroundShader.SetMat4("projection", projection);
}

/// <summary>
/// Creates a Shader object for the healing vignette, initializes the vertex buffer and array, and generates the texture
/// used for it
/// </summary>
void Game::InitializeHealingVignette() {
	healingShader = Shader("healing.vs", "healing.fs");

	glGenVertexArrays(1, &healingVAO);
	glGenBuffers(1, &healingVBO);

	glBindBuffer(GL_ARRAY_BUFFER, healingVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(grayScaleVerts), grayScaleVerts, GL_STATIC_DRAW);

	glBindVertexArray(healingVAO);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
	glEnableVertexAttribArray(1);

	LoadTexture("healing.png", healingTex);

	healingShader.Use();
	healingShader.SetInt("texture", 4);
}

/// <summary>
/// Creates a Shader object for the time stop filter, intializes the vertex buffer and array for
/// the Shader, and generates a texture that will be drawn to with a bound framebuffer
/// </summary>
void Game::InitializeTimeStopFilter() {
	grayScaleShader = Shader("timeStop.vs", "timeStop.fs");

	glGenVertexArrays(1, &timestopVAO);
	glGenBuffers(1, &timestopVBO);
	glBindVertexArray(timestopVAO);
	glBindBuffer(GL_ARRAY_BUFFER, timestopVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(grayScaleVerts), &grayScaleVerts, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));

	glGenFramebuffers(1, &fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo);

	glGenTextures(1, &grayscaleTex);
	glBindTexture(GL_TEXTURE_2D, grayscaleTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 800, 600, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, grayscaleTex, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

/// <summary>
/// Generates a texture from the provided imageFile and sets it to the referenced unsigned int
/// </summary>
/// <param name="imageFile">File name/path of the image used</param>
/// <param name="texture">Unsigned int to assign texture to</param>
void Game::LoadTexture(string imageFile, unsigned int& texture) {
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	stbi_set_flip_vertically_on_load(true);
	int width, height, nrChannels;
	const char* c = imageFile.c_str();
	unsigned char* data = stbi_load(c, &width, &height, &nrChannels, 0);

	if (nrChannels == 3) {
		if (data) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else {
			cout << "Failed to load texture" << endl;
		}
	}
	else if (nrChannels == 4) {
		if (data) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
			glGenerateMipmap(GL_TEXTURE_2D);
		}
		else {
			cout << "Failed to load texture" << endl;
		}
	}

	stbi_image_free(data);
}

/// <summary>
/// Handles keyboard inputs by setting the Player's direction enums based on W/A/S/D inputs
/// and changes the game state to loading when Enter is pressed
/// </summary>
void Game::ProcessInput() {
	if (Keys[GLFW_KEY_A]) {
		player->horDrct = LEFT;
	}
	if (Keys[GLFW_KEY_D]) {
		player->horDrct = RIGHT;
	}
	if (Keys[GLFW_KEY_W]) {
		player->vertDrct = UP;
	}
	if (Keys[GLFW_KEY_S]) {
		player->vertDrct = DOWN;
	}
	if (!Keys[GLFW_KEY_A] && !Keys[GLFW_KEY_D]) {
		player->horDrct = H_NONE;
	}
	if (!Keys[GLFW_KEY_W] && !Keys[GLFW_KEY_S]) {
		player->vertDrct = V_NONE;
	}

	if (Keys[GLFW_KEY_ENTER]) {
		State = GAME_LOAD;
	}
}

/// <summary>
/// Handles the general gameloop by having separate behaviors for different game states and calling
/// updates to object postions, spawning waves, checking collsions, reseting the player's combo, and
/// checking for win and loss state conditions
/// </summary>
/// <param name="dt">Time elapsed between frames</param>
void Game::Update(float dt) {

	if (State == GAME_LOAD) {
		loadTime -= dt;

		if (loadTime <= 0.0f) {
			State = GAME_ACTIVE;
		}
	}

	if (State == GAME_ACTIVE) {
		player->UpdatePosition(dt);
		player->UpdateRotation(mouseX, mouseY);

		if (pState != P_TIME_STOP) {
			for (Enemy* enemy : enemies) {
				enemy->UpdatePosition(dt);
			}
		}

		// Changes the games background between waves
		if (waveCountDown > 0) {
			waveCountDown -= dt;

			if (waveCount != 0) {
				ChangeBackground(dt);
			}
		}
		else {
			switch (waveCount) {
				case 0:
					SpawnWave(dt, wave1);
					break;
				case 1:
					SpawnWave(dt, wave2);
					break;
				case 2:
					SpawnWave(dt, wave3);
					break;
				case 3:
					SpawnWave(dt, wave4);
					break;
				case 4:
					break;
			}
		}

		// Timer for reseting powerup state
		if (pState != P_NONE) {
			if (pState == P_HEALING) {
				powerUpTimer -= (10 * dt);
			}
			else {
				powerUpTimer -= dt;
			}

			if (powerUpTimer <= 0) {
				pState = P_NONE;
				powerUpTimer = POWER_UP_TIME;
			}
		}

		CheckCollisions();

		// Deletes bullets after a time to free up space
		for (int i = 0; i < playerBullets.size(); i++) {
			if (playerBullets[i]->hasExpired) {
				playerBullets.erase(playerBullets.begin() + i);
			}
			else {
				playerBullets[i]->UpdatePosition(dt);
			}
		}

		// Enemy bullets do not lose lifetime while time is frozen
		if (pState != P_TIME_STOP) {
			for (int i = 0; i < enemyBullets.size(); i++) {
				if (enemyBullets[i].hasExpired) {
					enemyBullets.erase(enemyBullets.begin() + i);
				}
				else {
					enemyBullets[i].UpdatePosition(dt);
				}
			}
		}

		if (comboNumber > 0) {
			comboResetTime -= dt;

			if (comboResetTime <= 0) {
				comboNumber = 0;
				scoreMultiplier = 1.0f;
			}
		}

		if (player->health <= 0) {
			State = GAME_LOSS;
		}

		if (waveCount == 4) {
			winTime -= dt;

			if (winTime <= 0) {
				State = GAME_WIN;
			}
		}
	}
}

/// <summary>
/// Gradually shifts from one background to another by incrementing and setting a
/// uniform shader in the background's Shader
/// </summary>
/// <param name="dt">Time elapsed between frames</param>
void Game::ChangeBackground(float dt) {
	if (backgroundShift < 1.0f) {
		backgroundShift += (0.2f * dt);
		backgroundShader.Use();
		backgroundShader.SetFloat("shift", backgroundShift);
	}
}

/// <summary>
/// Handles rendering behavior for the game; only the background and title UI are drawn in the Title game
/// state, objects in the scene and gameplay UI are draw in the Active state
/// </summary>
void Game::Render() {

	glm::vec3 cameraPos = glm::vec3(player->pos.x - 400.0f, player->pos.y - 300.0f, 0.0f);

	view = glm::lookAt(cameraPos, cameraPos + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	if (State == GAME_TITLE) {
		DrawBackground();
	}

	if (State == GAME_ACTIVE || State == GAME_WIN || State == GAME_LOSS) {
		/* When the player has the time stop powerup the background, enemies, and enemy projectiles
		* are all drawn to a framebuffer and saved as a texture, which is then drawn to the screen
		* with the player and player bullets drawn on top
		*/
		if (pState == P_TIME_STOP) {
			glBindFramebuffer(GL_FRAMEBUFFER, fbo);
			glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glEnable(GL_DEPTH_TEST);

			DrawBackground();

			for (Enemy* enemy : enemies) {
				enemy->DrawEnemy(view);
			}

			for (Projectile bullet : enemyBullets) {
				bullet.DrawProjectile(view);
			}

			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);

			// Draws texture from the framebuffer back on the default framebuffer
			grayScaleShader.Use();
			glDisable(GL_DEPTH_TEST);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, grayscaleTex);
			glBindVertexArray(timestopVAO);
			glDrawArrays(GL_TRIANGLES, 0, 6);

			for (Powerup* powerup : powerups) {
				powerup->DrawPowerup(view);
			}

			player->DrawPlayer(view);

			for (Projectile* bullet : playerBullets) {
				bullet->DrawProjectile(view);
			}
		}
		else {
			DrawBackground();

			if (pState == P_HEALING) {
				healingShader.Use();

				glBindVertexArray(healingVAO);
				glDrawArrays(GL_TRIANGLES, 0, 6);
				glBindVertexArray(0);
			}

			for (Powerup* powerup : powerups) {
				powerup->DrawPowerup(view);
			}

			player->DrawPlayer(view);

			for (Enemy* enemy : enemies) {
				enemy->DrawEnemy(view);
			}

			for (Projectile* bullet : playerBullets) {
				bullet->DrawProjectile(view);
			}

			for (Projectile bullet : enemyBullets) {
				bullet.DrawProjectile(view);
			}
		}
	}

	DrawUI();
}

/// <summary>
/// Draws the background by setting the Shader's view matrix, binding active textures, and then
/// drawing it to the screen
/// </summary>
void Game::DrawBackground() {
	backgroundShader.Use();
	backgroundShader.SetMat4("view", view);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, backgroundTex);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, background2);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, background3);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, background4);
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D, healingTex);

	glBindVertexArray(backgroundVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}

/// <summary>
/// Draws text UI on the screen; depending on the game state, different strings
/// of text are drawn, with some using string vectors with updating elements
/// </summary>
void Game::DrawUI() {
	string waveDisplay;
	healthStrings[1] = std::to_string(player->health);
	scoreStrings[1] = std::to_string(score);
	comboStrings[1] = std::to_string(comboNumber) + "x";

	if (waveCount != 4) {
		waveDisplay = "Wave " + std::to_string(waveCount + 1);
	}

	if (State == GAME_TITLE) {
		titleRenderer->DrawText("Geometry Shooter", glm::vec2(104.0f, 350.0f), 1.0f, glm::vec3(0.2f, 1.0f, 0.2f));
		uiRenderer->DrawText("Press ENTER to start", glm::vec2(240.0f, 290.0f), 0.7f, glm::vec3(1.0f, 1.0f, 1.0f));
	}

	if (State == GAME_LOAD) {
		uiRenderer->DrawText("Loading...", glm::vec2(285.0f, 300.0f), 1.0f, glm::vec3(1.0f, 1.0f, 1.0f));
	}

	if (State == GAME_ACTIVE) {
		uiRenderer->DrawTextMultColor(scoreStrings, scoreColors, glm::vec2(15.0f, 550.0f), 0.7f);
		uiRenderer->DrawTextMultColor(healthStrings, healthColors, glm::vec2(625.0f, 550.0f), 0.7f);
		uiRenderer->DrawText(waveDisplay, glm::vec2(350.0f, 550.0f), 0.7f, glm::vec3(1.0f, 1.0f, 1.0f));

		if (comboNumber > 0) {
			uiRenderer->DrawTextMultColor(comboStrings, comboColors, glm::vec2(15.0f, 515.0f), 0.5f);
		}

		if (pState != P_NONE && pState != P_HEALING) {
			string pDisplay = "Powerup Active: " + powerUpDisplay;
			uiRenderer->DrawText(pDisplay, glm::vec2(15.0f, 485.0f), 0.5f, glm::vec3(0.0f, 0.96f, 0.98f));
		}
	}

	if (State == GAME_LOSS) {
		titleRenderer->DrawText("YOU LOSE!", glm::vec2(220.0f, 325.0f), 1.0f, glm::vec3(1.0f, 1.0f, 1.0f));
		uiRenderer->DrawTextMultColor(scoreStrings, scoreColors, glm::vec2(290.0f, 260.0f), 1.0f);
	}

	if (State == GAME_WIN) {
		titleRenderer->DrawText("YOU WIN!", glm::vec2(220.0f, 325.0f), 1.0f, glm::vec3(1.0f, 1.0f, 1.0f));
		uiRenderer->DrawTextMultColor(scoreStrings, scoreColors, glm::vec2(290.0f, 260.0f), 1.0f);
	}
}

/// <summary>
/// Sets the values of the variables used in updating the player's rotation to the mouse's coordinates
/// </summary>
/// <param name="xPos">Mouse's x position in screen space</param>
/// <param name="yPos">Mouse's y position in screen space</param>
void Game::SetMousePos(float xPos, float yPos) {
	mouseX = xPos;
	mouseY = yPos;
}

/// <summary>
/// Spawns bullets at the player's bullet spawn position; spawns different or more projectiles based
/// on powerup states
/// </summary>
void Game::CreateBullet() {

	if (State != GAME_LOSS) {
		player->UpdateBulletSpawnPosition();

		// Add new bullets to end of vector
		if (pState == P_BETTER_BULLETS) {
			playerBullets.push_back(new Projectile(player->bulletSpawn, BETTER_PROJ_SIZE, (player->rotation + 45), glm::vec2(300, 300) * CalculateDirectionVector(player->pos, player->bulletSpawn), glm::vec3(0.99f, 0.76f, 0.0f), 20, false));
		}
		else {
			playerBullets.push_back(new Projectile(player->bulletSpawn, PROJECTILE_SIZE, player->rotation, glm::vec2(300, 300) * CalculateDirectionVector(player->pos, player->bulletSpawn), glm::vec3(1.0f, 0.0f, 0.0f), 10, false));
		}

		if (pState == P_MULTI_SHOT) {
			playerBullets.push_back(new Projectile(player->shiftedLeftSpawn, PROJECTILE_SIZE, (player->rotation + 30), glm::vec2(300, 300) * CalculateDirectionVector(player->pos, player->shiftedLeftSpawn), glm::vec3(1.0f, 0.0f, 0.0f), 10, false));
			playerBullets.push_back(new Projectile(player->shiftedRightSpawn, PROJECTILE_SIZE, (player->rotation - 30), glm::vec2(300, 300) * CalculateDirectionVector(player->pos, player->shiftedRightSpawn), glm::vec3(1.0f, 0.0f, 0.0f), 10, false));
		}
	}
}

/// <summary>
/// Calculates and returns the direction vector for player projectiles
/// </summary>
/// <param name="pos1">Player's position</param>
/// <param name="pos2">Position of bullet spawn point</param>
/// <returns>Direction vector for projectiles</returns>
glm::vec2 Game::CalculateDirectionVector(glm::vec2 pos1, glm::vec2 pos2) {
	glm::vec2 direction = glm::vec2(pos2.x - pos1.x, pos2.y - pos1.y);
	float length = sqrt(pow(direction.x, 2) + pow(direction.y, 2));
	return glm::vec2(direction.x / length, direction.y / length);
}

/// <summary>
/// Checks for and resolves collsions between the objects in the game; index is used as an
/// iterator for identifying which element in object vectors should be erased
/// </summary>
void Game::CheckCollisions() {
	int index = 0;
	bool xCol;
	bool yCol;

	// Used for solving edge case where multiple bullets delete the same frame
	int eraseCount = 0;

	for (Projectile* bullet : playerBullets) {
		float enemyIndex = 0;
		for (Enemy* enemy : enemies) {
			// Check if enemy and bullet overlap on both x- and y-axis
			xCol = bullet->pos.x >= enemy->pos.x - enemy->size.x && bullet->pos.x <= enemy->pos.x + enemy->size.x;
			yCol = bullet->pos.y >= enemy->pos.y - enemy->size.y && bullet->pos.y <= enemy->pos.y + enemy->size.y;

			if (xCol && yCol) {
				enemy->TakeDamage(bullet->damage);
				playerBullets.erase(playerBullets.begin() + (index - eraseCount));
				eraseCount += 1;

				if (enemy->health <= 0) {
					IncreaseScore(enemy->GetPointValue());
					int randInt = rand() % 100 + 1;
					if (randInt <= powerupSpawnChance) {
						SpawnPowerup(enemy->pos);
					}
					enemies.erase(enemies.begin() + enemyIndex);
					CheckWaveEnd();
				}

				// break used to prevent cases of 1 bullet hitting multiple enemies
				break;
			}

			enemyIndex += 1;
		}

		// Player and Enemy bullets destroy each other
		float bulletIndex = 0;
		for (Projectile& eBullet : enemyBullets) {
			//Check for overlaps
			xCol = bullet->pos.x >= eBullet.pos.x - eBullet.size.x && bullet->pos.x <= eBullet.pos.x + eBullet.size.x;
			yCol = bullet->pos.y >= eBullet.pos.y - eBullet.size.y && bullet->pos.y <= eBullet.pos.y + eBullet.size.y;

			if (xCol && yCol) {
				// Wave bullets take three hits to destroy
				if (eBullet.isWave) {
					eBullet.SetWaveHealth(eBullet.waveHealth - 10);

					if (eBullet.waveHealth <= 0) {
						enemyBullets.erase(enemyBullets.begin() + bulletIndex);
					}
					playerBullets.erase(playerBullets.begin() + (index - eraseCount));
					eraseCount += 1;
				}
				else {
					enemyBullets.erase(enemyBullets.begin() + bulletIndex);
					playerBullets.erase(playerBullets.begin() + (index - eraseCount));
					eraseCount += 1;
				}
			}

			bulletIndex += 1;
		}

		index += 1;
	}

	index = 0;

	// Collision between player and enemy projectiles
	for (Projectile bullet : enemyBullets) {
		xCol = bullet.pos.x >= player->pos.x - (player->size.x - 10.0f) && bullet.pos.x <= player->pos.x + (player->size.x - 10.0f);
		yCol = bullet.pos.y >= player->pos.y - player->size.y && bullet.pos.y <= player->pos.y + player->size.y;

		if (xCol && yCol) {
			player->TakeDamage(bullet.pos, bullet.damage);
			enemyBullets.erase(enemyBullets.begin() + index);
		}

		index += 1;
	}

	index = 0;

	// Collision between player and powerups
	for (Powerup* power : powerups) {
		xCol = power->pos.x >= player->pos.x - (player->size.x - 10.0f) && power->pos.x <= player->pos.x + (player->size.x - 10.0f);
		yCol = power->pos.y >= player->pos.y - player->size.y && power->pos.y <= player->pos.y + player->size.y;

		if (xCol && yCol) {
			if (power->pType == BETTER_BULLETS) {
				pState = P_BETTER_BULLETS;
				powerUpDisplay = "Better Bullets";
			}
			else if (power->pType == MULTI_SHOT) {
				pState = P_MULTI_SHOT;
				powerUpDisplay = "Multi Shot";
			}
			else if (power->pType == TIME_STOP) {
				pState = P_TIME_STOP;
				powerUpDisplay = "Time Stop";
			}
			else if (power->pType == HEALING) {
				pState = P_HEALING;
				player->AddHealth(20);
			}
			powerups.erase(powerups.begin() + index);
		}

		index += 1;
	}
}

/// <summary>
/// Updates the player's score by the given value, increases the score multiplier, and resets
/// the combo reset timer
/// </summary>
/// <param name="points">point value to increase score by</param>
void Game::IncreaseScore(int points) {
	score += (points * scoreMultiplier);
	comboNumber += 1;
	comboResetTime = 5.0f;
	scoreMultiplier += 0.1f;
}

/// <summary>
/// Spawns a new Powerup object
/// </summary>
/// <param name="pos">Position to spawn Powerup at</param>
void Game::SpawnPowerup(glm::vec2 pos) {
	powerups.push_back(new Powerup(pos, POWERUP_SIZE, 0.0f, glm::vec3(0.0f, 0.96f, 0.98f)));
}

/// <summary>
/// Handles spawning waves by reading the elements of the given vector and performing actions
/// based on each element value; elements are removed from the vector once read and resovled
/// </summary>
/// <param name="dt">time elapsed between frames</param>
/// <param name="wave">vector to read elements from</param>
void Game::SpawnWave(float dt, vector<int>& wave) {
	int index = 0;

	if (spawnPauseTimer == SPAWN_PAUSE) {
		for (int element : wave) {
			// 0 represents a 5 second pause between enemy spawns
			if (element == 0) {
				spawnPauseTimer -= dt;
				wave.erase(wave.begin(), wave.begin() + (index + 1));
				break;
			}
			else {
				SpawnEnemy(element);
			}

			index += 1;

			if (index == wave.size()) {
				wave.clear();
			}
		}
	}
	else {
		spawnPauseTimer -= dt;

		if (spawnPauseTimer <= 0) {
			spawnPauseTimer = SPAWN_PAUSE;
		}
	}
}

/// <summary>
/// Spawns an enemy of a specified type at a randomised position slightly out of view of the player
/// </summary>
/// <param name="enemyType">Value for switch case determining which enemy type to spawn</param>
void Game::SpawnEnemy(int enemyType) {
	int randomX, randomY;

	// X-value is randomly generated first
	randomX = rand() % ((int)player->pos.x + 500) + ((int)player->pos.x - 500);

	// If the x-value is outside of the screen's width, the y-value is randomly generated
	if (randomX < player->pos.x - 430 || randomX > player->pos.x + 430) {
		randomY = rand() % ((int)player->pos.y + 450) + ((int)player->pos.y - 450);
	}
	// Otherwise, the y-value is set to a fixed value either above or below the screen
	else {
		int side = rand() % 2;
		
		if (side == 0) {
			randomY = player->pos.y + 450;
		}
		else {
			randomY = player->pos.y - 450;
		}
	}
	
	switch (enemyType) {
		case 1:
			enemies.push_back(new Enemy(glm::vec2(randomX, randomY), ENEMY_SIZE, 0.0f, N_ENEMY_COLOR, player));
			break;
		case 2:
			enemies.push_back(new RangedEnemy(glm::vec2(randomX, randomY), ENEMY_SIZE, 0.0f, R_ENEMY_COLOR, player, enemyBullets));
			break;
		case 3:
			enemies.push_back(new WaveEnemy(glm::vec2(randomX, randomY), WAVE_ENEMY_SIZE, 0.0f, W_ENEMY_COLOR, player, enemyBullets));
			break;
	}
}

/// <summary>
/// Checks to see if a wave has been completed, and if so sets a delay for when the next wave starts
/// spawning changes the background's active textures
/// </summary>
void Game::CheckWaveEnd() {
	if (enemies.size() == 0) {
		if (waveCount == 0 && wave1.size() == 0) {
			waveCount += 1;
			waveCountDown = WAVE_START_DELAY;
		}
		if (waveCount == 1 && wave2.size() == 0) {
			waveCount += 1;
			waveCountDown = WAVE_START_DELAY;
			backgroundShader.Use();
			backgroundShader.SetInt("texture", 1);
			backgroundShader.SetInt("texture2", 2);
			backgroundShift = 0.0f;
		}
		if (waveCount == 2 && wave3.size() == 0) {
			waveCount += 1;
			waveCountDown = WAVE_START_DELAY;
			backgroundShader.Use();
			backgroundShader.SetInt("texture", 2);
			backgroundShader.SetInt("texture2", 3);
			backgroundShift = 0.0f;
		}
		if (waveCount == 3 && wave4.size() == 0) {
			waveCount += 1;
		}
	}
}