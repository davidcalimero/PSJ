#pragma once

#include "Utils.h"

#include <iostream>
#include <glm.hpp>

#include "GL/glew.h"
#include "GL/freeglut.h"

class Texture {
	private:
		GLuint _textureID[2];

	public:
		Texture(char * textureName1, char * textureName2);
		~Texture();
		void bind();
		void unbind();
};