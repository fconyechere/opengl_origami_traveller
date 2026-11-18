#ifndef DELIVERABLE2_TEXTURE_H
#define DELIVERABLE2_TEXTURE_H

#include <glad/gl.h>

#include <string>

class Texture {
public:
    Texture();
    ~Texture();

    Texture(const Texture&) = delete;
    Texture& operator=(const Texture&) = delete;

    void loadFromFile(const std::string& path);
    void bind(GLuint textureUnit) const;

private:
    GLuint texture_;
};

#endif
