/* Copyright © 2013 Fabian Schuiki */
#pragma once
#include <SFML/OpenGL.hpp>

namespace gl
{
	class Texture
	{
	public:
		GLuint id;
		GLenum wrap, magFilter, minFilter;

		Texture();
		~Texture();

		void updateParameters();
	};
}