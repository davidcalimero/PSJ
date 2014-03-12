#pragma once

#include "Utils.h"
#include "Texture.h"

#include <iostream>
#include <glm.hpp>

#include "GL/glew.h"
#include "GL/freeglut.h"

class TextureCube : public Texture {
	public:
		TextureCube(char * right, char * left, char * top, char * bottom, char * back, char * front);
		~TextureCube();
		void bind();
		void unbind();
};