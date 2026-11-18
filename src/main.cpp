#include "Camera.h"
#include "Scene.h"
#include "Shader.h"

#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include <exception>
#include <iostream>
#include <stdexcept>

namespace {

void resizeFramebuffer(GLFWwindow*, int width, int height) {
    // keep the viewport aligned with the window.
    glViewport(0, 0, width, height);
}

GLFWwindow* createWindow() {
    // create the window and initialize the opengl context.
    if (glfwInit() == 0) {
        throw std::runtime_error("failed to initialize glfw");
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, 1);
#endif

    GLFWwindow* window = glfwCreateWindow(
        800,
        600,
        "OpenGL Starter",
        nullptr,
        nullptr
    );  
    if (window == nullptr) {
        glfwTerminate();
        throw std::runtime_error("failed to create the window");
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, resizeFramebuffer);
    if (gladLoadGL(glfwGetProcAddress) == 0) {
        glfwDestroyWindow(window);
        glfwTerminate();
        throw std::runtime_error("failed to load opengl functions");
    }

    glEnable(GL_DEPTH_TEST);
    int width = 0;
    int height = 0;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);
    return window;
}

void renderLoop(
    GLFWwindow* window,
    const Shader& shader,
    Scene& scene,
    Camera& camera
) {
    // update input, scene state, and drawing in one predictable loop.
    float previousTime = static_cast<float>(glfwGetTime());

    while (glfwWindowShouldClose(window) == 0) {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, 1);
        }

        const float currentTime = static_cast<float>(glfwGetTime());
        const float deltaSeconds = currentTime - previousTime;
        previousTime = currentTime;

        camera.update(window, deltaSeconds);
        scene.update(deltaSeconds);

        glClearColor(0.12f, 0.07f, 0.035f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        int width = 0;
        int height = 0;
        glfwGetFramebufferSize(window, &width, &height);
        const float aspectRatio =
            static_cast<float>(width) / static_cast<float>(height);

        shader.use();
        scene.draw(shader, camera.viewProjection(aspectRatio));

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

}

int main() {
    GLFWwindow* window = nullptr;

    try {
        window = createWindow();
        Camera camera;
        camera.attach(window);
        const Shader shader("shaders/scene.vert", "shaders/scene.frag");
        Scene scene;
        renderLoop(window, shader, scene, camera);
    } catch (const std::exception& error) {
        std::cerr << error.what() << '\n';
        if (window != nullptr) {
            glfwDestroyWindow(window);
            glfwTerminate();
        }
        return 1;
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
