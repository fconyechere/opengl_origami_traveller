#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 textureCoordinate;

uniform mat4 combinedTransformationMatrix;
uniform mat4 modelMatrix;

out vec3 transformedNormal;
out vec3 transformedWorldPosition;
out vec2 transformedTextureCoordinate;

void main() {
    gl_Position = combinedTransformationMatrix * vec4(position, 1.0);
    transformedNormal = normalize(mat3(modelMatrix) * normal);

    vec4 worldPosition = modelMatrix * vec4(position, 1.0);

    transformedWorldPosition = worldPosition.xyz;
    transformedTextureCoordinate = textureCoordinate;
}