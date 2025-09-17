#include "../headers/texture.h"

unsigned int Texture::generateTexture(unsigned int count)
{
    unsigned texture;
    glGenTextures(count, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    return texture;
}

void Texture::setTextureWrappingParamsInt(GLint parameter)
{
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, parameter);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, parameter);
}

void Texture::setTextureFilteringParamsInt(GLint parameter)
{
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, parameter);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, parameter);
}

unsigned char* Texture::loadTextureImage(const char* filename, int& width, int& height, int& numberOfChannels) {
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(filename, &width, &height, &numberOfChannels, 0);
    return data;
}

void Texture::freeImageData(unsigned char* data)
{
    stbi_image_free(data);
}

void Texture::specifyTextureImage2D(unsigned char* data, unsigned int format, unsigned int width, unsigned int height, bool generateMipmap = true)
{
  glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format,
               GL_UNSIGNED_BYTE, data);
    if (generateMipmap)
    {
        glGenerateMipmap(GL_TEXTURE_2D);
    }
}

void Texture::setTextureActive2D(GLenum texture, unsigned int& textureID)
{
    glActiveTexture(texture);
    glBindTexture(GL_TEXTURE_2D, textureID);
}