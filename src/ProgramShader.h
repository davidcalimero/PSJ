#pragma once

#include "Utils.h"

#include <iostream>
#include <vector>
#include <glm.hpp>



class ProgramShader {

	private:
		glm::vec3 _activeProgram;
		std::vector<glm::vec3> _programId;
		int compileShader(char * shaderFile, int shaderType);
		ProgramShader();

	public:
		static ProgramShader * getInstance();
		int createShaderProgram(char * vSFile, char * fSFile);
		void destroyShaderProgram();
		const GLuint getUId(std::string key);
		const GLint getId(std::string key);
		void bind(GLuint id);
		void unBind();
};