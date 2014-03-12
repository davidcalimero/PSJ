#pragma once

#include "Utils.h"

#include <iostream>
#include <glm.hpp>

#include "GL/glew.h"
#include "GL/freeglut.h"

class SkyBox {
	private:
		GLuint _cubeID;

	public:
		SkyBox();
		~SkyBox();
		void bind();
		void unbind();
};