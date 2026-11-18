#include "Camera.h"

#include <GLFW/glfw3.h>

#include <cmath>

namespace {

float clampValue(float value, float minimum, float maximum) {
    return value < minimum ? minimum : value > maximum ? maximum : value;
}

}

Camera::Camera()
    : target_(0.0f, -0.23f, 0.0f),
      horizontalOrbitAngle_(0.6f),
      orbitElevationRadians_(0.55f),
      orbitRadius_(1.8f),
      isOrbitDragging_(false),
      lastCursorX_(0.0),
      lastCursorY_(0.0) {
}

void Camera::attach(GLFWwindow* window) {
    glfwSetWindowUserPointer(window, this);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwSetCursorPosCallback(window, cursorPositionCallback);
}

void Camera::update(GLFWwindow* window, float deltaSeconds) {
    const float orbitSpeed = deltaSeconds;

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        horizontalOrbitAngle_ += orbitSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        horizontalOrbitAngle_ -= orbitSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        orbitElevationRadians_ += orbitSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        orbitElevationRadians_ -= orbitSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        orbitRadius_ += orbitSpeed;
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        orbitRadius_ -= orbitSpeed;
    }

    clampOrbit();
}

Mat4 Camera::viewProjection(float aspectRatio) const {
    // combine the camera view and perspective transforms.
    const Mat4 view = lookAt(
        orbitPosition(),
        target_,
        Vec3(0.0f, 1.0f, 0.0f)
    );
    const Mat4 projection = perspective(
        60.0f * 3.14159265359f / 180.0f,
        aspectRatio,
        0.1f,
        100.0f
    );
    return projection * view;
}

void Camera::mouseButtonCallback(
    GLFWwindow* window,
    int button,
    int action,
    int
) {
    Camera* camera = static_cast<Camera*>(glfwGetWindowUserPointer(window));
    if (camera != nullptr) {
        camera->handleMouseButton(window, button, action);
    }
}

void Camera::cursorPositionCallback(
    GLFWwindow* window,
    double x,
    double y
) {
    Camera* camera = static_cast<Camera*>(glfwGetWindowUserPointer(window));
    if (camera != nullptr) {
        camera->handleCursorPosition(x, y);
    }
}

void Camera::handleMouseButton(GLFWwindow* window, int button, int action) {
    if (button != GLFW_MOUSE_BUTTON_LEFT) {
        return;
    }

    isOrbitDragging_ = action == GLFW_PRESS;
    if (isOrbitDragging_) {
        glfwGetCursorPos(window, &lastCursorX_, &lastCursorY_);
    }
}

void Camera::handleCursorPosition(double x, double y) {
    if (!isOrbitDragging_) {
        lastCursorX_ = x;
        lastCursorY_ = y;
        return;
    }

    horizontalOrbitAngle_ += static_cast<float>(x - lastCursorX_) * 0.005f;
    orbitElevationRadians_ -= static_cast<float>(y - lastCursorY_) * 0.005f;
    lastCursorX_ = x;
    lastCursorY_ = y;
    clampOrbit();
}

void Camera::clampOrbit() {
    orbitElevationRadians_ = clampValue(
        orbitElevationRadians_,
        -1.5f,
        1.5f
    );
    orbitRadius_ = clampValue(orbitRadius_, 0.8f, 10.0f);
}

Vec3 Camera::orbitPosition() const {
    const float horizontalRadius =
        orbitRadius_ * std::cos(orbitElevationRadians_);
    return {
        target_.x + horizontalRadius * std::sin(horizontalOrbitAngle_),
        target_.y + orbitRadius_ * std::sin(orbitElevationRadians_),
        target_.z + horizontalRadius * std::cos(horizontalOrbitAngle_)
    };
}
