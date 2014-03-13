#include "GameManager.h"



GameManager::GameManager(){
	_state = QUAD;
	_programa = 0;
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
	GLuint program = ProgramShader::getInstance()->createShaderProgram("shaders/vertex.glsl", "shaders/skybox.glsl");
	_programas[0] = ProgramShader::getInstance()->createShaderProgram("shaders/vertex.glsl", "shaders/fragment.glsl");
	_programas[1] = ProgramShader::getInstance()->createShaderProgram("shaders/vertex.glsl", "shaders/sphereMapping.glsl");
	_programas[2] = ProgramShader::getInstance()->createShaderProgram("shaders/vertex.glsl", "shaders/cubeMapping-v6.glsl");

	// Light
	_light = new Light(glm::vec3(-3.0,0.0,9.0), 
					   glm::vec3(0.1,0.1,0.1), 
					   glm::vec3(1.0,1.0,1.0), 
					   glm::vec3(1.0, 1.0, 1.0),
					   glm::vec3(0.0, 0.025, 0.002));

	// SkyBox
	_skybox = new SkyBox("skybox");
	_skybox->setProgram(program);
	_skybox->scale(15, 15, 15);
	_skybox->rotate(1, 0, 0, 90);
	_skybox->setMesh("mesh/invCube.ply", "materials/skybox.mtl");
	_skybox->setTexture("textures/Cubemap.tga");
	

	/**/    // Quad
	Object * quad = new Object("quad");
	quad->setProgram(_programas[0]);
	quad->setMesh("mesh/quad.obj", "materials/ruby.mtl");
	quad->setTexture("textures/stone.tga");
	quad->setNormalTexture("textures/stone_normal.tga");
	add(quad);

	/**/    // Cube
	Object * cube = new Object("cube");
	cube->setProgram(_programas[0]);
	cube->setMesh("mesh/cube.obj", "materials/ruby.mtl");
	cube->setTexture("textures/stone.tga");
	cube->setNormalTexture("textures/stone_normal.tga");
	add(cube);
	
	/**/    // Sphere
	Object * sphere = new Object("sphere");
	sphere->setProgram(_programas[0]);
	sphere->setMesh("mesh/sphere.obj", "materials/ruby.mtl");
	sphere->setTexture("textures/stone.tga");
	sphere->setNormalTexture("textures/stone_normal.tga");
	add(sphere);

	/**/    // Torus
	Object * torus = new Object("torus");
	torus->setProgram(_programas[0]);
	torus->setMesh("mesh/torus.obj", "materials/ruby.mtl");
	torus->setTexture("textures/stone.tga");
	torus->setNormalTexture("textures/stone_normal.tga");
	add(torus);

	/**/    // Teapot
	Object * teapot = new Object("teapot");
	teapot->setProgram(_programas[0]);
	teapot->setMesh("mesh/teapot.obj", "materials/ruby.mtl");
	teapot->setTexture("textures/stone.tga");
	teapot->setNormalTexture("textures/stone_normal.tga");
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
		_programa = (_programa + 1) % 3;
		for (entityIterator i = _entities.begin(); i != _entities.end(); i++){
			i->second->setProgram(_programas[_programa]);
			switch (_programa){
				case 0:
					i->second->setTexture("textures/stone.tga");
					i->second->setNormalTexture("textures/stone_normal.tga");
					std::cout << "Bump Mapping" << std::endl;
					break;
				case 1:
					i->second->setTexture("textures/sphereMap.tga");
					std::cout << "Sphere Mapping" << std::endl;
					break;
				case 2:
					i->second->setTexture("textures/SkyBox/cubeMap-right.tga", "textures/SkyBox/cubeMap-left.tga", "textures/SkyBox/cubeMap-top.tga", "textures/SkyBox/cubeMap-bottom.tga", "textures/SkyBox/cubeMap-back.tga", "textures/SkyBox/cubeMap-front.tga");
					std::cout << "Cube Mapping v6" << std::endl;
					break;
			}
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