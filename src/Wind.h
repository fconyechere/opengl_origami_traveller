#ifndef DELIVERABLE2_WIND_H
#define DELIVERABLE2_WIND_H

#include "GraphicsMath.h"

class Shader;

class Wind {
public:
    Wind(float baseSpeed, float directionDegrees);

    void setDirection(float directionDegrees);
    void update(float deltaSeconds);
    Vec3 getWindAt(const Vec3& position) const;
    void applyToShader(const Shader& shader) const;

private:
    float baseSpeed_;
    Vec3 direction_;
    float time_;
};

#endif
