#ifndef DELIVERABLE2_MODEL_H
#define DELIVERABLE2_MODEL_H

#include "Mesh.h"

#include <string>
#include <array>
#include <vector>

enum class CharacterPart {
    Face,
    Body,
    LeftArm,
    RightArm,
    LeftHand,
    RightHand,
    LeftLeg,
    RightLeg,
    LeftCalf,
    RightCalf,
    LeftFoot,
    RightFoot,
    Count
};

class Model {
public:
    void loadFromFile(const std::string& path);
    void loadCharacterFromFile(const std::string& path);
    void draw(const class Shader& shader) const;
    void draw(const class Shader& shader, const Mat4& modelMatrix) const;
    void drawCharacter(const class Shader& shader, const Mat4& viewProjectionMatrix, const std::array<Mat4, static_cast<std::size_t>(CharacterPart::Count)>& partModelMatrices) const;

private:
    std::vector<Mesh> meshes_;
    std::vector<Mesh> armMeshes_;
    std::array<std::vector<Mesh>, static_cast<std::size_t>(CharacterPart::Count)>
        characterPartMeshes_;
};

#endif
