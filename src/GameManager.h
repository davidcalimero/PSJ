#pragma once

#include "Camera.h"
#include "Object.h"
#include "Light.h"
#include "SkyBox.h"

#include <iostream>
#include <map>



class GameManager {

	private:
		enum Objects { QUAD, CUBE, SPHERE, TORUS, TEAPOT };
		Objects _state;
		Light * _light;
		SkyBox * _skybox;
		GLuint _programas[4];
		int _programa;
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