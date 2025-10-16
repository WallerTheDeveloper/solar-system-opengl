//
// Created by Daniel on 08-Oct-25.
//

#include "TextureManager.h"

#include "stb_image/stb_image.h"
#include <utils/debug_utils.h>

unsigned int TextureManager::createTexture(std::string path, GLenum target,
                                                GLint wrapping,
                                                GLint filtering) {
  const unsigned int textureID = generateTexture(1, target);

  // examples: GL_REPEAT - wrapping, GL_LINEAR - filtering
  GL_CHECK(setTextureWrappingParamsInt(wrapping));
  GL_CHECK(setTextureFilteringParamsInt(filtering));

  int width = 0, height = 0, numberOfChannels = 0;
  unsigned char* data =
      loadTextureImage(path.c_str(), width, height, numberOfChannels);

  if (data && width > 0 && height > 0) {
    std::cout << "Texture loaded: " << width << "x" << height << " ("
         << numberOfChannels << " channels)" << std::endl;

    GLenum format = GL_RGB;
    if (numberOfChannels == 1)
      format = GL_RED;
    else if (numberOfChannels == 3)
      format = GL_RGB;
    else if (numberOfChannels == 4)
      format = GL_RGBA;

    GL_CHECK(specifyTextureImage2D(data, format, width, height, true));
    stbi_image_free(data);
  } else {
    std::cout << "Failed to load texture: " << path << " - using fallback color"
         << std::endl;

    GL_CHECK(specifyTextureImage2D(data, GL_RGB, 1, 1, false));
    if (data) {
      stbi_image_free(data);
    }
  }

  // checkTextureBinding(GL_TEXTURE0);
  return textureID;
}

unsigned int TextureManager::createCubemap(std::vector<std::string> faces) {
  std::cout << "Creating cubemap with files:" << std::endl;
  for (const auto& face : faces) {
    std::cout << "  " << face << std::endl;
  }

  unsigned int cubemapTexture = loadCubemap(faces);
  std::cout << "Cubemap created with ID: " << cubemapTexture << std::endl;

  return cubemapTexture;
}

unsigned int TextureManager::generateTexture(unsigned int count,
                                             GLenum target) {
  unsigned int texture;
  glGenTextures(count, &texture);
  glBindTexture(target, texture);

  return texture;
}

void TextureManager::setTextureWrappingParamsInt(GLint parameter) {
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, parameter);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, parameter);
}
void TextureManager::setTextureFilteringParamsInt(GLint parameter) {
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, parameter);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, parameter);
}
unsigned char* TextureManager::loadTextureImage(const char* filename,
                                                int& width, int& height,
                                                int& numberOfChannels) {
  stbi_set_flip_vertically_on_load(false);
  unsigned char* data =
      stbi_load(filename, &width, &height, &numberOfChannels, 0);
  return data;
}
void TextureManager::specifyTextureImage2D(unsigned char* data,
                                           unsigned int format,
                                           unsigned int width,
                                           unsigned int height,
                                           bool generateMipmap) {
  glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format,
               GL_UNSIGNED_BYTE, data);
  if (generateMipmap) {
    glGenerateMipmap(GL_TEXTURE_2D);
  }
}
unsigned int TextureManager::loadCubemap(std::vector<std::string> faces) {
  unsigned int textureID;
  glGenTextures(1, &textureID);
  glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);


  stbi_set_flip_vertically_on_load(true);

  int width, height, nrChannels;
  for (unsigned int i = 0; i < faces.size(); i++) {
    unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);

    if (data) {
      GLenum format;
      if (nrChannels == 1)
        format = GL_RED;
      else if (nrChannels == 3)
        format = GL_RGB;
      else if (nrChannels == 4)
        format = GL_RGBA;
      else {
        std::cout << "Unsupported number of channels (" << nrChannels << ") for cubemap face: " << faces[i] << std::endl;
        format = GL_RGB; // fallback
      }

      std::cout << "Loading cubemap face " << i << " (" << faces[i] << "): "
                << width << "x" << height << " with " << nrChannels << " channels" << std::endl;

      glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, width, height,
                   0, format, GL_UNSIGNED_BYTE, data);
      stbi_image_free(data);
    } else {
      std::cerr << "ERROR: Cubemap texture failed to load at path: " << faces[i] << std::endl;
      stbi_image_free(data);
    }
  }

  // Set texture parameters
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

  // Reset stbi flip setting for other textures
  stbi_set_flip_vertically_on_load(false);

  std::cout << "Cubemap texture created successfully with ID: " << textureID << std::endl;
  return textureID;
}