#include "Scene.h"

#include "Shader.h"

Scene::Scene()
    : windSystem_(0.25f, 45.0f) {
    // load the static scene before the render loop starts.
    notebookScene_.loadFromFile("assets/table.obj");
    notebookScene_.loadFromFile("assets/notebook.obj");
    paperTexture_.loadFromFile("assets/textures/notebook-paper.jpg");

    traveler_.setPosition({0.0f, -0.23f, 0.0f});
    traveler_.setRotation({0.0f, -0.6f, 0.0f});
    traveler_.setScale({0.15f, 0.15f, 0.15f});
}

void Scene::update(float deltaSeconds) {
    // advance wind before objects react to it.
    windSystem_.update(deltaSeconds);
    landscape_.update(deltaSeconds, windSystem_);
    traveler_.update(deltaSeconds);
}

void Scene::draw(
    const Shader& shader,
    const Mat4& viewProjectionMatrix
) const {
    // set lighting shared by every object.
    const Vec3 lightPosition = {-3.0f, 3.0f, 3.0f};
    const Vec3 lightTarget = {0.0f, -0.33f, 0.0f};
    shader.setVector3("lightDirection", normalize( subtract(lightTarget, lightPosition)));
    shader.setVector3("lightPosition", lightPosition);

    paperTexture_.bind(0);
    shader.setInteger("notebookTexture", 0);
    notebookScene_.draw(shader, viewProjectionMatrix);

    // landscape geometry uses the identity world transform.
    shader.setMatrix4("combinedTransformationMatrix", viewProjectionMatrix);
    shader.setMatrix4("modelMatrix", identityMatrix());
    landscape_.draw(shader, viewProjectionMatrix);
    traveler_.draw(shader, viewProjectionMatrix);
}

Traveler& Scene::traveler() {
    return traveler_;
}
