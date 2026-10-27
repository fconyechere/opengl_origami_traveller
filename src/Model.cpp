#include "Model.h"

void Model::addMesh(const std::vector<GLfloat>& positions) {
    meshes_.emplace_back(positions);
}

void Model::draw() const {
    for (const Mesh& mesh : meshes_) {
        mesh.draw();
    }
}
