#include "Input.h"



Input::Input(){
	_wheelDirection = 0;
	_keyPressedStates = new bool[SIZE];
	_keyReleasedStates = new bool[SIZE];
	_specialPressedStates = new bool[SIZE];
	_specialReleasedStates = new bool[SIZE];
	_mousePressedStates = new bool[3];
	_mouseReleasedStates = new bool[3];

    for (int i = 0; i < SIZE; i++) {
        _keyPressedStates[i] = false;
		_specialPressedStates[i] = false;
		_keyReleasedStates[i] = false;
		_specialReleasedStates[i] = false;
		if(i < 3){
			_mousePressedStates[i] = false;
			_mouseReleasedStates[i] = false;
		}
    }
}


Input * Input::getInstance(){
	static Input instance;
	return &instance;
}


void Input::keyPressed(unsigned char key){
	if (key > 96 && key < 123) key -= 32;
	_keyPressedStates[key] = true;
}


void Input::keyUp(unsigned char key){
	if (key > 96 && key < 123) key -= 32;
	_keyPressedStates[key] = false;
	_keyReleasedStates[key] = true;
}


void Input::specialPressed(int key){
	_specialPressedStates[key] = true;
}


void Input::specialUp(int key){
	_specialPressedStates[key] = false;
	_specialReleasedStates[key] = true;
}


void Input::reset(){
    for (int i = 0; i < SIZE; i++) {
		_keyReleasedStates[i] = false;
		_specialReleasedStates[i] = false;
		if(i < 3) _mouseReleasedStates[i] = false;
    }
	_mouseMotion.x = 0;
	_mouseMotion.y = 0;
	_wheelDirection = 0;
}


bool Input::keyWasPressed(unsigned char key){
	return _keyPressedStates[key];
}


bool Input::keyWasReleased(unsigned char key){
	return _keyReleasedStates[key];
}


bool Input::specialWasPressed(int key){
	return _specialPressedStates[key];
}


bool Input::specialWasReleased(int key){
	return _specialReleasedStates[key];
}


bool Input::mouseWasPressed(int key){
	return _mousePressedStates[key];
}


bool Input::mouseWasReleased(int key){
	return _mouseReleasedStates[key];
}


void Input::mouse(int button, int state) {
	if(state == GLUT_DOWN){
		_mousePressedStates[button] = true;
	}
	else if(state == GLUT_UP){
		_mouseReleasedStates[button] = true;
		_mousePressedStates[button] = false;
	}
}


void Input::mouseClickMotion(int x, int y) {
	_mouseMotion.x = x - _lastMousePositionX;
	_mouseMotion.y = y - _lastMousePositionY;
	_lastMousePositionX = x;
	_lastMousePositionY = y;
	
}


void Input::mousePassiveMotion(int x, int y){
	_lastMousePositionX = x;
	_lastMousePositionY = y;
}


void Input::mouseWheel(int direction){
	_wheelDirection = -direction;
}


glm::vec2 Input::getMousePostion(){
	glm::vec2 pos;
	pos.x = _lastMousePositionX;
	pos.y = _lastMousePositionY;
	return pos;
}


glm::vec2 Input::getMouseMotion(){
	return _mouseMotion;
}


int Input::getWheelDirection(){
	return _wheelDirection;
}