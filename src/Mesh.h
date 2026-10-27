#ifndef DELIVERABLE2_MESH_H
#define DELIVERABLE2_MESH_H

#include <glad/gl.h>

#include <vector>

class Mesh {
public:
    explicit Mesh(const std::vector<GLfloat>& positions);
    ~Mesh();

    Mesh(const Mesh&) = delete;
    Mesh& operator=(const Mesh&) = delete;

    Mesh(Mesh&& other) noexcept;
    Mesh& operator=(Mesh&& other) noexcept;

    void draw() const;

private:
    void release();

    GLuint vertexArray_;
    GLuint vertexBuffer_;
    GLsizei vertexCount_;
};

#endif
