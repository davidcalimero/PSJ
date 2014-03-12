#pragma once

#include "Entity.h"
#include "Input.h"

#include <iostream>



class SkyBox : public Entity {

	private:

	public:
		SkyBox(std::string id);
		void update();
};