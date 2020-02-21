#pragma once
#include <string>
#include "MiniMath/Core.h"
#include "GL\glew.h"
#include <vector>

class ShaderProgram
{
public: 
	ShaderProgram();
	ShaderProgram(const std::string &vertFile, const std::string &fragFile);
	~ShaderProgram();

	static bool initDefault();
	void setDefault();

	bool load(const std::string &vertFile, const std::string &fragFile);
	bool reload();
	bool isLoaded() const;
	void unload();
	bool linkProgram();

	void bind() const;
	static void unbind();

	GLint getUniformLocation(const std::string &uniformName) const;

	void sendUniform(const std::string &name, const float scalar) const;
	void sendUniform(const std::string &name, const vec3 &vector) const;
	void sendUniform(const std::string &name, const vec4 &vector) const;
	void sendUniform(const std::string &name, const mat4 &matrix, bool transpose = false) const;

private: 
	bool _IsInit = false;
	GLuint _VertexShader = 0;
	GLuint _FragShader = 0;
	GLuint _Program = 0;

	std::string vertexFilename;
	std::string fragFilename;

	static bool _IsInitDefault;
	static GLuint _VertexShaderDefault;
	static GLuint _FragShaderDefault;
	static GLuint _ProgramDefault;

	std::string readFile(const std::string &filename) const;
	bool compileShader(GLuint shader) const;
	void outputShaderLog(GLuint shader) const;
	void outputProgramLog() const;
};