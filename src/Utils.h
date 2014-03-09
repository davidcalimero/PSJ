#pragma once

#include "ply.h"
#include "GL/glew.h"
#include "GL/freeglut.h"


#include "ProgramShader.h"

#include <fstream>
#include <iostream>
#include <vector>
#include <sstream>
#include <gtc/quaternion.hpp>
#include <algorithm>

#define VERTICES 0
#define COLORS 1
#define NORMALS 2
#define UVS 3
#define TANGENTS 4

#define MAX(a,b) (((a)>(b))?(a):(b))
#define MIN(a,b) (((a)<(b))?(a):(b))
#define SWAP(T, a, b) {T temp = a; a = b; b = temp;}

#define TGA_RGB		2
#define TGA_A		3
#define TGA_RLE		10

typedef GLushort WORD;
typedef GLubyte byte;

typedef struct {
	GLfloat XYZ[3];
	GLfloat N[3];
	GLfloat UV[2];
} Vertex;

typedef struct Face {
	unsigned int count;
	unsigned int *vertices;
	GLfloat N[3];
	GLfloat UV[2];
} Face;

typedef struct tImageTGA
{
	int channels;
	int size_x;
	int size_y;
	unsigned char *data;
} tImageTGA;

namespace Utils {

	bool isOpenGLError();
	void checkOpenGLError(std::string error);	
	char * readFile(char * file);
	void loadMesh(char* filename, std::vector<unsigned int> &indices, std::vector<glm::vec3> &out_vertices, std::vector<glm::vec2> &out_uvs, std::vector<glm::vec3> &out_normals);
	void loadObj(char* filename, std::vector<unsigned int> &indices, std::vector<glm::vec3> &out_vertices, std::vector<glm::vec2> &out_uvs, std::vector<glm::vec3> &out_normals);
	void loadPLY(char *objFile, std::vector<unsigned int> &indices, std::vector<glm::vec3> &out_vertices, std::vector<glm::vec2> &out_uvs, std::vector<glm::vec3> &out_normals);
	std::vector<glm::vec4> calculateTangents(std::vector<glm::vec3> &_vertices, std::vector<glm::vec2> &_uvs, std::vector<glm::vec3> &_normals);
	void loadMaterial(char* filename, glm::vec3 &ambient, glm::vec3 &diffuse, glm::vec3 &specular, float &shininess);
	tImageTGA *loadTexture(char *filename);
}
