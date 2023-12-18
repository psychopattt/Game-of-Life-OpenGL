#include "Texture.h"
#include "glad/glad.h"

Texture::Texture(unsigned int width, unsigned int height) :
	Texture(width, height, GL_RGBA32F, GL_CLAMP_TO_EDGE, GL_NEAREST) { }

Texture::Texture(unsigned int width, unsigned int height, int format) :
	Texture(width, height, format, GL_CLAMP_TO_EDGE, GL_NEAREST) { }

Texture::Texture(unsigned int width, unsigned int height, int format, int wrap, int filtering) :
	Texture(width, height, format, wrap, wrap, filtering) { }

Texture::Texture(unsigned int width, unsigned int height, int format, int wrapX, int wrapY, int filtering)
{
	this->width = width;
	this->height = height;

	glGenTextures(1, &id);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, id);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapX);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapY);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filtering);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filtering);

	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, GL_RGBA, GL_FLOAT, 0);
	glBindImageTexture(0, id, 0, GL_FALSE, 0, GL_READ_WRITE, format);
}

unsigned int Texture::GetId() const
{
	return id;
}

void Texture::Destroy() const
{
	glDeleteTextures(1, &id);
}
