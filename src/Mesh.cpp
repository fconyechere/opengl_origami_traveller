#include "Mesh.h"
#include "Shader.h"

#include <cstddef>
#include <limits>
#include <stdexcept>

Mesh::Mesh(const std::vector<GLfloat>& positions, const std::vector<GLfloat>& normals, const Vec3& materialColor, const std::vector<GLfloat>& textureCoordinates, bool usesTexture)
    : vertexArray_(0),
      vertexBuffer_(0),
      normalBuffer_(0),
      textureCoordinateBuffer_(0),
      vertexCount_(0),
      materialColor_(materialColor),
      usesTexture_(usesTexture) {
    if (positions.empty() || positions.size() % 9 != 0) {
        throw std::invalid_argument(
            "A triangle mesh must contain a non-empty multiple of three 3D vertices"
        );
    }
    if (positions.size() != normals.size() || normals.size() % 3 != 0) {
        throw std::invalid_argument(
            "Positions and normals must contain the same number of 3D vertices"
        );
    }
    if (textureCoordinates.size() != positions.size() / 3 * 2) {
        throw std::invalid_argument(
            "Texture coordinates must contain two values for every vertex"
        );
    }
    if (positions.size() / 3 > static_cast<std::size_t>(std::numeric_limits<GLsizei>::max())) {
        throw std::length_error("Mesh contains too many vertices");
    }
    const std::size_t maximumBufferElements =
        static_cast<std::size_t>(
            std::numeric_limits<GLsizeiptr>::max() / sizeof(GLfloat));
    if (positions.size() > maximumBufferElements ||
        normals.size() > maximumBufferElements) {
        throw std::length_error("Mesh vertex data is too large for OpenGL");
    }

    vertexCount_ = static_cast<GLsizei>(positions.size() / 3);
    glGenVertexArrays(1, &vertexArray_);
    glGenBuffers(1, &vertexBuffer_);
    glGenBuffers(1, &normalBuffer_);
    glGenBuffers(1, &textureCoordinateBuffer_);
    if (vertexArray_ == 0 || vertexBuffer_ == 0 || normalBuffer_ == 0 ||
        textureCoordinateBuffer_ == 0) {
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

    glBindBuffer(GL_ARRAY_BUFFER, normalBuffer_);
    glBufferData(
        GL_ARRAY_BUFFER,
        static_cast<GLsizeiptr>(normals.size() * sizeof(GLfloat)),
        normals.data(),
        GL_STATIC_DRAW
    );
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), nullptr);
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, textureCoordinateBuffer_);
    glBufferData(
        GL_ARRAY_BUFFER,
        static_cast<GLsizeiptr>(textureCoordinates.size() * sizeof(GLfloat)),
        textureCoordinates.data(),
        GL_STATIC_DRAW
    );
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), nullptr);
    glEnableVertexAttribArray(2);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

Mesh::~Mesh() {
    release();
}

Mesh::Mesh(Mesh&& other) noexcept
    : vertexArray_(other.vertexArray_),
      vertexBuffer_(other.vertexBuffer_),
      normalBuffer_(other.normalBuffer_),
      textureCoordinateBuffer_(other.textureCoordinateBuffer_),
      vertexCount_(other.vertexCount_),
      materialColor_(other.materialColor_),
      usesTexture_(other.usesTexture_) {
    other.vertexArray_ = 0;
    other.vertexBuffer_ = 0;
    other.normalBuffer_ = 0;
    other.textureCoordinateBuffer_ = 0;
    other.vertexCount_ = 0;
}

Mesh& Mesh::operator=(Mesh&& other) noexcept {
    if (this != &other) {
        release();
        vertexArray_ = other.vertexArray_;
        vertexBuffer_ = other.vertexBuffer_;
        normalBuffer_ = other.normalBuffer_;
        textureCoordinateBuffer_ = other.textureCoordinateBuffer_;
        vertexCount_ = other.vertexCount_;
        materialColor_ = other.materialColor_;
        usesTexture_ = other.usesTexture_;
        other.vertexArray_ = 0;
        other.vertexBuffer_ = 0;
        other.normalBuffer_ = 0;
        other.textureCoordinateBuffer_ = 0;
        other.vertexCount_ = 0;
    }
    return *this;
}

void Mesh::draw(const Shader& shader) const {
    shader.setVector3("materialColor", materialColor_);
    shader.setInteger("usesTexture", usesTexture_ ? 1 : 0);
    glBindVertexArray(vertexArray_);
    glDrawArrays(GL_TRIANGLES, 0, vertexCount_);
}

void Mesh::release() {
    if (vertexBuffer_ != 0) {
        glDeleteBuffers(1, &vertexBuffer_);
        vertexBuffer_ = 0;
    }
    if (normalBuffer_ != 0) {
        glDeleteBuffers(1, &normalBuffer_);
        normalBuffer_ = 0;
    }
    if (textureCoordinateBuffer_ != 0) {
        glDeleteBuffers(1, &textureCoordinateBuffer_);
        textureCoordinateBuffer_ = 0;
    }
    if (vertexArray_ != 0) {
        glDeleteVertexArrays(1, &vertexArray_);
        vertexArray_ = 0;
    }
}
