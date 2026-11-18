#include "Shader.h"

#include <glm/gtc/type_ptr.hpp>

#include <fstream>
#include <sstream>
#include <stdexcept>
#include <utility>

namespace {

std::string readShaderFile(const std::string& path) {
    // keep shader source outside the c++ code.
    std::ifstream file(path.c_str());
    if (!file) {
        throw std::runtime_error("could not open shader file: " + path);
    }

    std::ostringstream contents;
    contents << file.rdbuf();
    if (file.bad()) {
        throw std::runtime_error("could not read shader file: " + path);
    }
    return contents.str();
}

GLuint compileShader(
    GLenum type,
    const std::string& source,
    const std::string& path
) {
    // compile one shader and report its log on failure.
    const GLuint shader = glCreateShader(type);
    if (shader == 0) {
        throw std::runtime_error("could not create shader: " + path);
    }

    const char* sourceText = source.c_str();
    glShaderSource(shader, 1, &sourceText, nullptr);
    glCompileShader(shader);

    GLint compiled = GL_FALSE;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
    if (compiled == GL_TRUE) {
        return shader;
    }

    char log[1024] = {};
    glGetShaderInfoLog(shader, sizeof(log), nullptr, log);
    glDeleteShader(shader);
    throw std::runtime_error("shader compilation failed (" + path + "): " + log);
}

} // namespace

Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath)
    : program_(0) {
    // compile both stages before linking the program.
    const GLuint vertexShader = compileShader(
        GL_VERTEX_SHADER,
        readShaderFile(vertexPath),
        vertexPath
    );

    GLuint fragmentShader = 0;
    try {
        fragmentShader = compileShader(GL_FRAGMENT_SHADER, readShaderFile(fragmentPath), fragmentPath);

        program_ = glCreateProgram();
        if (program_ == 0) {
            throw std::runtime_error("could not create shader program");
        }

        glAttachShader(program_, vertexShader);
        glAttachShader(program_, fragmentShader);
        glLinkProgram(program_);

        GLint linked = GL_FALSE;
        glGetProgramiv(program_, GL_LINK_STATUS, &linked);
        if (linked != GL_TRUE) {
            char log[1024] = {};
            glGetProgramInfoLog(program_, sizeof(log), nullptr, log);
            throw std::runtime_error(
                std::string("shader program linking failed: ") + log
            );
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
    // use this program for following draw calls.
    glUseProgram(program_);
}

void Shader::setMatrix4(const std::string& uniformName, const Mat4& matrix) const {
    // send a matrix uniform to the gpu.
    const GLint uniformLocation = glGetUniformLocation(program_, uniformName.c_str());
    if (uniformLocation == -1) {
        throw std::runtime_error("shader uniform not found: " + uniformName);
    }

    glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(matrix));
}

void Shader::setVector3(const std::string& uniformName, const Vec3& vector) const {
    // send a vector uniform to the gpu.
    const GLint uniformLocation = glGetUniformLocation(program_, uniformName.c_str());
    if (uniformLocation == -1) {
        throw std::runtime_error("shader uniform not found: " + uniformName);
    }

    glUniform3f(uniformLocation, vector.x, vector.y, vector.z);
}

void Shader::setFloat(const std::string& uniformName, GLfloat value) const {
    // send a float uniform to the gpu.
    const GLint uniformLocation = glGetUniformLocation(program_, uniformName.c_str());
    if (uniformLocation == -1) {
        throw std::runtime_error("shader uniform not found: " + uniformName);
    }

    glUniform1f(uniformLocation, value);
}

void Shader::setInteger(const std::string& uniformName, GLint value) const {
    // send an integer uniform to the gpu.
    const GLint uniformLocation = glGetUniformLocation(program_, uniformName.c_str());
    if (uniformLocation == -1) {
        throw std::runtime_error("shader uniform not found: " + uniformName);
    }

    glUniform1i(uniformLocation, value);
}
