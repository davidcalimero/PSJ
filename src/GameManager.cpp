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
	
	GLuint program1 = ProgramShader::getInstance()->createShaderProgram("shaders/vertex.glsl", "shaders/fragment.glsl");
	GLuint program2 = ProgramShader::getInstance()->createShaderProgram("shaders/vertex.glsl", "shaders/sphereMapping.glsl");
	GLuint program3 = ProgramShader::getInstance()->createShaderProgram("shaders/vertex.glsl", "shaders/cubeMapping-v1.glsl");
	GLuint program4 = ProgramShader::getInstance()->createShaderProgram("shaders/vertex.glsl", "shaders/cubeMapping-v6.glsl");


	_light = new Light(glm::vec3(-3.0,0.0,9.0), 
					   glm::vec3(0.1,0.1,0.1), 
					   glm::vec3(1.0,1.0,1.0), 
					   glm::vec3(1.0, 1.0, 1.0),
					   glm::vec3(0.0, 0.025, 0.002));

	/**/
	// Bump Mapping - Quad
	Object * quad = new Object("quad");
	quad->setProgram(program1);
	quad->rotate(1, 0, 0, 90);
	quad->setMesh("mesh/quad.obj", "materials/ruby.mtl");
	quad->setTexture("textures/stone.tga", "textures/stone_normal.tga");
	add(quad);

	/**/
	// Bump Mapping - Cube
	Object * cube = new Object("cube");
	cube->setProgram(program1);
	cube->setMesh("mesh/cube.obj", "materials/ruby.mtl");
	cube->setTexture("textures/stone.tga", "textures/stone_normal.tga");
	add(cube);
	
	/**/
	// Cube Mapping with 1 image (Cubo Desconstruído) - Sphere
	Object * sphere = new Object("sphere");
	sphere->setProgram(program3);
	sphere->setMesh("mesh/sphere.obj", "materials/ruby.mtl");
	sphere->setTexture("textures/cubeMap.tga", "textures/stone_normal.tga");
	add(sphere);

	/**/
	// Cube Mapping with 6 images (TextureCube) - Torus
	Object * torus = new Object("torus");
	torus->setProgram(program4);
	torus->setMesh("mesh/torus.obj", "materials/ruby.mtl");
	torus->setTexture("textures/cubeMap.tga", "textures/stone_normal.tga");
	torus->setTextureCube();
	add(torus);

	/**/
	// Sphere Mapping - Teapot
	Object * teapot = new Object("teapot");
	teapot->setProgram(program2);
	teapot->setMesh("mesh/teapot.obj", "materials/ruby.mtl");
	teapot->setTexture("textures/sphereMap.tga", "textures/stone_normal.tga");
	add(teapot);
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