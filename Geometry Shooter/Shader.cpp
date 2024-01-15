//*****************************************************************************
// Shader.cpp
// 
// Brief Description: Contains methods for building shader programs using
//					  file paths for vertex and fragment shaders, and methods
//					  for setting various types of uniforms
// 
// This code is a slightly modified version of:
// 
// "Shaders.h" from Learn OpenGL's Shaders chapter (https://learnopengl.com/code_viewer_gh.php?code=includes/learnopengl/shader_s.h)
// Created by Joey de Vries, licensed under CC BY 4.0
//*****************************************************************************
#include "Shader.h"

Shader::Shader() { }

/// <summary>
/// Opens files using the paths provided as arguments and builds them
/// into shader programs
/// </summary>
/// <param name="vertexPath">filepath of vertex shader</param>
/// <param name="fragmentPath">filepath of fragment shader</param>
Shader::Shader(const char* vertexPath, const char* fragmentPath) {
	string vertexCode;
	string fragmentCode;
	ifstream vShaderFile;
	ifstream fShaderFile;

	vShaderFile.exceptions(ifstream::failbit | ifstream::badbit);
	fShaderFile.exceptions(ifstream::failbit | ifstream::badbit);

	try {
		vShaderFile.open(vertexPath);
		fShaderFile.open(fragmentPath);
		stringstream vShaderStream, fShaderStream;

		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();

		vShaderFile.close();
		fShaderFile.close();

		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
	}
	catch (ifstream::failure e) {
		cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ" << endl;
	}
	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();

	unsigned int vertex, fragment;
	int success;
	char infoLog[512];

	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);

	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertex, 512, NULL, infoLog);
		cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << endl;
	};

	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);

	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragment, 512, NULL, infoLog);
		cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << endl;
	};

	ID = glCreateProgram();
	glAttachShader(ID, vertex);
	glAttachShader(ID, fragment);
	glLinkProgram(ID);

	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

void Shader::Use() {
	glUseProgram(ID);
}

/// <summary>
/// Sets the value of boolean uniforms
/// </summary>
/// <param name="name">Name of the uniform</param>
/// <param name="value">Value to set uniform as</param>
void Shader::SetBool(const string& name, bool value) const {
	glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

/// <summary>
/// Sets the value of integer uniforms
/// </summary>
/// <param name="name">Name of the uniform</param>
/// <param name="value">Value to set uniform as</param>
void Shader::SetInt(const string& name, int value) const {
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

/// <summary>
/// Sets the value of float uniforms
/// </summary>
/// <param name="name">Name of the uniform</param>
/// <param name="value">Value to set uniform as</param>
void Shader::SetFloat(const string& name, float value) const {
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

/// <summary>
/// Sets the value of vector2 uniforms (using a vec2 for the value)
/// </summary>
/// <param name="name">Name of the uniform</param>
/// <param name="value">Value to set uniform as</param>
void Shader::SetVec2(const std::string& name, const glm::vec2& value) const {
	glUniform2fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}

/// <summary>
/// Sets the value of vector2 uniforms (using two floats for the values)
/// </summary>
/// <param name="name">Name of the uniform</param>
/// <param name="x">Value to set the uniform's x as</param>
/// <param name="y">Value to set the uniform's y as</param>
void Shader::SetVec2(const std::string& name, float x, float y) const {
	glUniform2f(glGetUniformLocation(ID, name.c_str()), x, y);
}

/// <summary>
/// Sets the value of vector3 uniforms (using a vec3 for the value)
/// </summary>
/// <param name="name">Name of the uniform</param>
/// <param name="value">Value to set uniform as</param>
void Shader::SetVec3(const std::string& name, const glm::vec3& value) const {
	glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}

/// <summary>
/// Sets the value of vector3 uniforms (using three floats for the values)
/// </summary>
/// <param name="name">Name of the uniform</param>
/// <param name="x">Value to set the uniform's x as</param>
/// <param name="y">Value to set the uniform's y as</param>
/// <param name="z">Value to set the uniform's z as</param>
void Shader::SetVec3(const std::string& name, float x, float y, float z) const {
	glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
}

/// <summary>
/// Sets the value of vector4 uniforms (using a vec4 for the value)
/// </summary>
/// <param name="name">Name of the uniform</param>
/// <param name="value">Value to set uniform as</param>
void Shader::SetVec4(const std::string& name, const glm::vec4& value) const {
	glUniform4fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}

/// <summary>
/// Sets the value of vector4 uniforms (using four floats for the values)
/// </summary>
/// <param name="name">Name of the uniform</param>
/// <param name="x">Value to set the uniform's x to</param>
/// <param name="y">Value to set the uniform's y to</param>
/// <param name="z">Value to set the uniform's z to</param>
/// <param name="w">Value to set the unifrom's w to</param>
void Shader::SetVec4(const std::string& name, float x, float y, float z, float w) const {
	glUniform4f(glGetUniformLocation(ID, name.c_str()), x, y, z, w);
}

/// <summary>
/// Sets the value of 2x2 matrix uniforms
/// </summary>
/// <param name="name">Name of the uniform</param>
/// <param name="mat">Matrix that uniform is set to</param>
void Shader::SetMat2(const std::string& name, const glm::mat2& mat) const {
	glUniformMatrix2fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

/// <summary>
/// Sets the value of 3x3 matrix uniforms
/// </summary>
/// <param name="name">Name of the uniform</param>
/// <param name="mat">Matrix that uniform is set to</param>
void Shader::SetMat3(const std::string& name, const glm::mat3& mat) const {
	glUniformMatrix3fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}

/// <summary>
/// Sets the value of 4x4 matrix uniforms
/// </summary>
/// <param name="name">Name of the uniform</param>
/// <param name="mat">Matrix that uniform is set to</param>
void Shader::SetMat4(const std::string& name, const glm::mat4& mat) const {
	glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &mat[0][0]);
}