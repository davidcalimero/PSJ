#pragma once

#include "Entity.h"

#include <iostream>



class SkyBox : public Entity {

	public:
		SkyBox(std::string id);
		void update();
};