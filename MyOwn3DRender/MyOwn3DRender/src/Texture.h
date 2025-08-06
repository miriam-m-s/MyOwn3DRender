#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include <glad/glad.h> // o <GL/glew.h> si usas GLEW


class Texture {
public:
    unsigned int ID;
    int width, height, nrChannels;
    GLenum format;
    bool flipVertically;
    const std::string path;
    Texture(const std::string& path, GLenum format = GL_RGB, bool flipVertically = true);

    void bind(GLenum textureUnit = GL_TEXTURE0) const;
};

#endif
