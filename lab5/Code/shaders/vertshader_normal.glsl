#version 330 core

// Define constants
#define M_PI 3.141593

// Specify the input locations of attributes
layout (location = 0) in vec3 vertCoordinates_in;
layout (location = 1) in vec3 vertNormal_in;

// Specify the Uniforms of the vertex shader
uniform mat4 transformation;
uniform mat4 projection;
uniform mat4 viewTransform;
uniform mat3 normalTransformation;

// Specify the output of the vertex stage
out vec3 vertNormal;

void main()
{
    // gl_Position is the output (a vec4) of the vertex shader
    gl_Position = projection * viewTransform * transformation * vec4(vertCoordinates_in, 1.0);
    vertNormal = normalTransformation * vertNormal_in;
}
