#version 330 core

// Define constants
const float M_PI = 3.141592f;
const int MAX_WAVES = 10;

// Specify the input locations of attributes
layout (location = 0) in vec3 vertCoordinates_in;
layout (location = 1) in vec3 vertNormal_in;

// Specify the Uniforms of the vertex shader
uniform mat4 transformation;
uniform mat4 projection;
uniform mat4 viewTransform;
uniform vec3 lightPosition;
uniform mat3 normalTransformation;



uniform float amplitudes[MAX_WAVES];
uniform float frequencies[MAX_WAVES];
uniform float phase;

// Specify the output of the vertex stage
out vec3 vertNormal;
out vec2 uvCoordinates;
out vec3 vertPosition;
out vec3 relativeLightPosition;
out float finalHeight;


float waveHeight(int waveIdx, float u){
    return amplitudes[waveIdx] * sin(2.0f * 3.141592f * frequencies[waveIdx] * u + phase );
}

float waveDU(int waveIdx, float u){
    return 2.0f * M_PI * frequencies[waveIdx] * amplitudes[waveIdx] * cos(2.0f * M_PI * frequencies[waveIdx] * u + phase );
}

void main()
{
    uvCoordinates = vec2(vertCoordinates_in.x, vertCoordinates_in.y);

    // construct wawes
    float amplitude = 0.0f;
    float derivate = 0.0f;

    for(int i = 0; i<MAX_WAVES; i++){
        if(amplitudes[i] != 0){
            amplitude = amplitude + waveHeight(i,vertCoordinates_in.x);
            derivate = derivate + waveDU(i,vertCoordinates_in.x);
        }

    }
    vec3 waveCoordinates = vec3(vertCoordinates_in.x, vertCoordinates_in.y,amplitude);
    vec3 waveNormal = vec3(-derivate,-0.0f,1.0f);

    gl_Position = projection * viewTransform * transformation * vec4(waveCoordinates ,  1.0F);

    relativeLightPosition = vec3(transformation * vec4(lightPosition, 1.0F));
    vertPosition = vec3(transformation * vec4(waveCoordinates ,  1.0F));
    vertNormal = normalize(normalTransformation * waveNormal);
    finalHeight = amplitude;
}
