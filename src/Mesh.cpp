#include "Mesh.h"




Mesh::Mesh(char * objFile, char * mtlFile){

	Utils::loadMesh(objFile, _indices, _vertices, _uvs, _normals);
	_tangents = Utils::calculateTangents(_vertices, _uvs, _normals);

	glGenVertexArrays(1, &_vaoId);
	glBindVertexArray(_vaoId);

	glGenBuffers(4, _vbo);

	glBindBuffer(GL_ARRAY_BUFFER, _vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(glm::vec3), &_vertices[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(VERTICES);
	glVertexAttribPointer(VERTICES, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, _vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, _uvs.size() * sizeof(glm::vec2), &_uvs[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(UVS);
	glVertexAttribPointer(UVS, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

	Utils::loadMaterial(mtlFile, _ambientColor, _diffuseColor, _specularColor, _shininess);
	
	glBindBuffer(GL_ARRAY_BUFFER, _vbo[2]);
	glBufferData(GL_ARRAY_BUFFER, _normals.size() * sizeof(glm::vec3), &_normals[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(NORMALS);
	glVertexAttribPointer(NORMALS, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	
	glBindBuffer(GL_ARRAY_BUFFER, _vbo[3]);
	glBufferData(GL_ARRAY_BUFFER, _tangents.size() * sizeof(glm::vec4), &_tangents[0], GL_STATIC_DRAW);
	glEnableVertexAttribArray(TANGENTS);
	glVertexAttribPointer(TANGENTS, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);
	
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glDisableVertexAttribArray(VERTICES);
	glDisableVertexAttribArray(UVS);
	glDisableVertexAttribArray(NORMALS);
	glDisableVertexAttribArray(TANGENTS);

	Utils::checkOpenGLError("ERROR: Could not create VAOs and VBOs.");
}


void Mesh::draw(){
	glBindVertexArray(_vaoId);

	// Get IDs
	GLint ambientId = ProgramShader::getInstance()->getId("MaterialAmbient");
	GLint diffuseId = ProgramShader::getInstance()->getId("MaterialDiffuse");
	GLint specularId = ProgramShader::getInstance()->getId("MaterialSpecular");
	GLint shininessId = ProgramShader::getInstance()->getId("MaterialShininess");

	// Set Values
	glUniform1f(shininessId, _shininess);
	glUniform3fv(ambientId, 1, glm::value_ptr(_ambientColor));
	glUniform3fv(diffuseId, 1, glm::value_ptr(_diffuseColor));
	glUniform3fv(specularId, 1, glm::value_ptr(_specularColor));

	glDrawArrays(GL_TRIANGLES, 0, _vertices.size());
}


void Mesh::setMaterial(char * mtl){
	Utils::loadMaterial(mtl, _ambientColor, _diffuseColor, _specularColor, _shininess);
}


Mesh::~Mesh(){

	glDisableVertexAttribArray(VERTICES);
	glDisableVertexAttribArray(UVS);
	glDisableVertexAttribArray(NORMALS);
	glDisableVertexAttribArray(TANGENTS);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	glDeleteBuffers(4, _vbo);
	glDeleteVertexArrays(1, &_vaoId);

	Utils::checkOpenGLError("ERROR: Could not destroy VAOs and VBOs.");
}
