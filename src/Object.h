#pragma once

#include "Entity.h"
#include "Input.h"

#include <iostream>



class Object : public Entity {

	private:
		enum rotations {RX, RY, RZ, NR};
		rotations _state;

	public:
		Object(std::string id);
		void reset();
		void update();
};