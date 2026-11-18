#ifndef DELIVERABLE2_CAMERA_H
#define DELIVERABLE2_CAMERA_H

#include "GraphicsMath.h"

struct GLFWwindow;

class Camera {
public:
    Camera();

    void attach(GLFWwindow* window);
    void update(GLFWwindow* window, float deltaSeconds);
    Mat4 viewProjection(float aspectRatio) const;

private:
    static void mouseButtonCallback(GLFWwindow* window, int button, int action, int);
    static void cursorPositionCallback(GLFWwindow* window, double x, double y);

    void handleMouseButton(GLFWwindow* window, int button, int action);
    void handleCursorPosition(double x, double y);
    void clampOrbit();
    Vec3 orbitPosition() const;

    Vec3 target_;
    float horizontalOrbitAngle_;
    float orbitElevationRadians_;
    float orbitRadius_;
    bool isOrbitDragging_;
    double lastCursorX_;
    double lastCursorY_;
};

#endif
