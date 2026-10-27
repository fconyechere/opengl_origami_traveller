#include "Mesh.h"

#include <cstddef>
#include <limits>
#include <stdexcept>

Mesh::Mesh(const std::vector<GLfloat>& positions)
    : vertexArray_(0),
      vertexBuffer_(0),
      vertexCount_(0) {
    if (positions.empty() || positions.size() % 9 != 0) {
        throw std::invalid_argument(
            "A triangle mesh must contain a non-empty multiple of three 3D vertices"
        );
    }
    if (positions.size() / 3 > static_cast<std::size_t>(std::numeric_limits<GLsizei>::max())) {
        throw std::length_error("Mesh contains too many vertices");
    }
    if (positions.size() > static_cast<std::size_t>(
            std::numeric_limits<GLsizeiptr>::max() / sizeof(GLfloat))) {
        throw std::length_error("Mesh vertex data is too large for OpenGL");
    }

    vertexCount_ = static_cast<GLsizei>(positions.size() / 3);
    glGenVertexArrays(1, &vertexArray_);
    glGenBuffers(1, &vertexBuffer_);
    if (vertexArray_ == 0 || vertexBuffer_ == 0) {
        release();
        throw std::runtime_error("Could not allocate OpenGL mesh buffers");
    }

    glBindVertexArray(vertexArray_);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer_);
    glBufferData(
        GL_ARRAY_BUFFER,
        static_cast<GLsizeiptr>(positions.size() * sizeof(GLfloat)),
        positions.data(),
        GL_STATIC_DRAW
    );
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), nullptr);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

Mesh::~Mesh() {
    release();
}

Mesh::Mesh(Mesh&& other) noexcept
    : vertexArray_(other.vertexArray_),
      vertexBuffer_(other.vertexBuffer_),
      vertexCount_(other.vertexCount_) {
    other.vertexArray_ = 0;
    other.vertexBuffer_ = 0;
    other.vertexCount_ = 0;
}

Mesh& Mesh::operator=(Mesh&& other) noexcept {
    if (this != &other) {
        release();
        vertexArray_ = other.vertexArray_;
        vertexBuffer_ = other.vertexBuffer_;
        vertexCount_ = other.vertexCount_;
        other.vertexArray_ = 0;
        other.vertexBuffer_ = 0;
        other.vertexCount_ = 0;
    }
    return *this;
}

void Mesh::draw() const {
    glBindVertexArray(vertexArray_);
    glDrawArrays(GL_TRIANGLES, 0, vertexCount_);
}

void Mesh::release() {
    if (vertexBuffer_ != 0) {
        glDeleteBuffers(1, &vertexBuffer_);
        vertexBuffer_ = 0;
    }
    if (vertexArray_ != 0) {
        glDeleteVertexArrays(1, &vertexArray_);
        vertexArray_ = 0;
    }
}
