#ifndef DELIVERABLE2_MESH_H
#define DELIVERABLE2_MESH_H

#include "GraphicsMath.h"

#include <glad/gl.h>

#include <vector>

class Mesh {
public:
    explicit Mesh(
        const std::vector<GLfloat>& positions,
        const std::vector<GLfloat>& normals,
        const Vec3& materialColor,
        const std::vector<GLfloat>& textureCoordinates,
        bool usesTexture
    );
    ~Mesh();

    Mesh(const Mesh&) = delete;
    Mesh& operator=(const Mesh&) = delete;

    Mesh(Mesh&& other) noexcept;
    Mesh& operator=(Mesh&& other) noexcept;

    void draw(const class Shader& shader) const;

private:
    void release();

    GLuint vertexArray_;
    GLuint vertexBuffer_;
    GLuint normalBuffer_;
    GLuint textureCoordinateBuffer_;
    GLsizei vertexCount_;
    Vec3 materialColor_;
    bool usesTexture_;
};

#endif
