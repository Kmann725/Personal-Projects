//*****************************************************************************
// TextRenderer.h
// 
// Author: Kyle Manning
// 
// Brief Description: Header for TextRenderer objects
//*****************************************************************************
#pragma once

#include <iostream>
#include <string>
#include <map>
#include <vector>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <ft2build.h>
#include FT_FREETYPE_H

#include "Shader.h"

struct Character {
	unsigned int TextureId;
	glm::ivec2 Size;
	glm::ivec2 Bearing;
	unsigned int Advance;
};

class TextRenderer {
public:
	int fontSize;
	unsigned int textVAO, textVBO;
	string fontFile;
	std::map<GLchar, Character> Characters;
	Shader textShader;

	TextRenderer(string file, int size);

	// Draws a string of text to the screen at a specified position, scale, and color
	void DrawText(std::string text, glm::vec2 pos, float scale, glm::vec3 color);

	// Draws text to the screen with different sections being different colors
	void DrawTextMultColor(vector<string> strings, vector<glm::vec3> colors, glm::vec2 pos, float scale);
};

