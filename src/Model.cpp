#include "Model.h"
#include "Shader.h"

#include <assimp/Importer.hpp>
#include <assimp/material.h>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

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

void Model::draw(const Shader& shader) const {
    for (const Mesh& mesh : meshes_) {
        mesh.draw(shader);
    }
}
