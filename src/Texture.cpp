#include "Texture.h"

Texture::Texture(char * textureName1, char * textureName2) {
	tImageTGA* image;
	int textureType;
	glGenTextures(2, _textureID);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _textureID[0]);
	image = Utils::loadTexture(textureName1);
	
	textureType = GL_RGB;
	if (image->channels == 4)
		textureType = GL_RGBA;
	
	glTexImage2D(GL_TEXTURE_2D, 0, image->channels, image->size_x, image->size_y, 0, textureType, GL_UNSIGNED_BYTE, image->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	if (image){
		if (image->data)
			free(image->data);
		free(image);
	}

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, _textureID[1]);
	image = Utils::loadTexture(textureName2);

	textureType = GL_RGB;
	if (image->channels == 4)
		textureType = GL_RGBA;

	glTexImage2D(GL_TEXTURE_2D, 0, image->channels, image->size_x, image->size_y, 0, textureType, GL_UNSIGNED_BYTE, image->data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	if (image){
		if (image->data)
			free(image->data);
		free(image);
	}
}

Texture::~Texture(){
	glDeleteTextures(2, _textureID);
}

void Texture::bind() {
	glBindTexture(GL_TEXTURE_2D, _textureID[0]);
	glUniform1i(ProgramShader::getInstance()->getId("tex1"), 0);
	glBindTexture(GL_TEXTURE_2D, _textureID[1]);
	glUniform1i(ProgramShader::getInstance()->getId("tex2"), 1);
}

void Texture::unbind() {
	glBindTexture(GL_TEXTURE_2D, 0);
}
