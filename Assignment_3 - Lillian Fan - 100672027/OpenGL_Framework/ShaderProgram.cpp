#include "ShaderProgram.h"
#include <iostream>
#include <fstream>

bool ShaderProgram::_IsInitDefault = false;
GLuint ShaderProgram::_VertexShaderDefault = 0;
GLuint ShaderProgram::_FragShaderDefault = 0;
GLuint ShaderProgram::_ProgramDefault = 0;

ShaderProgram::ShaderProgram()
{

}

ShaderProgram::ShaderProgram(const std::string & vertFile, const std::string & fragFile)
{
	load(vertFile, fragFile);
}

ShaderProgram::~ShaderProgram()
{
	if (_IsInit)
	{
		unload();
	}
}

bool ShaderProgram::initDefault()
{
	if (!_IsInitDefault)
	{
		ShaderProgram errorShader; 
		bool compileSuccess = errorShader.load("../assets/shaders/error.vert", "../assets/shaders/error.frag");
		if (!compileSuccess)
		{
			std::cout << "Could not compile default Shader!";
			system("PAUSE");
			exit(EXIT_FAILURE);
		}
		ShaderProgram::_VertexShaderDefault = errorShader._VertexShader;
		ShaderProgram::_FragShaderDefault   = errorShader._FragShader;
		ShaderProgram::_ProgramDefault		= errorShader._Program;
		_IsInitDefault = true;
	}
	return _IsInitDefault;
}

void ShaderProgram::setDefault()
{
	_VertexShader = _VertexShaderDefault;
	_FragShader = _FragShaderDefault;
	_Program = _ProgramDefault;
}

bool ShaderProgram::load(const std::string & vertFile, const std::string & fragFile)
{
	vertexFilename = vertFile;
	fragFilename = fragFile;

	// Create shader and program objects
	_VertexShader = glCreateShader(GL_VERTEX_SHADER);
	_FragShader = glCreateShader(GL_FRAGMENT_SHADER);
	_Program = glCreateProgram();


	// Load our source code for shaders
	std::string source = readFile(vertFile);
	const GLchar *temp = static_cast<const GLchar *>(source.c_str());
	glShaderSource(_VertexShader, 1, &temp, NULL);

	source = readFile(fragFile);
	temp = static_cast<const GLchar *>(source.c_str());
	glShaderSource(_FragShader, 1, &temp, NULL);

	// Compile vertex and frag shaders
	if (!compileShader(_VertexShader))
	{
		std::cout << "Vertex Shader failed to compile. \n";
		outputShaderLog(_VertexShader);
		unload();
		setDefault();
		return false;
	}

	if (!compileShader(_FragShader))
	{
		std::cout << "Fragment Shader failed to compile. \n";
		outputShaderLog(_FragShader);
		unload();
		setDefault();
		return false;
	}

	glAttachShader(_Program, _VertexShader);
	glAttachShader(_Program, _FragShader);

	if (!linkProgram())
	{
		std::cout << "Shader Program failed to link. \n";
		outputProgramLog();
		unload();
		setDefault();
		return false;
	}

	_IsInit = true;
	return true;
}

bool ShaderProgram::reload()
{
	return load(vertexFilename, fragFilename);
}

bool ShaderProgram::isLoaded() const
{
	return _IsInit;
}

void ShaderProgram::unload()
{
	if (_VertexShader != 0 && _VertexShader != _VertexShaderDefault) //If the Vertex shader exists, remove it.
	{
		glDetachShader(_Program, _VertexShader);
		glDeleteShader(_VertexShader);
		_VertexShader = 0;
	}
	if (_FragShader != 0 && _FragShader != _FragShaderDefault)
	{
		glDetachShader(_Program, _FragShader);
		glDeleteShader(_FragShader);
		_FragShader = 0;
	}
	if (_Program != 0 && _Program != _ProgramDefault) //If the Fragment shader exists, remove it.
	{
		glDeleteProgram(_Program);
		_Program = 0;
	}
	_IsInit = false;
}

bool ShaderProgram::linkProgram()
{
	glLinkProgram(_Program);
	GLint success;
	glGetProgramiv(_Program, GL_LINK_STATUS, &success);
	return success == GL_TRUE;
}

void ShaderProgram::bind() const
{
	glUseProgram(_Program);
}

void ShaderProgram::unbind()
{
	glUseProgram(0);
}

GLint ShaderProgram::getUniformLocation(const std::string & uniformName) const
{
	GLint uniformLoc = glGetUniformLocation(_Program, uniformName.c_str());	
#if _DEBUG
	//if (uniformLoc == -1)
	//	std::cout << "WARNING: Uniform " << uniformName << " not found!" << std::endl;
#endif
	return uniformLoc;
}

void ShaderProgram::sendUniform(const std::string & name, const float scalar) const
{
	GLint location = getUniformLocation(name);
	glUniform1f(location, scalar);
}

void ShaderProgram::sendUniform(const std::string & name, const vec3 & vector) const
{
	GLint location = getUniformLocation(name);
	glUniform3fv(location, 1, &vector.x);
}

void ShaderProgram::sendUniform(const std::string & name, const vec4 & vector) const
{
	GLint location = getUniformLocation(name);
	glUniform4fv(location, 1, &vector.x);
}

void ShaderProgram::sendUniform(const std::string & name, const mat4 & matrix, bool transpose) const
{
	GLint location = getUniformLocation(name);
	glUniformMatrix4fv(location, 1, transpose, matrix.data);
}

std::string ShaderProgram::readFile(const std::string & filename) const
{
	std::ifstream inStream(filename);
	if (!inStream.good())
	{
		std::cout << "Shader File not found:\"" << filename << "\"\n";
		return std::string();
	}
	std::string data(std::istreambuf_iterator<char>(inStream), (std::istreambuf_iterator<char>()));
	return data;
}

bool ShaderProgram::compileShader(GLuint shader) const
{
	glCompileShader(shader);
	GLint success;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	return success == GL_TRUE;
}

void ShaderProgram::outputShaderLog(GLuint shader) const
{
	std::vector<char> infoLog;
	infoLog.resize(512);

	GLint infoLen;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
	glGetShaderInfoLog(shader, sizeof(char) * 512, &infoLen, &infoLog[0]); //Size of char array in bits for portability, rather than characters.
	std::cout << std::string(infoLog.begin(), infoLog.end()) << "\n";
}

void ShaderProgram::outputProgramLog() const
{
	std::vector<char> infoLog;
	infoLog.resize(512);

	GLint infoLen;
	glGetProgramiv(_Program, GL_INFO_LOG_LENGTH, &infoLen);
	glGetProgramInfoLog(_Program, sizeof(char) * 512, &infoLen, &infoLog[0]); // Size of char array in bits for portability, rather than characters.
	std::cout << std::string(infoLog.begin(), infoLog.end()) << "\n";
}
