/* Copyright © 2013 Fabian Schuiki */
#include "Texture.h"
using gl::Texture;

Texture::Texture()
{
	// Setup default texture parameters.
	wrap = GL_CLAMP_TO_EDGE;
	magFilter = GL_LINEAR;
	minFilter = GL_LINEAR;

	glGenTextures(1, &id);
	updateParameters();
}

Texture::~Texture()
{
	if (id) {
		glDeleteTextures(1, &id);
		id = 0;
	}
}

void Texture::updateParameters()
{
	glBindTexture(GL_TEXTURE_2D, id);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
}