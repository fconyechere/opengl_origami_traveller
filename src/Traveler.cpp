#include "Traveler.h"

#include "Model.h"
#include "Shader.h"

#include <cmath>
#include <memory>

void Traveler::load(const TravelerAssetPaths& assetPaths) {
    // load each part once so animation only changes transforms.
    body_.reset(new Model());
    head_.reset(new Model());
    leftArm_.reset(new Model());
    rightArm_.reset(new Model());
    leftLeg_.reset(new Model());
    rightLeg_.reset(new Model());

    body_->loadFromFile(assetPaths.body);
    head_->loadFromFile(assetPaths.head);
    leftArm_->loadFromFile(assetPaths.leftArm);
    rightArm_->loadFromFile(assetPaths.rightArm);
    leftLeg_->loadFromFile(assetPaths.leftLeg);
    rightLeg_->loadFromFile(assetPaths.rightLeg);
}

void Traveler::update(float deltaSeconds) {
    // keep animation time independent from the frame rate.
    animationTimeSeconds_ += deltaSeconds;
}

void Traveler::draw(
    const Shader& shader,
    const Mat4& viewProjectionMatrix
) const {
    if (!isLoaded()) {
        return;
    }

    // rotate limbs around their joints after applying the traveler root.
    const float swing =
        std::sin(animationTimeSeconds_ * 3.0f) * 0.55f;
    const Mat4 root = rootMatrix();

    const Mat4 leftShoulder = multiply(
        root,
        multiply(
            translation({-0.18f, 0.62f, 0.0f}),
            multiply(
                rotationZ(-swing),
                translation({0.18f, -0.62f, 0.0f})
            )
        )
    );
    const Mat4 rightShoulder = multiply(
        root,
        multiply(
            translation({0.18f, 0.62f, 0.0f}),
            multiply(
                rotationZ(swing),
                translation({-0.18f, -0.62f, 0.0f})
            )
        )
    );
    const Mat4 leftHip = multiply(
        root,
        multiply(
            translation({-0.10f, 0.32f, 0.0f}),
            multiply(
                rotationZ(-swing),
                translation({0.10f, -0.32f, 0.0f})
            )
        )
    );
    const Mat4 rightHip = multiply(
        root,
        multiply(
            translation({0.10f, 0.32f, 0.0f}),
            multiply(
                rotationZ(swing),
                translation({-0.10f, -0.32f, 0.0f})
            )
        )
    );

    drawPart(*body_, shader, viewProjectionMatrix, root);
    drawPart(
        *head_,
        shader,
        viewProjectionMatrix,
        multiply(root, translation({0.0f, 0.72f, 0.0f}))
    );
    drawPart(*leftArm_, shader, viewProjectionMatrix, leftShoulder);
    drawPart(*rightArm_, shader, viewProjectionMatrix, rightShoulder);
    drawPart(*leftLeg_, shader, viewProjectionMatrix, leftHip);
    drawPart(*rightLeg_, shader, viewProjectionMatrix, rightHip);
}

bool Traveler::isLoaded() const {
    return body_ != nullptr &&
        head_ != nullptr &&
        leftArm_ != nullptr &&
        rightArm_ != nullptr &&
        leftLeg_ != nullptr &&
        rightLeg_ != nullptr;
}

void Traveler::setPosition(const Vec3& position) {
    position_ = position;
}

void Traveler::setRotation(const Vec3& rotationRadians) {
    rotationRadians_ = rotationRadians;
}

void Traveler::setScale(const Vec3& scale) {
    scale_ = scale;
}

Mat4 Traveler::rootMatrix() const {
    // build the shared local-to-world transform for every body part.
    Mat4 transform = translation(position_);
    transform = multiply(transform, rotationY(rotationRadians_.y));
    transform = multiply(transform, rotationX(rotationRadians_.x));
    transform = multiply(transform, rotationZ(rotationRadians_.z));
    return multiply(transform, scale(scale_));
}

void Traveler::drawPart(
    const Model& part,
    const Shader& shader,
    const Mat4& viewProjectionMatrix,
    const Mat4& partMatrix
) const {
    // each part receives its final world transform.
    part.drawWithTransform(shader, viewProjectionMatrix, partMatrix);
}
