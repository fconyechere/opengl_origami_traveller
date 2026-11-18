#include "Bird.h"

#include "Mesh.h"
#include "Shader.h"
#include "Wind.h"

#include <cmath>
#include <vector>

namespace {

Vec3 faceNormal(const Vec3& first, const Vec3& second, const Vec3& third) {
    return glm::normalize(glm::cross(second - first, third - first));
}

void addTriangle(std::vector<GLfloat>& positions, std::vector<GLfloat>& normals, const Vec3& first, const Vec3& second, const Vec3& third) 
{
    const Vec3 normal = faceNormal(first, second, third);
    for (const Vec3& point : {first, second, third}) {
        positions.push_back(point.x);
        positions.push_back(point.y);
        positions.push_back(point.z);
        normals.push_back(normal.x);
        normals.push_back(normal.y);
        normals.push_back(normal.z);
    }
}

std::unique_ptr<Mesh> createMesh( const std::vector<Vec3>& triangles) {
    // build one folded-paper bird part from triangles.
    std::vector<GLfloat> positions;
    std::vector<GLfloat> normals;
    std::vector<GLfloat> textureCoordinates;

    for (std::size_t index = 0; index < triangles.size(); index += 3) {
        addTriangle(
            positions,
            normals,
            triangles[index],
            triangles[index + 1],
            triangles[index + 2]
        );
    }

    textureCoordinates.resize(positions.size() / 3 * 2, 0.0f);
    return std::unique_ptr<Mesh>(new Mesh(
        positions,
        normals,
        {0.92f, 0.92f, 0.92f},
        textureCoordinates,
        false
    ));
}

void createBirdMeshes(
    std::unique_ptr<Mesh>& bodyMesh,
    std::unique_ptr<Mesh>& leftWingMesh,
    std::unique_ptr<Mesh>& rightWingMesh
) {
    // keep the body and wings separate so the wings can flap.
    const Vec3 nose = {0.0f, 0.12f, 0.45f};
    const Vec3 chest = {0.0f, -0.10f, 0.05f};
    const Vec3 tail = {0.0f, 0.18f, -0.45f};
    const Vec3 leftWing = {-0.75f, 0.08f, -0.05f};
    const Vec3 rightWing = {0.75f, 0.08f, -0.05f};
    const Vec3 spine = {0.0f, 0.08f, 0.0f};
    bodyMesh = createMesh({nose, chest, spine, spine, chest, tail});
    leftWingMesh = createMesh({nose, leftWing, spine, spine, leftWing, tail});
    rightWingMesh = createMesh({spine, rightWing, nose, tail, rightWing, spine});
}

}

Bird::Bird(
    const PerchSpot& perchSpot,
    float flapOffset,
    float reactionDelay
)
    : position_(perchSpot.position),
      velocity_(0.0f, 0.0f, 0.0f),
      perchOrigin_(perchSpot.position),
      branchWeight_(perchSpot.branchWeight),
      yaw_(0.0f),
      flapOffset_(flapOffset),
      reactionDelay_(reactionDelay),
      state_(BirdState::Perched),
      animationTimeSeconds_(0.0f),
      bodyMesh_(nullptr),
      leftWingMesh_(nullptr),
      rightWingMesh_(nullptr) {
    createBirdMeshes(bodyMesh_, leftWingMesh_, rightWingMesh_);
}

Bird::~Bird() = default;

void Bird::update(float deltaSeconds, const Wind& wind) {
    animationTimeSeconds_ += deltaSeconds;
    const Vec3 localWind = wind.getWindAt(position_);
    const float windMagnitude = glm::length(localWind);

    if (state_ == BirdState::Perched) {
        // move perched birds slightly with their branch.
        position_ = perchOrigin_ + localWind * branchWeight_;

        if (windMagnitude >= 0.45f) {
            reactionDelay_ -= deltaSeconds;
            if (reactionDelay_ <= 0.0f) {
                state_ = BirdState::Flying;
                velocity_ = {
                    0.0f,
                    1.6f,
                    0.0f
                };
                velocity_ += localWind * 3.0f;
            }
        }
        return;
    }

    // steer flying birds upward and along the wind.
    const Vec3 targetVelocity =
        localWind * 3.0f + Vec3(0.0f, 1.4f, 0.0f);
    velocity_ = glm::mix(velocity_, targetVelocity, deltaSeconds * 1.5f);
    position_ += velocity_ * deltaSeconds;

    if (glm::length(velocity_) > 0.01f) {
        yaw_ = std::atan2(velocity_.x, velocity_.z);
    }
}

void Bird::reset(
    const PerchSpot& perchSpot,
    float flapOffset,
    float reactionDelay
) {
    position_ = perchSpot.position;
    perchOrigin_ = perchSpot.position;
    branchWeight_ = perchSpot.branchWeight;
    velocity_ = {0.0f, 0.0f, 0.0f};
    yaw_ = 0.0f;
    flapOffset_ = flapOffset;
    reactionDelay_ = reactionDelay;
    state_ = BirdState::Perched;
}

void Bird::draw(
    const Shader& shader,
    const Mat4& viewProjectionMatrix
) const {
    // place the bird at its simulated position and flight heading.
    const Mat4 rootMatrix =
        translation(position_) * rotationY(yaw_) * scale({0.08f, 0.08f, 0.08f});
    const float flapAngle =
        std::sin(animationTimeSeconds_ * 12.0f + flapOffset_) * 0.65f;

    const Mat4 leftWingTransform = rootMatrix *
        translation({-0.05f, 0.08f, 0.0f}) *
        rotationZ(flapAngle) *
        translation({0.05f, -0.08f, 0.0f});
    const Mat4 rightWingTransform = rootMatrix *
        translation({0.05f, 0.08f, 0.0f}) *
        rotationZ(-flapAngle) *
        translation({-0.05f, -0.08f, 0.0f});

    shader.setMatrix4("modelMatrix", rootMatrix);
    shader.setMatrix4(
        "combinedTransformationMatrix",
        viewProjectionMatrix * rootMatrix
    );
    bodyMesh_->draw(shader);
    shader.setMatrix4("modelMatrix", leftWingTransform);
    shader.setMatrix4( "combinedTransformationMatrix", viewProjectionMatrix * leftWingTransform);
    leftWingMesh_->draw(shader);
    shader.setMatrix4("modelMatrix", rightWingTransform);
    shader.setMatrix4("combinedTransformationMatrix", viewProjectionMatrix * rightWingTransform);
    rightWingMesh_->draw(shader);
}

bool Bird::isOutOfScene() const {
    return position_.y > 1.4f || std::abs(position_.x) > 1.6f || std::abs(position_.z) > 1.6f;
}

const Vec3& Bird::position() const {
    return position_;
}

float Bird::flapOffset() const {
    return flapOffset_;
}

BirdState Bird::state() const {
    return state_;
}
