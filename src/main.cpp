#include "Model.h"
#include "Shader.h"
#include "GraphicsMath.h"
#include "Landscape.h"

#include <glad/gl.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <exception>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

namespace {

void resizeFramebuffer(GLFWwindow*, int width, int height) {
    glViewport(0, 0, width, height);
}

struct CameraControlState {
    float* horizontalCameraOrbitAngle;
    float* verticalCameraOrbitAngle;
    float* cameraOrbitDistance;
    bool isDragging;
    double previousMousePositionX;
    double previousMousePositionY;
};

void mouseButtonCallback(
    GLFWwindow* window,
    int button,
    int action,
    int
) {
    CameraControlState* cameraControlState =
        static_cast<CameraControlState*>(
            glfwGetWindowUserPointer(window)
        );

    if (cameraControlState == nullptr ||
        button != GLFW_MOUSE_BUTTON_LEFT) {
        return;
    }

    if (action == GLFW_PRESS) {
        cameraControlState->isDragging = true;
        glfwGetCursorPos(
            window,
            &cameraControlState->previousMousePositionX,
            &cameraControlState->previousMousePositionY
        );
    } else if (action == GLFW_RELEASE) {
        cameraControlState->isDragging = false;
    }
}

void cursorPositionCallback(
    GLFWwindow* window,
    double currentMousePositionX,
    double currentMousePositionY
) {
    CameraControlState* cameraControlState =
        static_cast<CameraControlState*>(
            glfwGetWindowUserPointer(window)
        );

    if (cameraControlState == nullptr ||
        !cameraControlState->isDragging) {
        return;
    }

    const double mouseMovementX =
        currentMousePositionX -
        cameraControlState->previousMousePositionX;
    const double mouseMovementY =
        currentMousePositionY -
        cameraControlState->previousMousePositionY;

    cameraControlState->previousMousePositionX = currentMousePositionX;
    cameraControlState->previousMousePositionY = currentMousePositionY;

    *cameraControlState->horizontalCameraOrbitAngle +=
        static_cast<float>(mouseMovementX) * 0.005f;

    const float proposedVerticalCameraOrbitAngle =
        *cameraControlState->verticalCameraOrbitAngle -
        static_cast<float>(mouseMovementY) * 0.005f;

    if (proposedVerticalCameraOrbitAngle > 1.5f) {
        *cameraControlState->verticalCameraOrbitAngle = 1.5f;
    } else if (proposedVerticalCameraOrbitAngle < -1.5f) {
        *cameraControlState->verticalCameraOrbitAngle = -1.5f;
    } else {
        *cameraControlState->verticalCameraOrbitAngle =
            proposedVerticalCameraOrbitAngle;
    }
}

void scrollCallback(
    GLFWwindow* window,
    double,
    double scrollMovementY
) {
    CameraControlState* cameraControlState =
        static_cast<CameraControlState*>(
            glfwGetWindowUserPointer(window)
        );

    if (cameraControlState == nullptr) {
        return;
    }

    const float proposedCameraOrbitDistance =
        *cameraControlState->cameraOrbitDistance -
        static_cast<float>(scrollMovementY) * 0.3f;

    if (proposedCameraOrbitDistance < 1.5f) {
        *cameraControlState->cameraOrbitDistance = 1.5f;
    } else if (proposedCameraOrbitDistance > 10.0f) {
        *cameraControlState->cameraOrbitDistance = 10.0f;
    } else {
        *cameraControlState->cameraOrbitDistance =
            proposedCameraOrbitDistance;
    }
}

GLFWwindow* createWindow() {
    if (glfwInit() == 0) {
        throw std::runtime_error("Failed to initialize GLFW");
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, 1);
#endif

    GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL Starter", nullptr, nullptr);
    if (window == nullptr) {
        glfwTerminate();
        throw std::runtime_error("Failed to create an OpenGL window");
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, resizeFramebuffer);

    if (gladLoadGL(glfwGetProcAddress) == 0) {
        glfwDestroyWindow(window);
        glfwTerminate();
        throw std::runtime_error("Failed to load OpenGL functions with GLAD");
    }

    glEnable(GL_DEPTH_TEST);

    int framebufferWidth = 0;
    int framebufferHeight = 0;
    glfwGetFramebufferSize(window, &framebufferWidth, &framebufferHeight);
    glViewport(0, 0, framebufferWidth, framebufferHeight);
    return window;
}

GLuint loadNotebookTexture(const std::string& path) {
    int imageWidth = 0;
    int imageHeight = 0;
    int imageChannels = 0;
    stbi_set_flip_vertically_on_load(1);
    unsigned char* imageData = stbi_load(
        path.c_str(),
        &imageWidth,
        &imageHeight,
        &imageChannels,
        0
    );
    if (imageData == nullptr) {
        throw std::runtime_error(
            "Could not load notebook texture: " + path
        );
    }

    GLenum imageFormat = GL_RGB;
    if (imageChannels == 4) {
        imageFormat = GL_RGBA;
    } else if (imageChannels != 3) {
        stbi_image_free(imageData);
        throw std::runtime_error(
            "Notebook texture must contain RGB or RGBA pixels"
        );
    }

    GLuint texture = 0;
    glGenTextures(1, &texture);
    if (texture == 0) {
        stbi_image_free(imageData);
        throw std::runtime_error("Could not create notebook texture");
    }

    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(
        GL_TEXTURE_2D,
        0,
        imageFormat,
        imageWidth,
        imageHeight,
        0,
        imageFormat,
        GL_UNSIGNED_BYTE,
        imageData
    );
    glGenerateMipmap(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, 0);
    stbi_image_free(imageData);
    return texture;
}

Model createScene() {
    Model scene;
    scene.loadFromFile("assets/table.obj");
    scene.loadFromFile("assets/notebook.obj");
    return scene;
}

void renderLoop(
    GLFWwindow* window,
    const Shader& shader,
    const Model& scene,
    GLuint notebookTexture,
    const Landscape& landscape
) {
    const Vec3 cameraTarget = {0.0f, -1.3f, 0.0f};
    float horizontalCameraOrbitAngle = 0.6f;
    float verticalCameraOrbitAngle = 0.4f;
    float cameraOrbitDistance = 4.0f;
    CameraControlState cameraControlState = {
        &horizontalCameraOrbitAngle,
        &verticalCameraOrbitAngle,
        &cameraOrbitDistance,
        false,
        0.0,
        0.0
    };

    glfwSetWindowUserPointer(window, &cameraControlState);
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwSetCursorPosCallback(window, cursorPositionCallback);
    glfwSetScrollCallback(window, scrollCallback);

    const float cameraPositionX =
        cameraOrbitDistance * std::cos(verticalCameraOrbitAngle) * std::sin(horizontalCameraOrbitAngle);

    const float cameraPositionY =
        cameraOrbitDistance * std::sin(verticalCameraOrbitAngle);
    
    const float cameraPositionZ =
        cameraOrbitDistance * std::cos(verticalCameraOrbitAngle) * std::cos(horizontalCameraOrbitAngle);

    const Vec3 cameraPosition = {cameraTarget.x + cameraPositionX, cameraTarget.y + cameraPositionY, cameraTarget.z + cameraPositionZ};
    const Vec3 worldUpDirection = {0.0f, 1.0f, 0.0f};
    const Vec3 windowLightPosition = {-3.0f, 3.0f, 3.0f};
    const Vec3 windowLightTarget = {0.0f, -0.33f, 0.0f};
    const Vec3 lightDirection =
        normalize(subtract(windowLightTarget, windowLightPosition));
    const float verticalFieldOfViewRadians = 60.0f * 3.14159265359f / 180.0f;
    const float nearClippingPlane = 0.1f;
    const float farClippingPlane = 100.0f;
    const float rotationRadiansPerSecond = 0.8f;

    float lastFrameTime = static_cast<float>(glfwGetTime());
    if(verticalCameraOrbitAngle > 1.5f){
        verticalCameraOrbitAngle = 1.5f;
    } 
    else if(verticalCameraOrbitAngle < -1.5f){
        verticalCameraOrbitAngle = -1.5f;
    }

    while (glfwWindowShouldClose(window) == 0) {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, 1);
        }

        const float currentFrameTime = static_cast<float>(glfwGetTime());
        const float deltaTime = currentFrameTime - lastFrameTime;
        lastFrameTime = currentFrameTime;

        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
            horizontalCameraOrbitAngle += deltaTime *  1.0f;
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
            horizontalCameraOrbitAngle -= deltaTime *  1.0f;
        }
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
            if(verticalCameraOrbitAngle + deltaTime *  1.0f > 1.5f){
                verticalCameraOrbitAngle = 1.5f;
            }
            else{
                verticalCameraOrbitAngle += deltaTime *  1.0f;
            }
        }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
            if(verticalCameraOrbitAngle < -1.5f || verticalCameraOrbitAngle - deltaTime *  1.0f < -1.5f){
                verticalCameraOrbitAngle = -1.5f;
            }
            else{
                verticalCameraOrbitAngle -= deltaTime *  1.0f;
            }
        }
        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
            if(cameraOrbitDistance + deltaTime *  1.0f > 10.0f){
                cameraOrbitDistance = 10.0f;
            }
            else{
                cameraOrbitDistance += deltaTime *  1.0f;
            }
        }
        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
            if(cameraOrbitDistance - deltaTime *  1.0f < 1.5f){
                cameraOrbitDistance = 1.5f;
            }
            else{
                cameraOrbitDistance -= deltaTime *  1.0f;
            }
        }

        const float cameraPositionX =
            cameraOrbitDistance * std::cos(verticalCameraOrbitAngle) * std::sin(horizontalCameraOrbitAngle);
        const float cameraPositionY =
            cameraOrbitDistance * std::sin(verticalCameraOrbitAngle);
        const float cameraPositionZ =
            cameraOrbitDistance * std::cos(verticalCameraOrbitAngle) * std::cos(horizontalCameraOrbitAngle);

        const Vec3 cameraPosition = {
            cameraTarget.x + cameraPositionX,
            cameraTarget.y + cameraPositionY,
            cameraTarget.z + cameraPositionZ
        };

        glClearColor(0.12f, 0.07f, 0.035f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        int framebufferWidth = 0;
        int framebufferHeight = 0;
        glfwGetFramebufferSize(window, &framebufferWidth, &framebufferHeight);
        const float aspectRatio =
            static_cast<float>(framebufferWidth) /
            static_cast<float>(framebufferHeight);

        const float elapsedTimeSeconds = static_cast<float>(glfwGetTime());
    
        //model, view and projection matrices
        const Mat4 modelMatrix = identityMatrix();
        const Mat4 viewMatrix =
            lookAt(cameraPosition, cameraTarget, worldUpDirection);
        const Mat4 projectionMatrix = perspective(
            verticalFieldOfViewRadians,
            aspectRatio,
            nearClippingPlane,
            farClippingPlane
        );
        const Mat4 viewModelMatrix = multiply(viewMatrix, modelMatrix);
        const Mat4 combinedTransformationMatrix =
            multiply(projectionMatrix, viewModelMatrix);

        shader.use();
        shader.setMatrix4(
            "combinedTransformationMatrix",
            combinedTransformationMatrix
        );

        shader.setMatrix4(
            "modelMatrix",
            modelMatrix
        );
        shader.setVector3("lightDirection", lightDirection);
        shader.setVector3("lightPosition", windowLightPosition);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, notebookTexture);
        shader.setInteger("notebookTexture", 0);
        scene.draw(shader);
        landscape.draw(shader);

        glfwSwapBuffers(window); 
        glfwPollEvents();
    }
}

} // namespace

int main() {
    GLFWwindow* window = nullptr;
    GLuint notebookTexture = 0;

    try {
        window = createWindow();

        const std::string shaderDirectory = "shaders";
        const Shader shader(
            shaderDirectory + "/scene.vert",
            shaderDirectory + "/scene.frag"
        );
        const Model scene = createScene();
        const Landscape landscape;
        notebookTexture = loadNotebookTexture(
            "assets/textures/notebook-paper.jpg"
        );

        renderLoop(window, shader, scene, notebookTexture, landscape);

    } catch (const std::exception& error) {
        std::cerr << error.what() << '\n';
        if (notebookTexture != 0) {
            glDeleteTextures(1, &notebookTexture);
        }
        if (window != nullptr) {
            glfwDestroyWindow(window);
            glfwTerminate();
        }
        return 1;
    }

    if (notebookTexture != 0) {
        glDeleteTextures(1, &notebookTexture);
    }
    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
