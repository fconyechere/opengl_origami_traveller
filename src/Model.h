#ifndef DELIVERABLE2_MODEL_H
#define DELIVERABLE2_MODEL_H

#include "Mesh.h"

#include <string>
#include <vector>

class Model {
public:
    void loadFromFile(const std::string& path);
    void draw(const class Shader& shader) const;

private:
    std::vector<Mesh> meshes_;
};

#endif
