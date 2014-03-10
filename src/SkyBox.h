#pragma once

#include "Entity.h"

#include <iostream>



class SkyBox : public Entity {

	private:
		GLuint _cubeID;

	public:
		SkyBox(std::string id);
		void update();
		void draw();
};