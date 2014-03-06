#pragma once

#include <glm.hpp>
#include "GL/freeglut.h"

#define SIZE 256

class Input {

	private:
		int _lastMousePositionX;
		int _lastMousePositionY;
		glm::vec2 _mouseMotion;
		bool * _keyPressedStates; 
		bool * _specialPressedStates;
		bool * _keyReleasedStates;
		bool * _specialReleasedStates;
		bool * _mousePressedStates;
		bool * _mouseReleasedStates;
		int _wheelDirection;
		Input();

	public:
		static Input * getInstance();
		void keyPressed(unsigned char key);
		void keyUp(unsigned char key);
		void specialPressed(int key);
		void specialUp(int key);
		void mouse(int button, int state);
		void mouseClickMotion(int x, int y);
		void mousePassiveMotion(int x, int y);
		void mouseWheel(int direction);
		void reset();

		bool keyWasPressed(unsigned char key);
		bool keyWasReleased(unsigned char key);
		bool specialWasPressed(int key);
		bool specialWasReleased(int key);
		bool mouseWasPressed(int key);
		bool mouseWasReleased(int key);
		glm::vec2 getMousePostion();
		glm::vec2 getMouseMotion();
		int getWheelDirection();
};