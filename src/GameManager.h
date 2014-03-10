#pragma once

#include "Camera.h"
#include "Object.h"
#include "Light.h"

#include <iostream>
#include <map>



class GameManager {

	private:
		enum Objects {CUBE, TEAPOT, TORUS};
		Objects _state;
		Light * _light;
		typedef std::map<const std::string, Entity *>::iterator entityIterator;
		std::map<const std::string, Entity *> _entities;
		GameManager();
		std::string enumToId(Objects e);


	public:
		static GameManager * getInstance();
		void init();
		void add(Entity * entity);
		Entity * getEntityById(std::string id);
		void destroyBufferObjects();
		void draw();
		void update();
		void postProcessing();
};