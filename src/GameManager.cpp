#include "GameManager.h"



GameManager::GameManager(){
	_state = QUAD;
}


std::string GameManager::enumToId(Objects e){
	switch (e){
		case QUAD:
			return "quad";
		case CUBE:
			return "cube";
		case SPHERE:
			return "sphere";
		case TORUS:
			return "torus";
		case TEAPOT:
			return "teapot";
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
	
	// Programs
	GLuint program1 = ProgramShader::getInstance()->createShaderProgram("shaders/vertex.glsl", "shaders/skybox.glsl");
	GLuint program2 = ProgramShader::getInstance()->createShaderProgram("shaders/vertex.glsl", "shaders/fragment.glsl");
	GLuint program3 = ProgramShader::getInstance()->createShaderProgram("shaders/vertex.glsl", "shaders/sphereMapping.glsl");
	GLuint program4 = ProgramShader::getInstance()->createShaderProgram("shaders/vertex.glsl", "shaders/cubeMapping-v1.glsl");
	GLuint program5 = ProgramShader::getInstance()->createShaderProgram("shaders/vertex.glsl", "shaders/cubeMapping-v6.glsl");

	// Light
	_light = new Light(glm::vec3(-3.0,0.0,9.0), 
					   glm::vec3(0.1,0.1,0.1), 
					   glm::vec3(1.0,1.0,1.0), 
					   glm::vec3(1.0, 1.0, 1.0),
					   glm::vec3(0.0, 0.025, 0.002));

	// SkyBox
	_skybox = new SkyBox("skybox");
	_skybox->setProgram(program1);
	_skybox->scale(15, 15, 15);
	_skybox->setMesh("mesh/invCube.obj", "materials/skybox.mtl");
	_skybox->setTexture("textures/Cubemap.tga", "textures/stone_normal.tga");
	

	/**/    // Quad
	Object * quad = new Object("quad");
	quad->setProgram(program2);
	quad->rotate(1, 0, 0, 90);
	quad->setMesh("mesh/quad.obj", "materials/ruby.mtl");
	quad->setTexture("textures/stone.tga", "textures/stone_normal.tga");
	add(quad);

	/**/    // Cube
	Object * cube = new Object("cube");
	cube->setProgram(program2);
	cube->setMesh("mesh/cube.obj", "materials/ruby.mtl");
	cube->setTexture("textures/stone.tga", "textures/stone_normal.tga");
	add(cube);
	
	/**/    // Sphere
	Object * sphere = new Object("sphere");
	sphere->setProgram(program2);
	sphere->setMesh("mesh/sphere.obj", "materials/ruby.mtl");
	sphere->setTexture("textures/stone.tga", "textures/stone_normal.tga");
	add(sphere);

	/**/    // Torus
	Object * torus = new Object("torus");
	torus->setProgram(program2);
	torus->setMesh("mesh/torus.obj", "materials/ruby.mtl");
	torus->setTexture("textures/stone.tga", "textures/stone_normal.tga");
	add(torus);

	/**/    // Teapot
	Object * teapot = new Object("teapot");
	teapot->setProgram(program2);
	teapot->setMesh("mesh/teapot.obj", "materials/ruby.mtl");
	teapot->setTexture("textures/stone.tga", "textures/stone_normal.tga");
	add(teapot);

	/**/
}


void GameManager::draw(){
	GLuint program = _skybox->getProgram(); 

	ProgramShader::getInstance()->bind(program);
	Camera::getInstance()->put();
	_skybox->draw();
	ProgramShader::getInstance()->unBind();

	program = getEntityById(enumToId(_state))->getProgram();
	
	ProgramShader::getInstance()->bind(program);
	Camera::getInstance()->put();
	_light->setShaderLightValues();
	getEntityById(enumToId(_state))->draw();
	ProgramShader::getInstance()->unBind();
}


void GameManager::update(){
	
	//Object Change
	if (Input::getInstance()->keyWasReleased('T')){
		_state = (Objects)((_state + 1) % 5);
		((Object*)getEntityById(enumToId(_state)))->reset();
	}

	//Object Program
	if (Input::getInstance()->keyWasReleased('Y')){
		for (entityIterator i = _entities.begin(); i != _entities.end(); i++){
			//i->second->;
		}
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