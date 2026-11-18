#ifndef DELIVERABLE2_LANDSCAPE_H
#define DELIVERABLE2_LANDSCAPE_H

#include "Mesh.h"
#include "Bird.h"
#include "Wind.h"

#include <memory>
#include <random>
#include <vector>

class Landscape {
public:
    Landscape();

    void update(float deltaSeconds, const Wind& wind);
    void draw(
        const class Shader& shader,
        const Mat4& viewProjectionMatrix
    ) const;

private:
    std::unique_ptr<Mesh> treeMesh_;
    std::unique_ptr<Mesh> grassMesh_;
    std::unique_ptr<Mesh> dirtMesh_;
    std::vector<ProceduralTree> trees_;
    std::vector<PerchSpot> perchSpots_;
    std::vector<Bird> birds_;
    std::mt19937 birdGenerator_;
};

#endif