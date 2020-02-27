#version 330 core

// Define constants
#define M_PI 3.141593

// Specify the inputs to the fragment shader
// These must have the same type and name!
in vec3 vertNormal;

// Specify the Uniforms of the fragment shaders
// uniform vec3 lightPosition; // for example

// Specify the output of the fragment shader
// Usually a vec4 describing a color (Red, Green, Blue, Alpha/Transparency)
out vec4 fColor;

void main()
{
    vec3 normalizedVertNormal = normalize(vertNormal);
    vec3 colorMapping = vec3(0.5,0.5,0.5);
    vec3 mappedColors = (colorMapping*normalizedVertNormal + colorMapping) / 2;
    fColor = vec4(mappedColors, 1.0);
}
