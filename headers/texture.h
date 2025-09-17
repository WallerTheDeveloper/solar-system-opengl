#ifndef TEXTURE_H
#define TEXTURE_H

#include <stb_image/stb_image.h>
#include <glad/glad.h>

class Texture
{
public:
  unsigned int generateTexture(unsigned int count);
  unsigned char* loadTextureImage(const char* filename, int& width, int& height, int& numberOfChannels);
  void specifyTextureImage2D(unsigned char* data, unsigned int format, unsigned int width, unsigned int height, bool generateMipmap);
  void setTextureWrappingParamsInt(GLint parameter);
  void setTextureFilteringParamsInt(GLint parameter);
  void freeImageData(unsigned char* data);
  void setTextureActive2D(GLenum texture, unsigned int& textureID);
};

#endif