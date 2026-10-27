#include "Model.h"
#include "Shader.h"

#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include <exception>
#include <iostream>
#include <stdexcept>
#include <string>

namespace {

void resizeFramebuffer(GLFWwindow*, int width, int height) {
    glViewport(0, 0, width, height);
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

    int framebufferWidth = 0;
    int framebufferHeight = 0;
    glfwGetFramebufferSize(window, &framebufferWidth, &framebufferHeight);
    glViewport(0, 0, framebufferWidth, framebufferHeight);
    return window;
}

Model createScene() {
    Model scene;
    scene.addMesh({
        -0.65f, -0.5f, 0.0f,
         0.65f, -0.5f, 0.0f,
         0.0f,   0.65f, 0.0f
    });
    return scene;
}

void renderLoop(GLFWwindow* window, const Shader& shader, const Model& scene) {
    while (glfwWindowShouldClose(window) == 0) {
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(window, 1);
        }

        glClearColor(0.12f, 0.07f, 0.035f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        shader.use();
        scene.draw();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

} // namespace

int main() {
    GLFWwindow* window = nullptr;

    try {
        window = createWindow();

        const std::string shaderDirectory = "shaders";
        const Shader shader(
            shaderDirectory + "/scene.vert",
            shaderDirectory + "/scene.frag"
        );
        const Model scene = createScene();

        renderLoop(window, shader, scene);

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
