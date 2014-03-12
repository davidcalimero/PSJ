#pragma once

#include "Utils.h"

#include <iostream>
#include <glm.hpp>

#include "GL/glew.h"
#include "GL/freeglut.h"

class TextureCube {
	private:
		GLuint _cubeID;

	public:
		TextureCube();
		~TextureCube();
		void bind();
		void unbind();
};