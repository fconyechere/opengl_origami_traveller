#ifndef DELIVERABLE2_PROCEDURAL_TREE_H
#define DELIVERABLE2_PROCEDURAL_TREE_H

#include "GraphicsMath.h"

#include <glad/gl.h>

#include <vector>

enum class ProceduralTreeType {
    Branching,
    Layered,
    Fan
};

struct PerchSpot {
    Vec3 position;
    float branchWeight;
};

class ProceduralTree {
public:
    ProceduralTree(const Vec3& treePosition, float treeHeight, int recursionLevels, ProceduralTreeType treeType = ProceduralTreeType::Branching);

    const std::vector<GLfloat>& positions() const;
    const std::vector<GLfloat>& normals() const;
    const std::vector<GLfloat>& textureCoordinates() const;
    const std::vector<PerchSpot>& perchSpots() const;

private:
    void createTree(const Vec3& treePosition, float treeHeight, int recursionLevels, ProceduralTreeType treeType);

    void createLayeredTree(const Vec3& treePosition, float treeHeight, int recursionLevels);

    void createFanTree(const Vec3& treePosition, float treeHeight, int recursionLevels);

    void createBranch(const Vec3& branchStart, const Vec3& branchDirection, float branchLength, float branchWidth, int remainingLevels);

    void addFoldedBranch(const Vec3& branchStart, const Vec3& branchDirection, float branchLength, float branchWidth);

    void addPointedLeaf(const Vec3& leafStart, const Vec3& leafDirection, float leafLength, float leafWidth);

    void addFoldedPanel(const Vec3& panelCentre, const Vec3& panelDirection, float panelLength, float panelWidth);

    void addTriangle(const Vec3& firstPoint, const Vec3& secondPoint, const Vec3& thirdPoint);

    std::vector<GLfloat> positions_;
    std::vector<GLfloat> normals_;
    std::vector<GLfloat> textureCoordinates_;
    std::vector<PerchSpot> perchSpots_;
};

#endif