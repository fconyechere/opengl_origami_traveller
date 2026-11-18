#include "ProceduralTree.h"

#include <cmath>

namespace {

Vec3 add(const Vec3& left, const Vec3& right) {
    // simple vector addition, used for moving points around in 3D space.
    return {left.x + right.x, left.y + right.y, left.z + right.z};
}

Vec3 multiply(const Vec3& vector, float value) {
    // simple vector scaling, used for moving points around in 3D space.
    return {vector.x * value, vector.y * value, vector.z * value};
}

}

ProceduralTree::ProceduralTree(const Vec3& treePosition, float treeHeight, int recursionLevels, ProceduralTreeType treeType) {
    // Geometry is generated once when each tree is constructed.
    createTree(treePosition, treeHeight, recursionLevels, treeType);
}

void ProceduralTree::createTree(const Vec3& treePosition, float treeHeight, int recursionLevels, ProceduralTreeType treeType) {
    // pick a different paper tree style using the same triangle helpers
    if (treeType == ProceduralTreeType::Layered) {
        createLayeredTree(treePosition, treeHeight, recursionLevels);
    } 
    else if (treeType == ProceduralTreeType::Fan) {
        createFanTree(treePosition, treeHeight, recursionLevels);
    } 
    else {
        createBranch(treePosition, {0.0f, 1.0f, 0.0f}, treeHeight, treeHeight * 0.18f, recursionLevels);
    }
}

const std::vector<GLfloat>& ProceduralTree::positions() const {
    return positions_;
}

const std::vector<GLfloat>& ProceduralTree::normals() const {
    return normals_;
}

const std::vector<GLfloat>& ProceduralTree::textureCoordinates() const {
    return textureCoordinates_;
}

const std::vector<PerchSpot>& ProceduralTree::perchSpots() const {
    return perchSpots_;
}

void ProceduralTree::createBranch(const Vec3& branchStart, const Vec3& branchDirection, float branchLength, float branchWidth, int remainingLevels) {
     // each branch can create three smaller ones.
    
    //this is the stop condition that prevents infinite recursion.
    if (remainingLevels == 0) {
        return;
    }

    addFoldedBranch( branchStart, branchDirection, branchLength, branchWidth);

    if (remainingLevels == 1){
        addPointedLeaf(add(branchStart, multiply(branchDirection, branchLength * 0.72f)), branchDirection, branchLength * 0.80f, branchWidth * 2.8f);
        return;
    }

    // Cross product gives a sideways direction for the folded branch.
    Vec3 sideDirection = cross(branchDirection, {0.0f, 1.0f, 0.0f});
    // If the branch is pointing straight up cross product will be zero, so theres a fallback sideways direction
    if (std::abs(sideDirection.x) < 0.001f && std::abs(sideDirection.z) < 0.001f) {
        sideDirection = {1.0f, 0.0f, 0.0f};
    } 
    else {
        sideDirection = normalize(sideDirection);
    }

    const Vec3 branchEnd = add( branchStart, multiply(branchDirection, branchLength));

    //just an average of the current direction and a sideways/upward offset to fan out the next generation of branches
    const Vec3 leftBranchDirection = normalize(add(multiply(branchDirection, 0.66f), add(multiply(sideDirection, 0.78f), {0.0f, 0.44f, 0.10f})));
    const Vec3 upwardBranchDirection = normalize(add(multiply(branchDirection, 0.84f), {0.0f, 0.52f, 0.0f}));
    const Vec3 rightBranchDirection = normalize(add(multiply(branchDirection, 0.74f), add(multiply(sideDirection, -0.52f), {0.0f, 0.31f, -0.12f})));

    createBranch(branchEnd, leftBranchDirection, branchLength * 0.52f, branchWidth * 0.58f, remainingLevels - 1);
    createBranch(branchEnd, upwardBranchDirection, branchLength * 0.60f, branchWidth * 0.54f, remainingLevels - 1);
    createBranch(branchEnd, rightBranchDirection, branchLength * 0.49f, branchWidth * 0.55f, remainingLevels - 1);
}

void ProceduralTree::addFoldedBranch(const Vec3& branchStart, const Vec3& branchDirection, float branchLength, float branchWidth) {
    // branches are two triangles folded around a centre line.
    Vec3 sideDirection = cross(branchDirection, {0.0f, 1.0f, 0.0f});
    if (std::abs(sideDirection.x) < 0.001f && std::abs(sideDirection.z) < 0.001f) {
        sideDirection = {1.0f, 0.0f, 0.0f};
    } 
    else {
        sideDirection = normalize(sideDirection);
    }

    const Vec3 branchEnd = add(branchStart, multiply(branchDirection, branchLength));
    const Vec3 leftPoint = add(branchStart, multiply(sideDirection, branchWidth));
    const Vec3 rightPoint = add(branchStart, multiply(sideDirection, -branchWidth));
    const Vec3 foldPoint = add(add(branchStart, multiply(branchDirection, branchLength * 0.52f)), multiply(sideDirection, branchWidth * 0.28f));

    addTriangle(leftPoint, branchEnd, foldPoint);
    addTriangle(foldPoint, branchEnd, rightPoint);
}

void ProceduralTree::addPointedLeaf(const Vec3& leafStart, const Vec3& leafDirection, float leafLength, float leafWidth) {
    //leaves use the same folded paper idea but taper to a point.
    Vec3 sideDirection = cross(leafDirection, {0.0f, 1.0f, 0.0f});
    if (std::abs(sideDirection.x) < 0.001f && std::abs(sideDirection.z) < 0.001f) {
        sideDirection = {1.0f, 0.0f, 0.0f};
    } 
    else {
        sideDirection = normalize(sideDirection);
    }

    const Vec3 leafTip = add(leafStart, multiply(leafDirection, leafLength));
    perchSpots_.push_back({
        leafStart,
        0.12f
    });
    const Vec3 leftPoint = add(leafStart, multiply(sideDirection, leafWidth));
    const Vec3 rightPoint = add(leafStart, multiply(sideDirection, -leafWidth));
    const Vec3 foldPoint = add(add(leafStart, multiply(leafDirection, leafLength * 0.46f)), multiply(sideDirection, leafWidth * 0.25f));

    addTriangle(leftPoint, leafTip, foldPoint);
    addTriangle(foldPoint, leafTip, rightPoint);
}

void ProceduralTree::createLayeredTree(const Vec3& treePosition, float treeHeight, int recursionLevels) {
    // Layered trees stack smaller folded panels as they get taller.
    const int numberOfLayers = recursionLevels + 1;
    for (int layer = 0; layer < numberOfLayers; ++layer) {
        const float layerFraction = static_cast<float>(layer + 1) / static_cast<float>(numberOfLayers);
        const Vec3 layerCenter = add(treePosition, {0.0f, treeHeight * layerFraction * 0.78f, 0.0f});
        addFoldedPanel(layerCenter, {0.0f, 0.25f, 1.0f}, treeHeight * (0.38f - layer * 0.04f), treeHeight * (0.45f - layer * 0.05f));
    }

    addFoldedBranch(treePosition, {0.0f, 1.0f, 0.0f}, treeHeight * 0.90f, treeHeight * 0.10f);
}

void ProceduralTree::createFanTree(const Vec3& treePosition, float treeHeight, int recursionLevels) {
    // Fan trees use one stem and several leaves spread across the top.
    addFoldedBranch(treePosition, {0.0f, 1.0f, 0.0f}, treeHeight * 0.72f, treeHeight * 0.12f);

    const int numberOfLeaves = recursionLevels + 3;
    for (int leafIndex = 0; leafIndex < numberOfLeaves; ++leafIndex) {
        const float leafFraction = static_cast<float>(leafIndex) / static_cast<float>(numberOfLeaves - 1);
        const float horizontalDirection = -0.75f + leafFraction * 1.5f;
        const Vec3 leafDirection = normalize({horizontalDirection, 0.72f + 0.18f * (1.0f - std::abs(horizontalDirection)), 0.0f});
        addPointedLeaf(add(treePosition, {0.0f, treeHeight * 0.48f, 0.0f}), leafDirection, treeHeight * 0.42f, treeHeight * 0.18f
        );
    }
}

void ProceduralTree::addFoldedPanel(const Vec3& panelCenter, const Vec3& panelDirection, float panelLength, float panelWidth) {
    // Panels are another pair of triangles, positioned around a centre point.
    perchSpots_.push_back({
        panelCenter,
        0.12f
    });
    Vec3 sideDirection = normalize(cross(panelDirection, {0.0f, 1.0f, 0.0f}));
    const Vec3 panelTip = add(panelCenter, multiply(normalize(panelDirection), panelLength));
    const Vec3 leftPoint = add(panelCenter, multiply(sideDirection, panelWidth));
    const Vec3 rightPoint = add(panelCenter, multiply(sideDirection, -panelWidth));
    const Vec3 foldPoint = add(panelCenter, multiply(sideDirection, panelWidth * 0.24f));

    addTriangle(leftPoint, panelTip, foldPoint);
    addTriangle(foldPoint, panelTip, rightPoint);
}

void ProceduralTree::addTriangle(const Vec3& firstPoint, const Vec3& secondPoint, const Vec3& thirdPoint) {
    // The cross product of two triangle edges gives its surface normal.
    const Vec3 firstEdge = subtract(secondPoint, firstPoint);
    const Vec3 secondEdge = subtract(thirdPoint, firstPoint);
    const Vec3 triangleNormal = normalize(cross(firstEdge, secondEdge));

    const Vec3 trianglePoints[3] = {firstPoint, secondPoint, thirdPoint};

    // Store each triangle as three complete vertices for glDrawArrays.
    for (const Vec3& point : trianglePoints) {
        positions_.push_back(point.x);
        positions_.push_back(point.y);
        positions_.push_back(point.z);
        normals_.push_back(triangleNormal.x);
        normals_.push_back(triangleNormal.y);
        normals_.push_back(triangleNormal.z);
        textureCoordinates_.push_back(0.0f);
        textureCoordinates_.push_back(0.0f);
    }
}