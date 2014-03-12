#include "Texture.h"

Texture::Texture(int slot){
	_slot = slot;
	glGenTextures(1, &_textureID);
}

Texture::Texture(char * textureName, int slot) {
	tImageTGA* image;
	int textureType;
	_slot = slot;

	glGenTextures(1, &_textureID);

	glActiveTexture(_slot);
	glBindTexture(GL_TEXTURE_2D, _textureID);
	image = Utils::loadTexture(textureName);
	
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

	unbind();
}

Texture::~Texture(){
	glDeleteTextures(1, &_textureID);
}

void Texture::bind() {
	glActiveTexture(_slot);
	glBindTexture(GL_TEXTURE_2D, _textureID);
	
	switch (_slot){
		case GL_TEXTURE0:
			glUniform1i(ProgramShader::getInstance()->getId("baseTexture"), 0);
			break;
		case GL_TEXTURE1:
			glUniform1i(ProgramShader::getInstance()->getId("normalTexture"), 1);
			break;
	}
}

void Texture::unbind() {
	glBindTexture(GL_TEXTURE_2D, 0);
}
