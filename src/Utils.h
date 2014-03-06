#pragma once

#include "GL/glew.h"
#include "GL/freeglut.h"

#include "ProgramShader.h"

#include <fstream>
#include <iostream>
#include <vector>
#include <sstream>
#include <gtc/quaternion.hpp>

#define VERTICES 0
#define COLORS 1
#define NORMALS 2
#define UVS 3

#define MAX(a,b) (((a)>(b))?(a):(b))
#define MIN(a,b) (((a)<(b))?(a):(b))
#define SWAP(T, a, b) {T temp = a; a = b; b = temp;}

#define TGA_RGB		2
#define TGA_A		3
#define TGA_RLE		10

typedef GLushort WORD;
typedef GLubyte byte;

typedef struct {
	GLfloat XYZW[4];
	GLfloat RGBA[4];
} Vertex;

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
	void loadObj(char* filename, std::vector<unsigned int> &indices, std::vector<glm::vec3> &out_vertices, std::vector<glm::vec2> &out_uvs, std::vector<glm::vec3> &out_normals);
	void loadMaterial(char* filename, glm::vec3 &ambient, glm::vec3 &diffuse, glm::vec3 &specular, float &shininess);
	tImageTGA *loadTexture(char *filename);
}
