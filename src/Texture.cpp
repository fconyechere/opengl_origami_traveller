#include "Texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <stdexcept>

Texture::Texture()
    : texture_(0) {
}

Texture::~Texture() {
    if (texture_ != 0) {
        glDeleteTextures(1, &texture_);
    }
}

void Texture::loadFromFile(const std::string& path) {
    // load image pixels and upload them to the gpu.
    int width = 0;
    int height = 0;
    int channels = 0;
    stbi_set_flip_vertically_on_load(1);
    unsigned char* image = stbi_load(
        path.c_str(),
        &width,
        &height,
        &channels,
        0
    );

    if (image == nullptr) {
        throw std::runtime_error("could not load texture: " + path);
    }

    GLenum format = GL_RGB;
    if (channels == 4) {
        format = GL_RGBA;
    } else if (channels != 3) {
        stbi_image_free(image);
        throw std::runtime_error("texture must contain rgb or rgba pixels");
    }

    glGenTextures(1, &texture_);
    if (texture_ == 0) {
        stbi_image_free(image);
        throw std::runtime_error("could not create texture");
    }

    glBindTexture(GL_TEXTURE_2D, texture_);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D,0,format,width,height,0,format,GL_UNSIGNED_BYTE,image);
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(image);
}
s
void Texture::bind(GLuint textureUnit) const {
    glActiveTexture(GL_TEXTURE0 + textureUnit);
    glBindTexture(GL_TEXTURE_2D, texture_);
}
