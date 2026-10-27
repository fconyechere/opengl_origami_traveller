#ifndef DELIVERABLE2_MODEL_H
#define DELIVERABLE2_MODEL_H

#include "Mesh.h"

#include <vector>

class Model {
public:
    void addMesh(const std::vector<GLfloat>& positions);
    void draw() const;

private:
    std::vector<Mesh> meshes_;
};

#endif
