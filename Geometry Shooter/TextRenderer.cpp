//*****************************************************************************
// TextRenderer.cpp
// 
// Author: Kyle Manning
// 
// Brief Description: Creates a useable set of textures for each character in
//					  a font file, and can draw strings on the screen using
//					  these textures
//*****************************************************************************
#include "TextRenderer.h"

/// <summary>
/// Creates a sets of Character structs with textures set to each character in the
/// given font file at the specified font size
/// </summary>
/// <param name="file">Font file to generate character textures of</param>
/// <param name="size">Font size</param>
TextRenderer::TextRenderer(string file, int size) : fontFile(file), fontSize(size) {
	textShader = Shader("text.vs", "text.fs");

	FT_Library ft;
	FT_Init_FreeType(&ft);
	FT_Face face;
	FT_New_Face(ft, fontFile.c_str(), 0, &face);
	FT_Set_Pixel_Sizes(face, 0, size);
	
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	for (unsigned char c = 0; c < 128; c++) {
		FT_Load_Char(face, c, FT_LOAD_RENDER);

		unsigned int texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, face->glyph->bitmap.width, face->glyph->bitmap.rows, 0, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		Character character = { texture, glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows), glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top), (unsigned int)face->glyph->advance.x };
		Characters.insert(std::pair<char, Character>(c, character));
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	FT_Done_Face(face);
	FT_Done_FreeType(ft);

	glGenVertexArrays(1, &textVAO);
	glGenBuffers(1, &textVBO);
	glBindVertexArray(textVAO);
	glBindBuffer(GL_ARRAY_BUFFER, textVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	textShader.Use();
	glm::mat4 projection = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f);
	textShader.SetMat4("projection", projection);
}

/// <summary>
/// Draws a given string of text by iteratirng through each character, finding the matching
/// Character struct, and then drawing that character to the screen
/// </summary>
/// <param name="text">String of text to draw</param>
/// <param name="pos">Position of the text in screen space</param>
/// <param name="scale">Scalar value used when drawing</param>
/// <param name="color">Color of the text</param>
void TextRenderer::DrawText(std::string text, glm::vec2 pos, float scale, glm::vec3 color) {
	textShader.Use();
	textShader.SetVec3("textColor", color);
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(textVAO);

	std::string::const_iterator c;
	for (c = text.begin(); c != text.end(); c++) {
		Character ch = Characters[*c];

		float xpos = pos.x + ch.Bearing.x * scale;
		float ypos = pos.y - (ch.Size.y - ch.Bearing.y) * scale;

		float w = ch.Size.x * scale;
		float h = ch.Size.y * scale;

		float vertices[6][4] = {
			{ xpos, ypos + h, 0.0f, 0.0f },
			{ xpos, ypos, 0.0f, 1.0f },
			{ xpos + w, ypos, 1.0f, 1.0f },

			{ xpos, ypos + h, 0.0f, 0.0f },
			{ xpos + w, ypos, 1.0f, 1.0f },
			{ xpos + w, ypos + h, 1.0f, 0.0f }
		};

		glBindTexture(GL_TEXTURE_2D, ch.TextureId);
		glBindBuffer(GL_ARRAY_BUFFER, textVBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glDrawArrays(GL_TRIANGLES, 0, 6);

		pos.x += (ch.Advance >> 6) * scale;
	}

	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

// NOTE: Number of elemetns in strings and colors must match to work
/// <summary>
/// Draws a set of strings to the screen using the color at the corresponding index in
/// the color vector
/// </summary>
/// <param name="strings">Set of strings to be drawn</param>
/// <param name="colors">Set of colors that each string should be</param>
/// <param name="pos">Position of the text in screen space</param>
/// <param name="scale">Scalar value for drawing the text</param>
void TextRenderer::DrawTextMultColor(vector<string> strings, vector<glm::vec3> colors, glm::vec2 pos, float scale) {
	textShader.Use();
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(textVAO);
	
	for (int i = 0; i < strings.size(); i++) {
		std::string::const_iterator c;
		textShader.SetVec3("textColor", colors[i]);
		for (c = strings[i].begin(); c != strings[i].end(); c++) {
			Character ch = Characters[*c];

			float xpos = pos.x + ch.Bearing.x * scale;
			float ypos = pos.y - (ch.Size.y - ch.Bearing.y) * scale;

			float w = ch.Size.x * scale;
			float h = ch.Size.y * scale;

			float vertices[6][4] = {
				{ xpos, ypos + h, 0.0f, 0.0f },
				{ xpos, ypos, 0.0f, 1.0f },
				{ xpos + w, ypos, 1.0f, 1.0f },

				{ xpos, ypos + h, 0.0f, 0.0f },
				{ xpos + w, ypos, 1.0f, 1.0f },
				{ xpos + w, ypos + h, 1.0f, 0.0f }
			};

			glBindTexture(GL_TEXTURE_2D, ch.TextureId);
			glBindBuffer(GL_ARRAY_BUFFER, textVBO);
			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
			glBindBuffer(GL_ARRAY_BUFFER, 0);

			glDrawArrays(GL_TRIANGLES, 0, 6);

			pos.x += (ch.Advance >> 6) * scale;
		}
	}

	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}