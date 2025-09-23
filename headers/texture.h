#ifndef TEXTURE_H
#define TEXTURE_H

#include <glad/glad.h>
#include <stb_image/stb_image.h>

#include <string>
#include <vector>

class Texture
{
public:
  unsigned int generateTexture(unsigned int count, GLenum target);
  unsigned char* loadTextureImage(const char* filename, int& width, int& height, int& numberOfChannels);
  void specifyTextureImage2D(unsigned char* data, unsigned int format, unsigned int width, unsigned int height, bool generateMipmap);
  void setTextureWrappingParamsInt(GLint parameter);
  void setTextureFilteringParamsInt(GLint parameter);
  void freeImageData(unsigned char* data);
  void setTextureActive2D(GLenum texture, unsigned int& textureID);
  unsigned int loadCubemap(std::vector<std::string> faces);
};

#endif