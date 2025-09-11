#include "texture.h"

unsigned int Texture::generateTexture(unsigned int count, unsigned int& texture)
{
    glGenTextures(count, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    return texture;
}

unsigned char* Texture::loadTextureImage(const char* filename, int& width, int& height, int& numberOfChannels)
{
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load("textures/sun.jpg", &width, &height, &numberOfChannels, 0);
    return data;
}

void Texture::freeImageData(unsigned char* data)
{
    stbi_image_free(data);
}

void Texture::specifyTextureImage2D(unsigned char* data, unsigned int width, unsigned int height, bool generateMipmap = true)
{
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    if (generateMipmap)
    {
        glGenerateMipmap(GL_TEXTURE_2D);
    }
}