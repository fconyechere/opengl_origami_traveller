#include "Model.h"
#include "Shader.h"
#include "GraphicsMath.h"
#include "Landscape.h"

#include <glad/gl.h>
#include <GLFW/glfw3.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <exception>
#include <array>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

namespace {

void resizeFramebuffer(GLFWwindow*, int width, int height) {
    // Keep the viewport in sync when the window changes size.
    glViewport(0, 0, width, height);
}

// The camera orbits around the scene instead of moving completely freely.
struct CameraControlState {
    float* horizontalOrbitAngle;
    float* orbitElevationRadians;
    float* orbitRadius;
    bool isOrbitDragging;
    double lastCursorX;
    double lastCursorY;
};

float clampValue(float value, float minimum, float maximum) {
    // Stops the camera from going past sensible angles or distances.
    if (value < minimum) {
        return minimum;
    }
    if (value > maximum) {
        return maximum;
    }
    return value;
}

void updateCameraFromKeyboard( GLFWwindow* renderWindow, float frameDeltaSeconds, CameraControlState& camera) {
    // Keyboard movement is scaled by delta time so it feels consistent at  different frame rates.
    const float orbitSpeed = frameDeltaSeconds;

    // WASD rotates the camera around the scene, and the arrow keys zoom in and out.
    if (glfwGetKey(renderWindow, GLFW_KEY_A) == GLFW_PRESS) {
        *camera.horizontalOrbitAngle += orbitSpeed;
    }
    if (glfwGetKey(renderWindow, GLFW_KEY_D) == GLFW_PRESS) {
        *camera.horizontalOrbitAngle -= orbitSpeed;
    }
    if (glfwGetKey(renderWindow, GLFW_KEY_W) == GLFW_PRESS) {
        *camera.orbitElevationRadians = clampValue( *camera.orbitElevationRadians + orbitSpeed, -1.5f, 1.5f );
    }
    if (glfwGetKey(renderWindow, GLFW_KEY_S) == GLFW_PRESS) {
        *camera.orbitElevationRadians = clampValue( *camera.orbitElevationRadians - orbitSpeed, -1.5f, 1.5f);
    }
    if (glfwGetKey(renderWindow, GLFW_KEY_UP) == GLFW_PRESS) {
        *camera.orbitRadius = clampValue( *camera.orbitRadius + orbitSpeed, 0.8f, 10.0f);
    }
    if (glfwGetKey(renderWindow, GLFW_KEY_DOWN) == GLFW_PRESS) {
        *camera.orbitRadius = clampValue(*camera.orbitRadius - orbitSpeed, 0.8f, 10.0f);
    }
}

Vec3 calculateOrbitPosition(
    const Vec3& target,
    float horizontalAngle,
    float elevationRadians,
    float radius
) {
    // Convert the orbit angles and distance into an actual camera position.
    const float horizontalRadius =
        radius * std::cos(elevationRadians);
    return {
        target.x + horizontalRadius * std::sin(horizontalAngle),
        target.y + radius * std::sin(elevationRadians),
        target.z + horizontalRadius * std::cos(horizontalAngle)
    };
}

// Start and stop orbiting when the left mouse button is pressed.
void mouseButtonCallback(GLFWwindow* renderWindow, int button, int action, int) {
    CameraControlState* orbitInputState =
        static_cast<CameraControlState*>(
            glfwGetWindowUserPointer(renderWindow)
        );

    if (orbitInputState == nullptr ||
        button != GLFW_MOUSE_BUTTON_LEFT) {
        return;
    }

    if (action == GLFW_PRESS) {
        orbitInputState->isOrbitDragging = true;
        glfwGetCursorPos( renderWindow, &orbitInputState->lastCursorX, &orbitInputState->lastCursorY);
    } else if (action == GLFW_RELEASE) {
        orbitInputState->isOrbitDragging = false;
    }
}

// While dragging, horizontal movement rotates around the scene and vertical
// movement changes the camera height.
void cursorPositionCallback(GLFWwindow* renderWindow, double cursorX, double cursorY) {
    CameraControlState* orbitInputState =
        static_cast<CameraControlState*>(
            glfwGetWindowUserPointer(renderWindow)
        );

    if (orbitInputState == nullptr ||
        !orbitInputState->isOrbitDragging) {
        return;
    }

    const double cursorDeltaX = cursorX - orbitInputState->lastCursorX;
    const double cursorDeltaY = cursorY - orbitInputState->lastCursorY;

    orbitInputState->lastCursorX = cursorX;
    orbitInputState->lastCursorY = cursorY;

    *orbitInputState->horizontalOrbitAngle += static_cast<float>(cursorDeltaX) * 0.005f;

    *orbitInputState->orbitElevationRadians = clampValue( *orbitInputState->orbitElevationRadians - static_cast<float>(cursorDeltaY) * 0.005f, -1.5f, 1.5f);
}

GLFWwindow* createWindow() {
    // Set up the OpenGL window and the callbacks needed by the renderer.
    if (glfwInit() == 0) {
        throw std::runtime_error("Failed to initialize GLFW");
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, 1);
#endif

    GLFWwindow* renderWindow = glfwCreateWindow(800, 600, "OpenGL Starter", nullptr, nullptr);
    if (renderWindow == nullptr) {
        glfwTerminate();
        throw std::runtime_error("Failed to create an OpenGL renderWindow");
    }

    glfwMakeContextCurrent(renderWindow);
    glfwSetFramebufferSizeCallback(renderWindow, resizeFramebuffer);

    if (gladLoadGL(glfwGetProcAddress) == 0) {
        glfwDestroyWindow(renderWindow);
        glfwTerminate();
        throw std::runtime_error("Failed to load OpenGL functions with GLAD");
    }

    // Depth testing makes nearer objects correctly hide objects behind them.
    glEnable(GL_DEPTH_TEST);

    int framebufferWidth = 0;
    int framebufferHeight = 0;
    glfwGetFramebufferSize(renderWindow, &framebufferWidth, &framebufferHeight);
    glViewport(0, 0, framebufferWidth, framebufferHeight);
    return renderWindow;
}

GLuint loadNotebookTexture(const std::string& path) {
    // Load the notebook image once and turn it into an OpenGL texture.
    int imageWidth = 0;
    int imageHeight = 0;
    int imageChannels = 0;
    stbi_set_flip_vertically_on_load(1);
    unsigned char* imageData = stbi_load(path.c_str(), &imageWidth, &imageHeight, &imageChannels,0);
    
    if (imageData == nullptr) {
        throw std::runtime_error(
            "Could not load notebook texture: " + path
        );
    }

    GLenum imageFormat = GL_RGB;
    
    if (imageChannels == 4) {
        imageFormat = GL_RGBA;
    } 
    else if (imageChannels != 3) {
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
    // Repeat and filtering make the texture look reasonable on the notebook.
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
    // Both imported models are kept in one scene object and drawn together.
    Model notebookScene;
    notebookScene.loadFromFile("assets/table.obj");
    notebookScene.loadFromFile("assets/notebook.obj");
    return notebookScene;
}

void renderLoop(GLFWwindow* renderWindow, const Shader& sceneShader, const Model& notebookScene, GLuint paperTexture, const Landscape& paperLandscape, const Model& traveler) {
    // Everything that changes every frame lives in this loop.
    const Vec3 miniatureWorldTarget = {0.0f, -0.23f, 0.0f};
    float horizontalOrbitAngle = 0.6f;
    float orbitElevationRadians = 0.55f;
    float orbitRadius = 1.8f;
    
    CameraControlState orbitInputState = {
        &horizontalOrbitAngle,
        &orbitElevationRadians,
        &orbitRadius,
        false,
        0.0,
        0.0
    };

    // GLFW callbacks use this pointer to access the current camera state.
    glfwSetWindowUserPointer(renderWindow, &orbitInputState);
    glfwSetMouseButtonCallback(renderWindow, mouseButtonCallback);
    glfwSetCursorPosCallback(renderWindow, cursorPositionCallback);

    const Vec3 worldUp = {0.0f, 1.0f, 0.0f};
    // The light points from this position towards the middle of the scene.
    const Vec3 lampPosition = {-3.0f, 3.0f, 3.0f};
    const Vec3 lampTarget = {0.0f, -0.33f, 0.0f};
    const Vec3 lampDirection = normalize(subtract(lampTarget, lampPosition));
    const float fieldOfViewRadians = 60.0f * 3.14159265359f / 180.0f;
    const float nearClipDistance = 0.1f;
    const float farClipDistance = 100.0f;
    const float sceneRotationSpeedRadians = 0.8f;

    float previousFrameTimestamp = static_cast<float>(glfwGetTime());

    while (glfwWindowShouldClose(renderWindow) == 0) {
        // Escape is handled here so the main loop can shut down cleanly.
        if (glfwGetKey(renderWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
            glfwSetWindowShouldClose(renderWindow, 1);
        }

        // Work out how much time passed since the previous frame.
        const float frameTimestamp = static_cast<float>(glfwGetTime());
        const float frameDeltaSeconds = frameTimestamp - previousFrameTimestamp;
        previousFrameTimestamp = frameTimestamp;

        updateCameraFromKeyboard(
            renderWindow,
            frameDeltaSeconds,
            orbitInputState
        );

        // Recalculate the camera position after any keyboard or mouse input.
        const Vec3 viewerPosition = calculateOrbitPosition(
            miniatureWorldTarget,
            horizontalOrbitAngle,
            orbitElevationRadians,
            orbitRadius
        );

        // Start with a clean colour buffer and depth buffer for this frame.
        glClearColor(0.12f, 0.07f, 0.035f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        int framebufferWidth = 0;
        int framebufferHeight = 0;
        glfwGetFramebufferSize(renderWindow, &framebufferWidth, &framebufferHeight);
        const float aspectRatio =
            static_cast<float>(framebufferWidth) /
            static_cast<float>(framebufferHeight);

        const float animationTimeSeconds = static_cast<float>(glfwGetTime());
    
        // Model = object placement, view = camera, projection = perspective.
        const Mat4 worldModelMatrix = identityMatrix();
        const Mat4 cameraViewMatrix = lookAt(viewerPosition, miniatureWorldTarget, worldUp);
        const Mat4 cameraProjectionMatrix = perspective(
            fieldOfViewRadians,
            aspectRatio,
            nearClipDistance,
            farClipDistance
        );
        const Mat4 cameraModelViewMatrix = multiply(cameraViewMatrix, worldModelMatrix);
        const Mat4 combinedTransformationMatrix = multiply(cameraProjectionMatrix, cameraModelViewMatrix);

        // These uniforms are shared by the table, notebook, and landscape.
        sceneShader.use();
        sceneShader.setMatrix4("combinedTransformationMatrix", combinedTransformationMatrix);

        sceneShader.setMatrix4("modelMatrix", worldModelMatrix);
        sceneShader.setVector3("lightDirection", lampDirection);
        sceneShader.setVector3("lightPosition", lampPosition);

        // Bind the notebook image to texture unit 0 before drawing.
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, paperTexture);
        sceneShader.setInteger("notebookTexture", 0);
        notebookScene.draw(sceneShader);
        paperLandscape.draw(sceneShader);

        // Build the traveler's animation from a root transform and child
        // transforms. Each limb inherits the movement of its parent.
        const Mat4 characterTranslation = translation({0.0f, -0.23f, 0.0f});
        const Mat4 characterScale = scale({0.15f, 0.15f, 0.15f});
        const Mat4 characterFacingRotation = rotationY(-0.6f);

        // Sine gives us a smooth repeating animation between 0 and 1.
        const float jumpingJackCycle = animationTimeSeconds * 3.0f;
        const float jumpingJackAmount = (std::sin(jumpingJackCycle) + 1.0f) * 0.5f;
        const float jumpingJackBob = jumpingJackAmount * 0.035f;
        const Mat4 characterRootTransformation = multiply(characterTranslation, multiply(translation({0.0f, jumpingJackBob, 0.0f}), multiply(characterFacingRotation, characterScale)));
        const float jumpingJackAngle = jumpingJackAmount * 0.55f;
        const float leftLegAngle = -jumpingJackAngle;
        const float rightLegAngle = jumpingJackAngle;
        
        // Rotate each limb around its joint: move to the joint, rotate, then
        // move back. Rotating without this would swing it around the origin.
        const Mat4 leftHipTransformation = 
            multiply(characterRootTransformation, 
                multiply( translation({-0.10f, 0.32f, 0.0f}), 
                    multiply(rotationZ(leftLegAngle), 
                    translation({0.10f, -0.32f, 0.0f}))));
        
        const Mat4 rightHipTransformation =
            multiply(
                characterRootTransformation,
                multiply(
                    translation({0.10f, 0.32f, 0.0f}),
                    multiply(
                        rotationZ(rightLegAngle),
                        translation({-0.10f, -0.32f, 0.0f})
                    )
                )
            );
        const Mat4 leftKneeTransformation =
            multiply(
                leftHipTransformation,
                multiply(
                    translation({-0.10f, 0.16f, 0.0f}),
                    multiply(
                        rotationZ(-leftLegAngle * 0.45f),
                        translation({0.10f, -0.16f, 0.0f})
                    )
                )
            );
        const Mat4 rightKneeTransformation =
            multiply(
                rightHipTransformation,
                multiply(
                    translation({0.10f, 0.16f, 0.0f}),
                    multiply(
                        rotationZ(-rightLegAngle * 0.45f),
                        translation({-0.10f, -0.16f, 0.0f})
                    )
                )
            );
        const Mat4 leftShoulderTransformation =
            multiply(
                characterRootTransformation,
                multiply(
                    translation({-0.18f, 0.62f, 0.0f}),
                    multiply(
                        rotationZ(-jumpingJackAngle),
                        translation({0.18f, -0.62f, 0.0f})
                    )
                )
            );
        const Mat4 rightShoulderTransformation =
            multiply(
                characterRootTransformation,
                multiply(
                    translation({0.18f, 0.62f, 0.0f}),
                    multiply(
                        rotationZ(jumpingJackAngle),
                        translation({-0.18f, -0.62f, 0.0f})
                    )
                )
            );
        const Mat4 leftHandTransformation =
            multiply(
                leftShoulderTransformation,
                multiply(
                    translation({-0.30f, 0.51f, 0.0f}),
                    multiply(
                        rotationZ(-jumpingJackAngle * 0.25f),
                        translation({0.30f, -0.51f, 0.0f})
                    )
                )
            );
        const Mat4 rightHandTransformation =
            multiply(
                rightShoulderTransformation,
                multiply(
                    translation({0.30f, 0.51f, 0.0f}),
                    multiply(
                        rotationZ(jumpingJackAngle * 0.25f),
                        translation({-0.30f, -0.51f, 0.0f})
                    )
                )
            );
        // Start every part at the root, then override the parts that animate.
        std::array<Mat4, static_cast<std::size_t>(CharacterPart::Count)>
            characterPartModelMatrices;
        characterPartModelMatrices.fill(characterRootTransformation);
        characterPartModelMatrices[
            static_cast<std::size_t>(CharacterPart::LeftArm)
        ] = leftShoulderTransformation;
        characterPartModelMatrices[
            static_cast<std::size_t>(CharacterPart::RightArm)
        ] = rightShoulderTransformation;
        characterPartModelMatrices[
            static_cast<std::size_t>(CharacterPart::LeftHand)
        ] = leftHandTransformation;
        characterPartModelMatrices[
            static_cast<std::size_t>(CharacterPart::RightHand)
        ] = rightHandTransformation;
        characterPartModelMatrices[
            static_cast<std::size_t>(CharacterPart::LeftLeg)
        ] = leftHipTransformation;
        characterPartModelMatrices[
            static_cast<std::size_t>(CharacterPart::RightLeg)
        ] = rightHipTransformation;
        characterPartModelMatrices[
            static_cast<std::size_t>(CharacterPart::LeftCalf)
        ] = leftKneeTransformation;
        characterPartModelMatrices[
            static_cast<std::size_t>(CharacterPart::RightCalf)
        ] = rightKneeTransformation;
        characterPartModelMatrices[
            static_cast<std::size_t>(CharacterPart::LeftFoot)
        ] = leftKneeTransformation;
        characterPartModelMatrices[
            static_cast<std::size_t>(CharacterPart::RightFoot)
        ] = rightKneeTransformation;
        // The character uses a separate model matrix for each body part.
        const Mat4 viewProjectionMatrix = multiply(cameraProjectionMatrix, cameraViewMatrix);
        traveler.drawCharacter( sceneShader, viewProjectionMatrix, characterPartModelMatrices);

        // Show the finished back buffer, then collect the next input events.
        glfwSwapBuffers(renderWindow); 
        glfwPollEvents();
    }
}

} // namespace

int main() {
    GLFWwindow* renderWindow = nullptr;
    GLuint paperTexture = 0;

    try {
        // Create the GPU resources and scene objects before entering the loop.
        renderWindow = createWindow();

        const std::string sceneShaderDirectory = "shaders";
        const Shader sceneShader( sceneShaderDirectory + "/scene.vert", sceneShaderDirectory + "/scene.frag");
        const Model notebookScene = createScene();
        const Landscape paperLandscape;
        Model traveler;
        traveler.loadCharacterFromFile( "assets/origami_traveler/origami_traveler.obj");
        paperTexture = loadNotebookTexture("assets/textures/notebook-paper.jpg");

        renderLoop(
            renderWindow,
            sceneShader,
            notebookScene,
            paperTexture,
            paperLandscape,
            traveler
        );

    } catch (const std::exception& error) {
        // Report setup/render errors and release anything that was created.
        std::cerr << error.what() << '\n';
        if (paperTexture != 0) {
            glDeleteTextures(1, &paperTexture);
        }
        if (renderWindow != nullptr) {
            glfwDestroyWindow(renderWindow);
            glfwTerminate();
        }
        return 1;
    }

    if (paperTexture != 0) {
        glDeleteTextures(1, &paperTexture);
    }
    glfwDestroyWindow(renderWindow);
    glfwTerminate();
    return 0;
}
