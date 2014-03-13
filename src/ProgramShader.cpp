#include "ProgramShader.h"



ProgramShader::ProgramShader(){}


ProgramShader * ProgramShader::getInstance(){
	static ProgramShader instance;
	return &instance;
}


int ProgramShader::compileShader(char * shaderFile, int shaderType){
	const char* shader = Utils::readFile(shaderFile);
	int shaderId = glCreateShader(shaderType);
	glShaderSource(shaderId, 1, &shader, 0);
	glCompileShader(shaderId);
	return shaderId;
}


int ProgramShader::createShaderProgram(char * vSFile, char * fSFile){
	GLuint vertexShaderId = compileShader(vSFile, GL_VERTEX_SHADER);
	GLuint fragmentShaderId = compileShader(fSFile, GL_FRAGMENT_SHADER);
	GLuint id = glCreateProgram();

	glm::vec3 program;
	program.x = id;
	program.y = vertexShaderId;
	program.z = fragmentShaderId;
	_programId.push_back(program);

	glAttachShader(id, vertexShaderId);
	glAttachShader(id, fragmentShaderId);
	glBindAttribLocation(id, VERTICES, "in_Position");
	glBindAttribLocation(id, NORMALS, "in_Normal");
	glBindAttribLocation(id, UVS, "in_TexCoord");
	glBindAttribLocation(id, TANGENTS, "in_Tangent");
	glBindAttribLocation(id, COLORS, "in_Color");
	glLinkProgram(id);
	glUniformBlockBinding(id, glGetUniformBlockIndex(id, "SharedMatrices"), 0);

	Utils::checkOpenGLError("ERROR: Could not create shaders.");

	return id;
}


void ProgramShader::destroyShaderProgram(){
	for(std::vector<glm::vec3>::iterator iterator = _programId.begin(); iterator < _programId.end(); iterator++){
		glUseProgram(iterator->x);
		glDetachShader(iterator->x, iterator->y);
		glDetachShader(iterator->x, iterator->z);
		glDeleteShader(iterator->y);
		glDeleteShader( iterator->z);
		glDeleteProgram(iterator->x);
	}

	Utils::checkOpenGLError("ERROR: Could not destroy shaders.");
}


const GLuint ProgramShader::getUId(std::string key) {
	if(key.compare("Program") == 0)
		return _activeProgram.x;
	if(key.compare("VertexShader") == 0)
		return _activeProgram.y;
	if(key.compare("FragmentShader") == 0)
		return _activeProgram.z;
}


const GLint ProgramShader::getId(std::string key) {
	return glGetUniformLocation(_activeProgram.x, key.c_str());
}


void ProgramShader::bind(GLuint id){
	glm::vec3 program;
	for(std::vector<glm::vec3>::iterator iterator = _programId.begin(); iterator < _programId.end(); iterator++){
		if(iterator->x == id)
			program = *iterator;
	}
	_activeProgram = program;
	glUseProgram(_activeProgram.x);
}


void ProgramShader::unBind(){
	_activeProgram = glm::vec3(0);
	glUseProgram(0);
}