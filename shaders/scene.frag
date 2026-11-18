#version 330 core

in vec3 transformedNormal;
in vec3 transformedWorldPosition;
in vec2 transformedTextureCoordinate;

uniform vec3 lightDirection;
uniform vec3 lightPosition;
uniform vec3 materialColor;
uniform sampler2D notebookTexture;
uniform int usesTexture;

out vec4 fragmentColor;

void main() {
    // Start with a little ambient light so the unlit side is not pitch black.
    const float ambientBrightness = 0.18;
    const float innerCutoff = 0.95106;
    const float outerCutoff = 0.86603;
    const float attenuationConstant = 1.0;
    const float attenuationLinear = 0.09;
    const float attenuationQuadratic = 0.032;

    // Work out how the current pixel sits relative to the spotlight.
    vec3 fragDirectionToLight =
        normalize(lightPosition - transformedWorldPosition);
    vec3 fragDirectionFromLight =
        normalize(transformedWorldPosition - lightPosition);

    float spotlightOnFrag =
        dot(fragDirectionFromLight, normalize(lightDirection));
    float spotlightIntensity = smoothstep(outerCutoff, innerCutoff, spotlightOnFrag);

    vec3 normalizedNormal = normalize(transformedNormal);
    // Lambert diffuse lighting: surfaces facing the light get brighter.
    float diffuseBrightness =
        max(dot(normalizedNormal, fragDirectionToLight), 0.0);

    float distanceToLight = length(lightPosition - transformedWorldPosition);
    // The light fades with distance instead of staying equally bright.
    float attenuation =
        1.0 / (
            attenuationConstant +
            attenuationLinear * distanceToLight +
            attenuationQuadratic * distanceToLight * distanceToLight
        );

    float spotlightDiffuseBrightness =
        diffuseBrightness * spotlightIntensity * attenuation * 2.5;

    float totalBrightness =
        min(ambientBrightness + spotlightDiffuseBrightness, 1.0);

    
    // Most objects use a flat material colour; notebook pages use the image.
    vec3 surfaceColor = materialColor;
    if (usesTexture == 1) {
        surfaceColor = texture(notebookTexture, transformedTextureCoordinate).rgb;
    }

    fragmentColor = vec4(surfaceColor * totalBrightness, 1.0);
}