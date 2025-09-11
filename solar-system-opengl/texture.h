#ifndef TEXTURE_H
#define TEXTURE_H

#include <stb_image/stb_image.h>
#include <glad/glad.h>

class Texture
{
public:

	unsigned int generateTexture(unsigned int count, unsigned int& texture);
	unsigned char* loadTextureImage(const char* filename, int& width, int& height, int& numberOfChannels);
	void specifyTextureImage2D(unsigned char* data, unsigned int width, unsigned int height, bool generateMipmap);
	void freeImageData(unsigned char* data);
};

#endif