#ifndef DELIVERABLE2_MODEL_H
#define DELIVERABLE2_MODEL_H

#include "Mesh.h"

#include <string>
#include <vector>

class Model {
public:
    void loadFromFile(const std::string& path);
    void setPosition(const Vec3& position);
    void setRotation(const Vec3& rotationRadians);
    void setScale(const Vec3& scale);
    void draw(const class Shader& shader, const Mat4& viewProjectionMatrix) const;
    void drawWithTransform(
        const class Shader& shader,
        const Mat4& viewProjectionMatrix,
        const Mat4& transform
    ) const;

private:
    Mat4 modelMatrix() const;

    std::vector<Mesh> meshes_;
    Vec3 position_ = {0.0f, 0.0f, 0.0f};
    Vec3 rotationRadians_ = {0.0f, 0.0f, 0.0f};
    Vec3 scale_ = {1.0f, 1.0f, 1.0f};
};

#endif
