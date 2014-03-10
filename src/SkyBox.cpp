#include "SkyBox.h"



SkyBox::SkyBox(std::string id) : Entity(id){

	tImageTGA* image;
	int textureType;

	/* Don't do this at home! */

	glActiveTexture(GL_TEXTURE3);
	glEnable(GL_TEXTURE_CUBE_MAP);
	glGenTextures(1, &_cubeID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, _cubeID);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	for (int i = 0; i < 6; i++) {
		if (i == 0)
			image = Utils::loadTexture("textures/SkyBox/cubeMap-right.tga");
		else if (i == 1)
			image = Utils::loadTexture("textures/SkyBox/cubeMap-left.tga");
		else if (i == 2)
			image = Utils::loadTexture("textures/SkyBox/cubeMap-top.tga");
		else if (i == 3)
			image = Utils::loadTexture("textures/SkyBox/cubeMap-bottom.tga");
		else if (i == 4)
			image = Utils::loadTexture("textures/SkyBox/cubeMap-back.tga");
		else if (i == 5)
			image = Utils::loadTexture("textures/SkyBox/cubeMap-front.tga");
		else
			std::cerr << "teste" << std::endl;

		textureType = GL_RGB;
		if (image->channels == 4)
			textureType = GL_RGBA;

		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, image->channels, image->size_x, image->size_y, 0, textureType, GL_UNSIGNED_BYTE, image->data);

		if (image){
			if (image->data)
				free(image->data);
			free(image);
		}
	}
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}


void SkyBox::update(){}

void SkyBox::draw(){
	std::cout << "teste1" << std::endl;
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_CUBE_MAP, _cubeID);
	glUniform1i(ProgramShader::getInstance()->getId("cubeMap"), 3);
	std::cout << "teste2" << std::endl;
}