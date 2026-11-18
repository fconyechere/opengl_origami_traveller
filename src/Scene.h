#ifndef DELIVERABLE2_SCENE_H
#define DELIVERABLE2_SCENE_H

#include "Landscape.h"
#include "Model.h"
#include "Texture.h"
#include "Traveler.h"
#include "Wind.h"

class Shader;

class Scene {
public:
    Scene();

    void update(float deltaSeconds);
    void draw(const Shader& shader, const Mat4& viewProjectionMatrix) const;

    Traveler& traveler();

private:
    Model notebookScene_;
    Landscape landscape_;
    Traveler traveler_;
    Texture paperTexture_;
    Wind windSystem_;
};

#endif
