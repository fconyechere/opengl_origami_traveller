#include "Model.h"
#include "Shader.h"

#include <assimp/Importer.hpp>
#include <assimp/material.h>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include <cmath>
#include <stdexcept>

namespace {

Vec3 getMaterialColor(const aiScene* scene, const aiMesh* mesh) {
    if (scene->mNumMaterials == 0 || mesh->mMaterialIndex >= scene->mNumMaterials) {
        return {0.8f, 0.8f, 0.8f};
    }

    aiColor3D diffuseColor(0.8f, 0.8f, 0.8f);
    const aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
    if (material->Get(AI_MATKEY_COLOR_DIFFUSE, diffuseColor) != AI_SUCCESS) {
        return {0.8f, 0.8f, 0.8f};
    }

    return {diffuseColor.r, diffuseColor.g, diffuseColor.b};
}

bool usesNotebookTexture(const aiScene* scene, const aiMesh* mesh) {
    if (scene->mNumMaterials == 0 || mesh->mMaterialIndex >= scene->mNumMaterials) {
        return false;
    }

    aiString materialName;
    scene->mMaterials[mesh->mMaterialIndex]->Get(AI_MATKEY_NAME, materialName);
    return std::string(materialName.C_Str()) == "NotebookPages";
}

}

void Model::loadFromFile(const std::string& path) {
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(
        path,
        aiProcess_Triangulate | aiProcess_GenSmoothNormals
    );

    if (scene == nullptr || (scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) != 0 ||
        scene->mRootNode == nullptr) {
        throw std::runtime_error(
            "Could not load model '" + path + "': " + importer.GetErrorString()
        );
    }

    for (unsigned int meshIndex = 0; meshIndex < scene->mNumMeshes; ++meshIndex) {
        const aiMesh* importedMesh = scene->mMeshes[meshIndex];
        std::vector<GLfloat> positions;
        std::vector<GLfloat> normals;
        std::vector<GLfloat> textureCoordinates;

        positions.reserve(importedMesh->mNumFaces * 9);
        normals.reserve(importedMesh->mNumFaces * 9);
        textureCoordinates.reserve(importedMesh->mNumFaces * 6);

        const bool meshUsesNotebookTexture =
            usesNotebookTexture(scene, importedMesh);

        for (unsigned int faceIndex = 0; faceIndex < importedMesh->mNumFaces; ++faceIndex) {
            const aiFace& face = importedMesh->mFaces[faceIndex];
            for (unsigned int faceVertexIndex = 0;
                 faceVertexIndex < face.mNumIndices;
                 ++faceVertexIndex) {
                const unsigned int vertexIndex = face.mIndices[faceVertexIndex];
                const aiVector3D& position = importedMesh->mVertices[vertexIndex];
                const aiVector3D& normal = importedMesh->mNormals[vertexIndex];

                positions.push_back(position.x);
                positions.push_back(position.y);
                positions.push_back(position.z);
                normals.push_back(normal.x);
                normals.push_back(normal.y);
                normals.push_back(normal.z);

                const float textureCoordinateU =
                    (position.x + 0.78f) / 1.56f;
                const float textureCoordinateV =
                    (position.z + 0.55f) / 1.10f;
                textureCoordinates.push_back(textureCoordinateU);
                textureCoordinates.push_back(textureCoordinateV);
            }
        }

        meshes_.emplace_back(
            positions,
            normals,
            getMaterialColor(scene, importedMesh),
            textureCoordinates,
            meshUsesNotebookTexture
        );
    }
}

void Model::loadCharacterFromFile(const std::string& path) {
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(
        path,
        aiProcess_Triangulate | aiProcess_GenSmoothNormals
    );

    if (scene == nullptr || (scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE) != 0 ||
        scene->mRootNode == nullptr) {
        throw std::runtime_error(
            "Could not load character '" + path + "': " +
            importer.GetErrorString()
        );
    }

    const Vec3 characterPaperColor = {0.92f, 0.92f, 0.92f};

    for (unsigned int meshIndex = 0; meshIndex < scene->mNumMeshes; ++meshIndex) {
        const aiMesh* importedMesh = scene->mMeshes[meshIndex];
        std::array<std::vector<GLfloat>, static_cast<std::size_t>(CharacterPart::Count)>
            partPositions;
        std::array<std::vector<GLfloat>, static_cast<std::size_t>(CharacterPart::Count)>
            partNormals;
        std::array<std::vector<GLfloat>, static_cast<std::size_t>(CharacterPart::Count)>
            partTextureCoordinates;

        for (unsigned int faceIndex = 0; faceIndex < importedMesh->mNumFaces; ++faceIndex) {
            const aiFace& face = importedMesh->mFaces[faceIndex];
            if (face.mNumIndices != 3) {
                continue;
            }

            Vec3 faceCentre = {0.0f, 0.0f, 0.0f};
            for (unsigned int faceVertexIndex = 0;
                 faceVertexIndex < face.mNumIndices;
                 ++faceVertexIndex) {
                const unsigned int vertexIndex = face.mIndices[faceVertexIndex];
                const aiVector3D& position = importedMesh->mVertices[vertexIndex];
                faceCentre.x += position.x / 3.0f;
                faceCentre.y += position.y / 3.0f;
                faceCentre.z += position.z / 3.0f;
            }

            CharacterPart characterPart = CharacterPart::Body;
            const float distanceFromCentre = std::abs(faceCentre.x);

            if (faceCentre.y > 0.70f && distanceFromCentre < 0.20f) {
                characterPart = CharacterPart::Face;
            } else if (faceCentre.y < 0.10f) {
                characterPart = faceCentre.x < 0.0f
                    ? CharacterPart::LeftFoot
                    : CharacterPart::RightFoot;
            } else if (faceCentre.y < 0.20f) {
                characterPart = faceCentre.x < 0.0f
                    ? CharacterPart::LeftCalf
                    : CharacterPart::RightCalf;
            } else if (faceCentre.y < 0.32f) {
                characterPart = faceCentre.x < 0.0f
                    ? CharacterPart::LeftLeg
                    : CharacterPart::RightLeg;
            } else if (distanceFromCentre > 0.30f &&
                       faceCentre.y > 0.48f) {
                characterPart = faceCentre.x < 0.0f
                    ? CharacterPart::LeftHand
                    : CharacterPart::RightHand;
            } else if (distanceFromCentre > 0.18f &&
                       faceCentre.y > 0.32f) {
                characterPart = faceCentre.x < 0.0f
                    ? CharacterPart::LeftArm
                    : CharacterPart::RightArm;
            }

            const std::size_t partIndex =
                static_cast<std::size_t>(characterPart);
            std::vector<GLfloat>& positions = partPositions[partIndex];
            std::vector<GLfloat>& normals = partNormals[partIndex];
            std::vector<GLfloat>& textureCoordinates =
                partTextureCoordinates[partIndex];

            for (unsigned int faceVertexIndex = 0;
                 faceVertexIndex < face.mNumIndices;
                 ++faceVertexIndex) {
                const unsigned int vertexIndex = face.mIndices[faceVertexIndex];
                const aiVector3D& position = importedMesh->mVertices[vertexIndex];
                const aiVector3D& normal = importedMesh->mNormals[vertexIndex];

                positions.push_back(position.x);
                positions.push_back(position.y);
                positions.push_back(position.z);
                normals.push_back(normal.x);
                normals.push_back(normal.y);
                normals.push_back(normal.z);
                textureCoordinates.push_back(0.0f);
                textureCoordinates.push_back(0.0f);
            }
        }

        for (std::size_t partIndex = 0;
             partIndex < static_cast<std::size_t>(CharacterPart::Count);
             ++partIndex) {
            if (!partPositions[partIndex].empty()) {
                characterPartMeshes_[partIndex].emplace_back(
                    partPositions[partIndex],
                    partNormals[partIndex],
                    characterPaperColor,
                    partTextureCoordinates[partIndex],
                    false
                );
            }
        }
    }
}

void Model::draw(const Shader& shader) const {
    for (const Mesh& mesh : meshes_) {
        mesh.draw(shader);
    }
}

void Model::draw(const Shader& shader, const Mat4& modelMatrix) const {
    shader.setMatrix4("modelMatrix", modelMatrix);
    for (const Mesh& mesh : meshes_) {
        mesh.draw(shader);
    }
}

void Model::drawCharacter(const Shader& shader, const Mat4& viewProjectionMatrix, const std::array<Mat4, static_cast<std::size_t>(CharacterPart::Count)>& partModelMatrices) const {
    for (std::size_t partIndex = 0;
         partIndex < static_cast<std::size_t>(CharacterPart::Count);
         ++partIndex) {
        shader.setMatrix4(
            "combinedTransformationMatrix",
            multiply(viewProjectionMatrix, partModelMatrices[partIndex])
        );
        shader.setMatrix4("modelMatrix", partModelMatrices[partIndex]);
        for (const Mesh& mesh : characterPartMeshes_[partIndex]) {
            mesh.draw(shader);
        }
    }
}
