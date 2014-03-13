#include "Camera.h"



Camera::Camera(){
	_type = 0;
	_distance = 4;
	_eye = glm::vec3(0.0,-1.0,0.0);
	_center = glm::vec3(0.0,0.0,0.0);
	_up = glm::vec3(0.0,0.0,1.0);
	_q = glm::angleAxis(90.0f, glm::vec3(1,0,0));
	 
	glGenBuffers(1, &_vboUBId);
	glBindBuffer(GL_UNIFORM_BUFFER, _vboUBId);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(float)*32, 0, GL_STREAM_DRAW);
	glBindBufferBase(GL_UNIFORM_BUFFER, 0, _vboUBId);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}


Camera::~Camera(){
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	glDeleteBuffers(1, &_vboUBId);
}


Camera * Camera::getInstance(){
	static Camera instance;
	return &instance;
}


void Camera::put(){
	float racio = (float)glutGet(GLUT_WINDOW_WIDTH) / (float)glutGet(GLUT_WINDOW_HEIGHT);
	glm::mat4 projection;
	glm::vec3 eye = _eye;
	eye.y *= _distance;
	_view = glm::lookAt(eye, _center, _up);

	//Ajusta o racio da janela e muda de projeccao
	if(_type){
		if (racio > 1)
			projection = glm::ortho(-1.5*racio*_distance/4, 
									1.5*racio*_distance/4, -1.5*_distance/4, 
									1.5*_distance/4, 1.0*_distance/4, 10.0*_distance/4);
		else 
			projection = glm::ortho(-1.5*_distance/4, 1.5*_distance/4, 
									-1.5/racio*_distance/4, 1.5/racio*_distance/4, 
									1.0*_distance/4, 10.0*_distance/4);
	}
	else
		projection = glm::perspective(38.0f, racio, 1.0f, 100.0f);

	_view = _view*glm::mat4_cast(_q);

	glBindBuffer(GL_UNIFORM_BUFFER, _vboUBId);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(float)*16, &_view[0][0]);
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(float)*16, sizeof(float)*16, &projection[0][0]);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}


void Camera::rotate(float angleX, float angleZ){
	_q = glm::angleAxis(angleX, glm::vec3(1,0,0)) * glm::angleAxis(angleZ, glm::vec3(0,0,1)) * _q;
}


glm::mat4 Camera::getView(){
	return _view;
}


void Camera::update(){

	// Alternate between orthographic and perspective
	if(Input::getInstance()->keyWasReleased('P'))
		_type = !_type;

	// spheric camera
	glm::vec2 mouse = Input::getInstance()->getMouseMotion();
	if (Input::getInstance()->keyWasPressed(' '))
		rotate(mouse.y, mouse.x);

	// zoom
	_distance = MAX(MIN((_distance + Input::getInstance()->getWheelDirection()), 10), 2);
}