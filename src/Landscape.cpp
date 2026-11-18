#include "Landscape.h"
#include "ProceduralTree.h"
#include "Shader.h"

#include <cmath>
#include <algorithm>
#include <random>
#include <vector>

namespace {

void addTriangle(std::vector<GLfloat>& positions, std::vector<GLfloat>& normals, std::vector<GLfloat>& textureCoordinates, const Vec3& firstPoint, const Vec3& secondPoint, const Vec3& thirdPoint) {
    // All procedural landscape pieces eventually become triangles with
    // positions, normals, and placeholder UVs.
    const Vec3 firstEdge = subtract(secondPoint, firstPoint);
    const Vec3 secondEdge = subtract(thirdPoint, firstPoint);
    const Vec3 triangleNormal = normalize(cross(firstEdge, secondEdge));
    const Vec3 trianglePoints[3] = {
        firstPoint,
        secondPoint,
        thirdPoint
    };

    for (const Vec3& point : trianglePoints) {
        positions.push_back(point.x);
        positions.push_back(point.y);
        positions.push_back(point.z);
        normals.push_back(triangleNormal.x);
        normals.push_back(triangleNormal.y);
        normals.push_back(triangleNormal.z);
        textureCoordinates.push_back(0.0f);
        textureCoordinates.push_back(0.0f);
    }
}

void addDirtMount(std::vector<GLfloat>& positions, std::vector<GLfloat>& normals, std::vector<GLfloat>& textureCoordinates, const Vec3& centre, float width, float height) {
    // Four folded triangles make a small pyramid-like patch of dirt.
    const Vec3 leftPoint = {centre.x - width, centre.y, centre.z};
    const Vec3 rightPoint = {centre.x + width, centre.y, centre.z};
    const Vec3 frontPoint = {centre.x, centre.y, centre.z + width};
    const Vec3 backPoint = {centre.x, centre.y, centre.z - width};
    const Vec3 topPoint = {centre.x, centre.y + height, centre.z};

    addTriangle(
        positions, normals, textureCoordinates,
        leftPoint, frontPoint, topPoint
    );
    addTriangle(
        positions, normals, textureCoordinates,
        frontPoint, rightPoint, topPoint
    );
    addTriangle(
        positions, normals, textureCoordinates,
        rightPoint, backPoint, topPoint
    );
    addTriangle(
        positions, normals, textureCoordinates,
        backPoint, leftPoint, topPoint
    );
}

void addGrassBlade(std::vector<GLfloat>& positions, std::vector<GLfloat>& normals, std::vector<GLfloat>& textureCoordinates, const Vec3& base, float height, float width) {
    // A blade is a few thin triangles, which is enough to suggest grass.
    const Vec3 leftPoint = {base.x - width, base.y, base.z};
    const Vec3 centrePoint = {base.x, base.y, base.z};
    const Vec3 rightPoint = {base.x + width, base.y, base.z};
    const Vec3 leftTip = {
        base.x - width * 1.15f,
        base.y + height * 0.82f,
        base.z + width * 0.50f
    };
    const Vec3 centreTip = {
        base.x,
        base.y + height,
        base.z - width * 0.25f
    };
    const Vec3 rightTip = {
        base.x + width * 1.15f,
        base.y + height * 0.76f,
        base.z + width * 0.45f
    };

    addTriangle(
        positions, normals, textureCoordinates,
        leftPoint, leftTip, centrePoint
    );
    addTriangle(
        positions, normals, textureCoordinates,
        centrePoint, centreTip, rightPoint
    );
    addTriangle(
        positions, normals, textureCoordinates,
        centrePoint, rightTip, rightPoint
    );
}

std::vector<ProceduralTree> createForest() {
    // The fixed seed keeps the random-looking forest the same every run.
    std::vector<ProceduralTree> trees;
    std::mt19937 randomGenerator(20261030);
    std::uniform_real_distribution<float> xDistribution(-0.78f, 0.78f);
    std::uniform_real_distribution<float> zDistribution(-0.54f, 0.54f);
    std::uniform_real_distribution<float> heightDistribution(0.24f, 0.42f);
    std::uniform_int_distribution<int> recursionDistribution(1, 3);
    std::uniform_int_distribution<int> typeDistribution(0, 2);

    // Keep trying positions until we have the full forest.
    while (trees.size() < 96) {
        const float x = xDistribution(randomGenerator);
        const float z = zDistribution(randomGenerator);
        const float clearingDistance =
            (x * x) / (0.30f * 0.30f) +
            (z * z) / (0.25f * 0.25f);

        // Reject points inside the ellipse so the traveller has a clearing.
        if (clearingDistance < 1.0f) {
            continue;
        }

        const int treeTypeNumber = typeDistribution(randomGenerator);
        ProceduralTreeType treeType = ProceduralTreeType::Branching;
        if (treeTypeNumber == 1) {
            treeType = ProceduralTreeType::Layered;
        } else if (treeTypeNumber == 2) {
            treeType = ProceduralTreeType::Fan;
        }

        trees.emplace_back(
            Vec3{x, -0.23f, z},
            heightDistribution(randomGenerator),
            recursionDistribution(randomGenerator),
            treeType
        );
    }

    return trees;
}

}

Landscape::Landscape()
    : treeMesh_(nullptr),
      grassMesh_(nullptr),
      dirtMesh_(nullptr) {
    // Generate all landscape geometry once, before the render loop starts.
    trees_ = createForest();

    std::vector<GLfloat> positions;
    std::vector<GLfloat> normals;
    std::vector<GLfloat> textureCoordinates;

    // Combine every tree into one large mesh to avoid one draw call per tree.
    perchSpots_.clear();
    for (const ProceduralTree& tree : trees_) {
        positions.insert(
            positions.end(),
            tree.positions().begin(),
            tree.positions().end()
        );
        normals.insert(
            normals.end(),
            tree.normals().begin(),
            tree.normals().end()
        );
        textureCoordinates.insert(
            textureCoordinates.end(),
            tree.textureCoordinates().begin(),
            tree.textureCoordinates().end()
        );
        perchSpots_.insert(
            perchSpots_.end(),
            tree.perchSpots().begin(),
            tree.perchSpots().end()
        );
    }

    // spawn a stable set of birds at shuffled tree perch spots.
    birdGenerator_.seed(20261118);
    std::shuffle(perchSpots_.begin(), perchSpots_.end(), birdGenerator_);
    const std::size_t birdCount = std::min<std::size_t>(12, perchSpots_.size());
    std::uniform_real_distribution<float> flapDistribution(0.0f, 6.2831853f);
    std::uniform_real_distribution<float> reactionDistribution(0.0f, 0.4f);
    for (std::size_t birdIndex = 0; birdIndex < birdCount; ++birdIndex) {
        birds_.emplace_back(
            perchSpots_[birdIndex],
            flapDistribution(birdGenerator_),
            reactionDistribution(birdGenerator_)
        );
    }

    treeMesh_.reset(new Mesh(
        positions,
        normals,
        {0.92f, 0.92f, 0.92f},
        textureCoordinates,
        false
    ));

    // Grass positions are hand-picked around the edge of the notebook page.
    std::vector<GLfloat> grassPositions;
    std::vector<GLfloat> grassNormals;
    std::vector<GLfloat> grassTextureCoordinates;
    const std::vector<Vec3> grassPositionsOnPage = {
        {-0.48f, -0.23f, -0.30f},
        {-0.39f, -0.23f, -0.17f},
        {-0.55f, -0.23f, 0.10f},
        {-0.42f, -0.23f, 0.33f},
        {-0.18f, -0.23f, 0.39f},
        {0.19f, -0.23f, 0.40f},
        {0.47f, -0.23f, 0.29f},
        {0.55f, -0.23f, 0.10f},
        {0.49f, -0.23f, -0.28f},
        {0.35f, -0.23f, -0.38f},
        {-0.68f, -0.23f, -0.42f},
        {-0.60f, -0.23f, -0.25f},
        {-0.67f, -0.23f, -0.08f},
        {-0.64f, -0.23f, 0.20f},
        {-0.58f, -0.23f, 0.38f},
        {-0.30f, -0.23f, 0.50f},
        {-0.04f, -0.23f, 0.48f},
        {0.24f, -0.23f, 0.51f},
        {0.53f, -0.23f, 0.43f},
        {0.68f, -0.23f, 0.25f},
        {0.69f, -0.23f, -0.03f},
        {0.67f, -0.23f, -0.22f},
        {0.58f, -0.23f, -0.43f},
        {0.28f, -0.23f, -0.47f},
        {0.02f, -0.23f, -0.46f},
        {-0.23f, -0.23f, -0.47f},
        {-0.47f, -0.23f, -0.44f},
        {-0.51f, -0.23f, 0.28f},
        {-0.28f, -0.23f, 0.37f},
        {0.30f, -0.23f, 0.36f},
        {0.49f, -0.23f, 0.18f}
    };

    for (std::size_t grassIndex = 0;
         grassIndex < grassPositionsOnPage.size();
         ++grassIndex) {
        const float grassHeight =
            0.07f + static_cast<float>(grassIndex % 3) * 0.018f;
        const float grassWidth =
            0.018f + static_cast<float>(grassIndex % 2) * 0.008f;
        addGrassBlade(
            grassPositions,
            grassNormals,
            grassTextureCoordinates,
            grassPositionsOnPage[grassIndex],
            grassHeight,
            grassWidth
        );
    }

    grassMesh_.reset(new Mesh(
        grassPositions,
        grassNormals,
        {0.78f, 0.86f, 0.72f},
        grassTextureCoordinates,
        false
    ));

    // Add a few small dirt mounts to break up the otherwise flat page.
    std::vector<GLfloat> dirtPositions;
    std::vector<GLfloat> dirtNormals;
    std::vector<GLfloat> dirtTextureCoordinates;
    addDirtMount(
        dirtPositions,
        dirtNormals,
        dirtTextureCoordinates,
        {-0.52f, -0.23f, -0.27f},
        0.10f,
        0.035f
    );
    addDirtMount(
        dirtPositions,
        dirtNormals,
        dirtTextureCoordinates,
        {0.49f, -0.23f, -0.28f},
        0.08f,
        0.028f
    );
    addDirtMount(
        dirtPositions,
        dirtNormals,
        dirtTextureCoordinates,
        {-0.38f, -0.23f, 0.34f},
        0.09f,
        0.032f
    );

    dirtMesh_.reset(new Mesh(
        dirtPositions,
        dirtNormals,
        {0.92f, 0.92f, 0.92f},
        dirtTextureCoordinates,
        false
    ));
}

void Landscape::update(float deltaSeconds, const Wind& wind) {
    // update all birds using the current wind field.
    std::uniform_real_distribution<float> flapDistribution(0.0f, 6.2831853f);
    std::uniform_real_distribution<float> reactionDistribution(0.0f, 0.4f);
    for (std::size_t birdIndex = 0; birdIndex < birds_.size(); ++birdIndex) {
        Bird& bird = birds_[birdIndex];
        bird.update(deltaSeconds, wind);
        if (bird.isOutOfScene() && !perchSpots_.empty()) {
            bird.reset(
                perchSpots_[birdIndex % perchSpots_.size()],
                flapDistribution(birdGenerator_),
                reactionDistribution(birdGenerator_)
            );
        }
    }
}

void Landscape::draw(
    const Shader& shader,
    const Mat4& viewProjectionMatrix
) const {
    // The three generated meshes share the same world/camera/lighting setup.
    shader.setMatrix4("combinedTransformationMatrix", viewProjectionMatrix);
    shader.setMatrix4("modelMatrix", identityMatrix());
    treeMesh_->draw(shader);
    dirtMesh_->draw(shader);
    grassMesh_->draw(shader);

    // draw birds after the landscape so their current positions are visible.
    for (const Bird& bird : birds_) {
        bird.draw(shader, viewProjectionMatrix);
    }
}