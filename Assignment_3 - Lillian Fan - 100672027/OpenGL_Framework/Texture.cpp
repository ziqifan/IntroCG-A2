#include "Texture.h"

#pragma comment(lib, "SOIL_ext.lib")
#include "SOIL/SOIL.h"

#include <assert.h>

Texture::Texture(const std::string & file)
{
	load(file);
}

Texture::~Texture()
{
	unload();
}

bool Texture::load(const std::string & file)
{
	this->_Filename = "../assets/textures/" + file;

	unsigned char* textureData;
	textureData = SOIL_load_image(this->_Filename.c_str(),
		&this->sizeX, &this->sizeY, &this->channels, SOIL_LOAD_RGBA);

	if (this->sizeX == 0 || this->sizeY == 0 || this->channels == 0)
	{
		printf("Texture broke: %s", _Filename.c_str());
		return false;
	}

	// if it's a 2D texture
	this->_Target = GL_TEXTURE_2D;
	this->_InternalFoemat = GL_RGBA8;

	glGenTextures(1, &this-> _TextureHandle);
	this->bind();
	glTextureStorage2D(this->_TextureHandle, 1, // we a re allocate space on gpu for a single layer
		this->_InternalFoemat, this->sizeX, this->sizeY);

	glTextureSubImage2D(this->_TextureHandle, 0, // we editing the first and only layer in memory
		0, 0, //no offset
		this->sizeX, this->sizeY, // the demnsions we are sending
		GL_RGBA, GL_UNSIGNED_BYTE,//date format
		textureData); //pointer to the texture data

	glTextureParameteri(this->_TextureHandle, GL_TEXTURE_MIN_FILTER, this->_FilterMin);
	glTextureParameteri(this->_TextureHandle, GL_TEXTURE_MAG_FILTER, this->_FilterMag);
	glTextureParameteri(this->_TextureHandle, GL_TEXTURE_WRAP_S, this->_WrapX);
	glTextureParameteri(this->_TextureHandle, GL_TEXTURE_WRAP_T, this->_WrapY);

	this->unbind();

	SOIL_free_image_data(textureData);
	return true;
}

bool Texture::unload()
{
	if (this->_TextureHandle)
	{
		glDeleteTextures(1, &this->_TextureHandle);
		this->_TextureHandle = 0;
		return true;
	}
	return false;
}

void Texture::setAcriveSlot(int textureSlot) const
{
	assert(textureSlot >= 0 && textureSlot <= 32);
	glActiveTexture(GL_TEXTURE0 + textureSlot);
}

void Texture::bind() const
{
	glBindTexture(this->_Target, this->_TextureHandle);
}

void Texture::bind(int textureSlot) const
{
	setAcriveSlot(textureSlot);
	bind();
}

void Texture::unbind() const
{
	glBindTexture(this->_Target, GL_NONE);
}

void Texture::unbind(int textureSlot) const
{
	setAcriveSlot(textureSlot);
	unbind();
}
