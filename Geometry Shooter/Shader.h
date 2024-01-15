//*****************************************************************************
// Shader.h
// 
// Brief Description: Header file for the shader class
// 
// This code is a slightly modified version of:
// 
// "Shaders.h" from Learn OpenGL's Shaders chapter (https://learnopengl.com/code_viewer_gh.php?code=includes/learnopengl/shader_s.h)
// Created by Joey de Vries, licensed under CC BY 4.0
//*****************************************************************************
#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;

class Shader {
public:
	unsigned int ID;

	Shader();

	Shader(const char* vertexPath, const char* fragmentPath);

	void Use();

	// Functions below are for setting uniforms various types
	void SetBool(const string& name, bool value) const;

	void SetInt(const string& name, int value) const;

	void SetFloat(const string& name, float value) const;

	void SetVec2(const std::string& name, const glm::vec2& value) const;

	void SetVec2(const std::string& name, float x, float y) const;

	void SetVec3(const std::string& name, const glm::vec3& value) const;

	void SetVec3(const std::string& name, float x, float y, float z) const;

	void SetVec4(const std::string& name, const glm::vec4& value) const;

	void SetVec4(const std::string& name, float x, float y, float z, float w) const;

	void SetMat2(const std::string& name, const glm::mat2& mat) const;

	void SetMat3(const std::string& name, const glm::mat3& mat) const;

	void SetMat4(const std::string& name, const glm::mat4& mat) const;
};

