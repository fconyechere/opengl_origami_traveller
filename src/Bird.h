#ifndef DELIVERABLE2_BIRD_H
#define DELIVERABLE2_BIRD_H

#include "GraphicsMath.h"
#include "ProceduralTree.h"

#include <memory>

class Wind;
class Mesh;
class Shader;

enum class BirdState {
    Perched,
    Flying
};

class Bird {
public:
    Bird(const PerchSpot& perchSpot, float flapOffset, float reactionDelay);
    ~Bird();

    Bird(const Bird&) = delete;
    Bird& operator=(const Bird&) = delete;
    Bird(Bird&&) noexcept = default;
    Bird& operator=(Bird&&) noexcept = default;

    void update(float deltaSeconds, const Wind& wind);
    void reset(const PerchSpot& perchSpot, float flapOffset, float reactionDelay);
    void draw(const Shader& shader, const Mat4& viewProjectionMatrix) const;
    bool isOutOfScene() const;
    const Vec3& position() const;
    float flapOffset() const;
    BirdState state() const;

private:
    Vec3 position_;
    Vec3 velocity_;
    Vec3 perchOrigin_;
    float branchWeight_;
    float yaw_;
    float flapOffset_;
    float reactionDelay_;
    BirdState state_;
    float animationTimeSeconds_;
    std::unique_ptr<Mesh> bodyMesh_;
    std::unique_ptr<Mesh> leftWingMesh_;
    std::unique_ptr<Mesh> rightWingMesh_;
};

#endif
