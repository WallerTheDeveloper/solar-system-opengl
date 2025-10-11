//
// Created by Daniel on 08-Oct-25.
//

#ifndef SOLAR_SYSTEM_OPENGL_TEXTUREMANAGER_H
#define SOLAR_SYSTEM_OPENGL_TEXTUREMANAGER_H

#include "glad/glad.h"

#include <string>
#include <vector>
#include <iostream>


class TextureManager {
public:
  // Textures
  unsigned int createTexture(std::string path, GLenum target,
                                  GLint wrapping, GLint filtering);
  unsigned int createCubemap(std::vector<std::string> faces);
private:
  unsigned int generateTexture(unsigned int count, GLenum target);
  void setTextureWrappingParamsInt(GLint parameter);
  void setTextureFilteringParamsInt(GLint parameter);
  unsigned char* loadTextureImage(const char* filename, int& width, int& height, int& numberOfChannels);
  void specifyTextureImage2D(unsigned char* data, unsigned int format, unsigned int width, unsigned int height, bool generateMipmap);

  unsigned int loadCubemap(std::vector<std::string> faces);

};

#endif  // SOLAR_SYSTEM_OPENGL_TEXTUREMANAGER_H
