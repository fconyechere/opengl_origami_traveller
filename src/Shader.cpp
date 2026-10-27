#include "Shader.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <utility>

namespace {

std::string readShaderFile(const std::string& path) {
    std::ifstream file(path.c_str());
    if (!file) {
        throw std::runtime_error("Could not open shader file: " + path);
    }

    std::ostringstream contents;
    contents << file.rdbuf();
    if (file.bad()) {
        throw std::runtime_error("Could not read shader file: " + path);
    }
    return contents.str();
}

GLuint compileShader(GLenum type, const std::string& source, const std::string& path) {
    const GLuint shader = glCreateShader(type);
    if (shader == 0) {
        throw std::runtime_error("Could not create shader: " + path);
    }

    const char* sourceText = source.c_str();
    glShaderSource(shader, 1, &sourceText, nullptr);
    glCompileShader(shader);

    GLint success = GL_FALSE;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (success == GL_TRUE) {
        return shader;
    }

    char log[1024] = {};
    glGetShaderInfoLog(shader, sizeof(log), nullptr, log);
    glDeleteShader(shader);
    throw std::runtime_error("Shader compilation failed (" + path + "): " + log);
}

} // namespace

Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath)
    : program_(0) {
    const GLuint vertexShader = compileShader(
        GL_VERTEX_SHADER,
        readShaderFile(vertexPath),
        vertexPath
    );

    GLuint fragmentShader = 0;
    try {
        fragmentShader = compileShader(
            GL_FRAGMENT_SHADER,
            readShaderFile(fragmentPath),
            fragmentPath
        );

        program_ = glCreateProgram();
        if (program_ == 0) {
            throw std::runtime_error("Could not create shader program");
        }

        glAttachShader(program_, vertexShader);
        glAttachShader(program_, fragmentShader);
        glLinkProgram(program_);

        GLint success = GL_FALSE;
        glGetProgramiv(program_, GL_LINK_STATUS, &success);
        if (success != GL_TRUE) {
            char log[1024] = {};
            glGetProgramInfoLog(program_, sizeof(log), nullptr, log);
            throw std::runtime_error(std::string("Shader program linking failed: ") + log);
        }
    } catch (...) {
        if (program_ != 0) {
            glDeleteProgram(program_);
            program_ = 0;
        }
        if (fragmentShader != 0) {
            glDeleteShader(fragmentShader);
        }
        glDeleteShader(vertexShader);
        throw;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

Shader::~Shader() {
    if (program_ != 0) {
        glDeleteProgram(program_);
    }
}

Shader::Shader(Shader&& other) noexcept
    : program_(other.program_) {
    other.program_ = 0;
}

Shader& Shader::operator=(Shader&& other) noexcept {
    if (this != &other) {
        if (program_ != 0) {
            glDeleteProgram(program_);
        }
        program_ = other.program_;
        other.program_ = 0;
    }
    return *this;
}

void Shader::use() const {
    glUseProgram(program_);
}
