#include "Texture.h"
#include "stb_image.h"
#include <iostream>

Texture::Texture(const std::string& path2, GLenum format, bool flipVertically) :path(path2), ID(0), format(GL_RGB), flipVertically(true), width(0), height(0), nrChannels(0)
{
    glGenTextures(1, &ID);
    glBindTexture(GL_TEXTURE_2D, ID);


    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    stbi_set_flip_vertically_on_load(flipVertically);

    unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrChannels, 0);
    if (!data) {
        throw std::runtime_error("Failed to load texture: " + path);
    }

    GLenum internalFormat;
    GLenum dataFormat;

    if (nrChannels == 1) {
        internalFormat = dataFormat = GL_RED;
    }
    else if (nrChannels == 3) {
        internalFormat = dataFormat = GL_RGB;
    }
    else if (nrChannels == 4) {
        internalFormat = dataFormat = GL_RGBA;
    }
    else {
        throw std::runtime_error("Unsupported texture format");
    }


    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, dataFormat, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);

    stbi_image_free(data);
    
}

Texture::Texture():format(GL_RGB), flipVertically(true)
{
}


void Texture::bind(GLenum textureUnit) const
{
    glActiveTexture(textureUnit);
    glBindTexture(GL_TEXTURE_2D, ID);
}
