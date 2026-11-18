#ifndef DELIVERABLE2_GRAPHICS_MATH_H
#define DELIVERABLE2_GRAPHICS_MATH_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

using Mat4 = glm::mat4;
using Vec4 = glm::vec4;
using Vec3 = glm::vec3;

inline Vec3 subtract(const Vec3& left, const Vec3& right) {
    return left - right;
}

inline float dot(const Vec3& left, const Vec3& right) {
    return glm::dot(left, right);
}

inline Vec3 cross(const Vec3& left, const Vec3& right) {
    return glm::cross(left, right);
}

inline Vec3 normalize(const Vec3& vector) {
    return glm::normalize(vector);
}

inline Mat4 identityMatrix() {
    return glm::mat4(1.0f);
}

inline Mat4 multiply(const Mat4& left, const Mat4& right) {
    return left * right;
}

inline Vec4 multiply(const Mat4& matrix, const Vec4& vector) {
    return matrix * vector;
}

inline Mat4 rotationY(float angleRadians) {
    return glm::rotate(identityMatrix(), angleRadians, Vec3(0.0f, 1.0f, 0.0f));
}

inline Mat4 rotationX(float angleRadians) {
    return glm::rotate(identityMatrix(), angleRadians, Vec3(1.0f, 0.0f, 0.0f));
}

inline Mat4 rotationZ(float angleRadians) {
    return glm::rotate(identityMatrix(), angleRadians, Vec3(0.0f, 0.0f, 1.0f));
}

inline Mat4 translation(const Vec3& position) {
    return glm::translate(identityMatrix(), position);
}

inline Mat4 scale(const Vec3& scaleValues) {
    return glm::scale(identityMatrix(), scaleValues);
}

inline Mat4 lookAt(const Vec3& eye, const Vec3& target, const Vec3& worldUp) {
    return glm::lookAt(eye, target, worldUp);
}

inline Mat4 perspective(
    float verticalFieldOfViewRadians,
    float aspectRatio,
    float nearPlane,
    float farPlane
) {
    return glm::perspective(
        verticalFieldOfViewRadians,
        aspectRatio,
        nearPlane,
        farPlane
    );
}

#endif
