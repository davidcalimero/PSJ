#pragma once

#include "GL/glew.h"
#include "GL/freeglut.h"

#include "GameManager.h"

#include <glm.hpp>
#include <gtc\quaternion.hpp>
#include <iostream>



class Camera {

	private:
		glm::quat _q;
		glm::vec3 _eye;
		glm::vec3 _center;
		glm::vec3 _up;
		GLuint _vboUBId;
		bool _type;
		Camera();
		glm::mat4 _view;
		int _distance;

	public:
		~Camera();
		static Camera * getInstance();
		void put();
		void rotate(float angleX, float angleZ);
		void update();
		glm::mat4 getView();
};