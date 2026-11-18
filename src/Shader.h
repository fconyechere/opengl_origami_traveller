#ifndef DELIVERABLE2_SHADER_H
#define DELIVERABLE2_SHADER_H

#include "GraphicsMath.h"

#include <glad/gl.h>

#include <string>

class Shader {
public:
    Shader(const std::string& vertexPath, const std::string& fragmentPath);
    ~Shader();

    Shader(const Shader&) = delete;
    Shader& operator=(const Shader&) = delete;

    Shader(Shader&& other) noexcept;
    Shader& operator=(Shader&& other) noexcept;

    void use() const;
    void setMatrix4(const std::string& uniformName, const Mat4& matrix) const;
    void setVector3(const std::string& uniformName, const Vec3& vector) const;
    void setFloat(const std::string& uniformName, GLfloat value) const;
    void setInteger(const std::string& uniformName, GLint value) const;

private:
    GLuint program_;
};

#endif
