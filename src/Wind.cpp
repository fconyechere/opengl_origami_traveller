#include "Wind.h"
#include "Shader.h"

#include <cmath>
#include <algorithm>

Wind::Wind(float baseSpeed, float directionDegrees)
    : baseSpeed_(baseSpeed),
      direction_(0.0f, 0.0f, 0.0f),
      time_(0.0f) {
    setDirection(directionDegrees);
}

void Wind::setDirection(float directionDegrees) {
    // convert the direction into a horizontal unit vector.
    const float directionRadians =
        directionDegrees * 3.14159265f / 180.0f;
    direction_ = {
        std::cos(directionRadians),
        0.0f,
        std::sin(directionRadians)
    };
}

void Wind::update(float deltaSeconds) {
    // advance the gust wave once per frame.
    time_ += deltaSeconds;
}

Vec3 Wind::getWindAt(const Vec3& position) const {
    // vary wind strength across the world to create gusts.
    const float wave = std::sin(
        time_ * 0.12f + position.x * 0.3f + position.z * 0.3f
    );
    const float gust = std::max(0.0f, wave * 0.5f + 0.5f);
    return direction_ * (baseSpeed_ + gust * 0.5f);
}

void Wind::applyToShader(const Shader& shader) const {
    // send wind values to shaders that support vertex movement.
    shader.setVector3("windDirection", direction_);
    shader.setFloat("windSpeed", baseSpeed_);
    shader.setFloat("windTime", time_);
}


