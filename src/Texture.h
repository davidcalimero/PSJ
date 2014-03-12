#pragma once

#include "Utils.h"

#include <iostream>
#include <glm.hpp>

#include "GL/glew.h"
#include "GL/freeglut.h"

class Texture {
	protected:
		Texture(int slot);
		GLuint _textureID;
		int _slot;

	public:
		Texture(char * textureName, int slot);
		~Texture();
		virtual void bind();
		virtual void unbind();
};