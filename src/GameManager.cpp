#include "GameManager.h"



GameManager::GameManager(){
	_state = CUBE;
}


std::string GameManager::enumToId(Objects e){
	switch (e){
		case CUBE:
			return "cube";
		case TEAPOT:
			return "teapot";
		case TORUS:
			return "torus";
	}
}


GameManager * GameManager::getInstance(){
	static GameManager instance;
	return &instance;
}


void GameManager::add(Entity * entity){
	_entities.insert(std::make_pair(entity->getId(), entity));
}


Entity * GameManager::getEntityById(std::string id){
	return _entities.find(id)->second;
}


void GameManager::init(){
	
	GLuint program1 = ProgramShader::getInstance()->createShaderProgram("shaders/vertex.glsl", "shaders/sphereMapping.glsl");
	GLuint program2 = ProgramShader::getInstance()->createShaderProgram("shaders/vertex.glsl", "shaders/cubeMapping.glsl");
	GLuint program3 = ProgramShader::getInstance()->createShaderProgram("shaders/vertex.glsl", "shaders/fragment.glsl");


	_light = new Light(glm::vec3(-3.0,0.0,9.0), 
					   glm::vec3(0.1,0.1,0.1), 
					   glm::vec3(1.0,1.0,1.0), 
					   glm::vec3(1.0, 1.0, 1.0),
					   glm::vec3(0.0, 0.025, 0.002));

	/**/

	Object * cube = new Object("cube");
	cube->setProgram(program3);
	cube->setMesh("mesh/cube.obj", "materials/ruby.mtl");
	cube->setTexture("textures/stone.tga", "textures/stone_normal.tga");
	add(cube);
	
	/**/

	Object * teapot = new Object("teapot");
	teapot->scale(0.05, 0.05, 0.05);
	teapot->setProgram(program2);
	teapot->setMesh("mesh/teapot.obj", "materials/ruby.mtl");
	teapot->setTexture("textures/sphereMap.tga", "textures/stone_normal.tga");
	add(teapot);

	/**/

	Object * torus = new Object("torus");
	torus->scale(1.0, 1.0, 1.0);
	torus->setProgram(program1);
	torus->setMesh("mesh/torus2.obj", "materials/ruby.mtl");
	torus->setTexture("textures/sphereMap.tga", "textures/stone_normal.tga");
	add(torus);

	/**/
}


void GameManager::draw(){
	GLuint program = getEntityById(enumToId(_state))->getProgram();
	
	ProgramShader::getInstance()->bind(program);
	Camera::getInstance()->put();
	_light->setShaderLightValues();
	getEntityById(enumToId(_state))->draw();
	ProgramShader::getInstance()->unBind();
}


void GameManager::update(){
	
	//Object Change
	if (Input::getInstance()->keyWasReleased('T')){
		_state = (Objects)((_state + 1) % 3);
		((Object*)getEntityById(enumToId(_state)))->reset();
	}

	_light->update();

	// Camera and entities update
	Camera::getInstance()->update();
	for (entityIterator i = _entities.begin(); i != _entities.end(); i++)
		i->second->update();
}



void GameManager::postProcessing(){}


void GameManager::destroyBufferObjects(){
	ProgramShader::getInstance()->destroyShaderProgram();
	for (entityIterator i = _entities.begin(); i != _entities.end(); i++)
		i->second->~Entity();
	Camera::getInstance()->~Camera();
}