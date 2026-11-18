#ifndef DELIVERABLE2_TRAVELER_H
#define DELIVERABLE2_TRAVELER_H

#include "GraphicsMath.h"

#include <memory>
#include <string>

class Model;
class Shader;

struct TravelerAssetPaths {
    std::string body;
    std::string head;
    std::string leftArm;
    std::string rightArm;
    std::string leftLeg;
    std::string rightLeg;
};

class Traveler {
public:
    void load(const TravelerAssetPaths& assetPaths);
    void update(float deltaSeconds);
    void draw(const Shader& shader, const Mat4& viewProjectionMatrix) const;
    bool isLoaded() const;

    void setPosition(const Vec3& position);
    void setRotation(const Vec3& rotationRadians);
    void setScale(const Vec3& scale);

private:
    Mat4 rootMatrix() const;
    void drawPart(
        const Model& part,
        const Shader& shader,
        const Mat4& viewProjectionMatrix,
        const Mat4& partMatrix
    ) const;

    std::unique_ptr<Model> body_;
    std::unique_ptr<Model> head_;
    std::unique_ptr<Model> leftArm_;
    std::unique_ptr<Model> rightArm_;
    std::unique_ptr<Model> leftLeg_;
    std::unique_ptr<Model> rightLeg_;

    Vec3 position_ = {0.0f, -0.23f, 0.0f};
    Vec3 rotationRadians_ = {0.0f, 0.0f, 0.0f};
    Vec3 scale_ = {0.15f, 0.15f, 0.15f};
    float animationTimeSeconds_ = 0.0f;
};

#endif
