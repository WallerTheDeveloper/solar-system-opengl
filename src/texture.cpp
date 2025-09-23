#include "texture.h"

#include <iostream>

unsigned int Texture::generateTexture(unsigned int count, GLenum target) {
  unsigned int texture;
  glGenTextures(count, &texture);
  glBindTexture(target, texture);

  return texture;
}

void Texture::setTextureWrappingParamsInt(GLint parameter) {
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, parameter);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, parameter);
}

void Texture::setTextureFilteringParamsInt(GLint parameter) {
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, parameter);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, parameter);
}

unsigned char* Texture::loadTextureImage(const char* filename, int& width,
                                         int& height, int& numberOfChannels) {
  stbi_set_flip_vertically_on_load(true);
  unsigned char* data =
      stbi_load(filename, &width, &height, &numberOfChannels, 0);
  return data;
}

void Texture::freeImageData(unsigned char* data) { stbi_image_free(data); }

void Texture::specifyTextureImage2D(unsigned char* data, unsigned int format,
                                    unsigned int width, unsigned int height,
                                    bool generateMipmap = true) {
  glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format,
               GL_UNSIGNED_BYTE, data);
  if (generateMipmap) {
    glGenerateMipmap(GL_TEXTURE_2D);
  }
}

void Texture::setTextureActive2D(GLenum texture, unsigned int& textureID) {
  glActiveTexture(texture);
  glBindTexture(GL_TEXTURE_2D, textureID);
}

unsigned int Texture::loadCubemap(std::vector<std::string> faces) {
  unsigned int textureID;
  glGenTextures(1, &textureID);
  glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

  int width, height, nrChannels;
  for (unsigned int i = 0; i < faces.size(); i++) {
    unsigned char* data =
        stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
    if (data) {
      glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height,
                   0, GL_RGB, GL_UNSIGNED_BYTE, data);
      stbi_image_free(data);
    } else {
      std::cout << "Cubemap tex failed to load at path: " << faces[i]
                << std::endl;
      stbi_image_free(data);
    }
  }
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

  return textureID;
}