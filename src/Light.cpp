#include "Light.h"


Light::Light(glm::vec3 position, glm::vec3 ambient, glm::vec3 diffuse, glm::vec3 specular, glm::vec3 attenuation){
	_position = position;
	_ambientColor = ambient;
	_diffuseColor = diffuse;
	_specularColor = specular;
	_attenuation = attenuation;
}

void Light::setShaderLightValues(){
	// Get IDs
	GLint positionId = ProgramShader::getInstance()->getId("LightPosition");
	GLint ambientId = ProgramShader::getInstance()->getId("AmbientLight");
	GLint diffuseId = ProgramShader::getInstance()->getId("DiffuseLight");
	GLint specularId = ProgramShader::getInstance()->getId("SpecularLight");
	GLint attenuationId = ProgramShader::getInstance()->getId("LightAttenuation");
	
	glm::vec3 position = _position;

	// Set Values
	glUniform3fv(positionId, 1, glm::value_ptr(position));
	glUniform3fv(ambientId, 1, glm::value_ptr(_ambientColor));
	glUniform3fv(diffuseId, 1, glm::value_ptr(_diffuseColor));
	glUniform3fv(specularId, 1, glm::value_ptr(_specularColor));
	glUniform3fv(attenuationId, 1, glm::value_ptr(_attenuation));
}

void Light::update(){
	if (Input::getInstance()->keyWasPressed('A') && glm::length(_position) > 5)
		_position -= glm::normalize(_position)*0.01f;
	if (Input::getInstance()->keyWasPressed('B') && glm::length(_position) < 25)
		_position += glm::normalize(_position)*0.01f;
}