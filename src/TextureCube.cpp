#include "TextureCube.h"

TextureCube::TextureCube(){

	tImageTGA* image[6];

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

	image[0] = Utils::loadTexture("textures/SkyBox/cubeMap-right.tga");
	image[1] = Utils::loadTexture("textures/SkyBox/cubeMap-left.tga");
	image[2] = Utils::loadTexture("textures/SkyBox/cubeMap-top.tga");
	image[3] = Utils::loadTexture("textures/SkyBox/cubeMap-bottom.tga");
	image[4] = Utils::loadTexture("textures/SkyBox/cubeMap-back.tga");
	image[5] = Utils::loadTexture("textures/SkyBox/cubeMap-front.tga");

	GLuint textureType[6];

	for (int i = 0; i < 6; i++) {
		textureType[i] = GL_RGB;
		if (image[i]->channels == 4)
			textureType[i] = GL_RGBA;
	}

	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, image[0]->channels, image[0]->size_x, image[0]->size_y, 0, textureType[0], GL_UNSIGNED_BYTE, image[0]->data);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, image[1]->channels, image[1]->size_x, image[1]->size_y, 0, textureType[1], GL_UNSIGNED_BYTE, image[1]->data);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, image[2]->channels, image[2]->size_x, image[2]->size_y, 0, textureType[2], GL_UNSIGNED_BYTE, image[2]->data);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, image[3]->channels, image[3]->size_x, image[3]->size_y, 0, textureType[3], GL_UNSIGNED_BYTE, image[3]->data);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, image[4]->channels, image[4]->size_x, image[4]->size_y, 0, textureType[4], GL_UNSIGNED_BYTE, image[4]->data);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, image[5]->channels, image[5]->size_x, image[5]->size_y, 0, textureType[5], GL_UNSIGNED_BYTE, image[5]->data);
	
	/**/
	for (int i = 0; i < 6; i++) {
		if (image[i]){
			if (image[i]->data)
				free(image[i]->data);
			free(image[i]);
		}
	}
	/**/

	unbind();
}

void TextureCube::bind(){
	
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_CUBE_MAP, _cubeID);
	glUniform1i(ProgramShader::getInstance()->getId("cubeMap"), 3);
	
}

void TextureCube::unbind(){
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}