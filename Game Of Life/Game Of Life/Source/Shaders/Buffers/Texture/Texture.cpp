#include "Texture.h"

#include "glad/gl.h"
#include "Settings/LogString/LogString.h"
#include "Settings/Settings.h"

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
	Activate();

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapX);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapY);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filtering);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filtering);

	glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, GL_RGBA, GL_FLOAT, 0);
	glBindImageTexture(id, id, 0, GL_FALSE, 0, GL_READ_WRITE, format);

	LogGenerationFailure(width, height, format, wrapX, wrapY, filtering);
}

unsigned int Texture::GetId() const
{
	return id;
}

void Texture::Activate() const
{
	glActiveTexture(GL_TEXTURE0 + id);
	glBindTexture(GL_TEXTURE_2D, id);
}

void Texture::LogGenerationFailure(int width, int height, int format,
	int wrapX, int wrapY, int filtering) const
{
	if (int errorCode = glGetError() != GL_NO_ERROR)
	{
		Settings::Log << "Texture Error - Failed to generate texture\nId: " <<
			id << ", size: [" << width << ", " << height << "], format: " <<
			format << ", wrap: [" << wrapX << ", " << wrapY << "], filtering: " <<
			filtering << ", error code: " << errorCode << "\n";

		int maxTextureSize;
		glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxTextureSize);

		if (width > maxTextureSize || height > maxTextureSize)
		{
			Settings::Log << "Max texture size supported by GPU: [" <<
				maxTextureSize << ", " << maxTextureSize << "]\n";
		}

		Settings::Log << "\n";
	}
}

Texture::~Texture()
{
	glDeleteTextures(1, &id);
}
