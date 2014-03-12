#pragma once

#include "Utils.h"
#include "Mesh.h"
#include "Texture.h"
#include "TextureCube.h"

#include <gtx/transform2.hpp>
#include <glm.hpp>
#include <iostream>



class Entity {

	private:
		glm::quat _q;
		glm::mat4 _matrix;
		float _height;
		Mesh * _mesh;
		Texture * _texture;
		TextureCube * _textureCube;
		GLuint _program;

	protected:
		float _px, _py, _pz;
		std::string _id;
		Entity(std::string id);

	public:
		virtual ~Entity();
		virtual void update() = 0;
		virtual void draw();
		glm::vec3 getPos();
		glm::quat getQuat();
		std::string getId();
		void setMesh(char * filename, char * mtl);
		Mesh * getMesh();
		void setProgram(GLuint id);
		GLuint getProgram();
		void setTexture(char * filename1, char * filename2);
		void setTextureCube();
		void setRotation(glm::quat q);
		void setTranslation(float x, float y, float z);
		void rotate(float x, float y, float z, float angle);
		void translate(float x, float y, float z);
		void scale(float x, float y, float z);
		void shear(float x, float y);
		void lerp(float x, float y, float z, float k);
};