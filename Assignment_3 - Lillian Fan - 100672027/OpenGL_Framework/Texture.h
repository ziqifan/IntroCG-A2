#pragma once
#include "GL/glew.h"
#include <string>

class Texture
{
public:
	Texture() = default;
	Texture(const std::string &file);
	~Texture();

	bool load(const std::string &file);//use to load 1D or 2D textures
	bool load3D(const std::string &file);//Make it :D
	bool unload();


	void setAcriveSlot(int textureSlot) const;
	void bind() const;
	void bind(int textureSlot) const;
	void unbind() const;
	void unbind(int textureSlot) const;

private:
	GLuint _TextureHandle;

	std::string _Filename;

	GLenum _Target;
	GLenum _InternalFoemat;

	// there's a couple
	GLenum _FilterMag = GL_LINEAR;// GL_NEAREST;
	GLenum _FilterMin = GL_LINEAR;// GL_NEAREST;
	GLenum _WrapX = GL_REPEAT;// GL_MIRRORED_REPEAT;
	GLenum _WrapY = GL_REPEAT;//GL_MIRRORED_REPEAT;


	int sizeX;
	int sizeY;
	int channels;
};