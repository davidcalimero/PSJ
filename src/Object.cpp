#include "Object.h"



Object::Object(std::string id) : Entity(id){
	_state = NR;
}


void Object::reset() {
	_state = NR;
	setRotation(glm::quat());
}


void Object::update(){

	//Muda estado do objecto de rotação
	if (Input::getInstance()->specialWasReleased(GLUT_KEY_LEFT))
		_state = RX;

	if (Input::getInstance()->specialWasReleased(GLUT_KEY_UP))
		_state = RY;

	if (Input::getInstance()->specialWasReleased(GLUT_KEY_RIGHT))
		_state = RZ;

	// rodar por incremento
	if (Input::getInstance()->mouseWasPressed(GLUT_LEFT_BUTTON))
		rotate(1, 0, 0, 0.25);

	if (Input::getInstance()->mouseWasPressed(GLUT_MIDDLE_BUTTON))
		rotate(0, 1, 0, 0.25);

	if (Input::getInstance()->mouseWasPressed(GLUT_RIGHT_BUTTON))
		rotate(0, 0, 1, 0.25);

	if (Input::getInstance()->keyWasReleased('R'))
		reset();

	if (Input::getInstance()->keyWasReleased('O'))
		_state = NR;



	//Muda material do objecto
	if (Input::getInstance()->keyWasReleased('1'))
		_mesh->setMaterial("materials/ruby.mtl");

	if (Input::getInstance()->keyWasReleased('2'))
		_mesh->setMaterial("materials/gold.mtl");

	if (Input::getInstance()->keyWasReleased('3'))
		_mesh->setMaterial("materials/silver.mtl");

	if (Input::getInstance()->keyWasReleased('4'))
		_mesh->setMaterial("materials/esmerald.mtl");

	if (Input::getInstance()->keyWasReleased('5'))
		_mesh->setMaterial("materials/cyan.mtl");


	//Roda objecto
	switch (_state) {
		case RX:
			rotate(1, 0, 0, 0.25);
			break;
		case RY:
			rotate(0, 1, 0, 0.25);
			break;
		case RZ:
			rotate(0, 0, 1, 0.25);
			break;
	}

}