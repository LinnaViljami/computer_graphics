#version 330 core

// Define constants
#define M_PI 3.141593

// Specify the input locations of attributes
layout (location = 0) in vec3 vertCoordinates_in;
layout (location = 1) in vec3 vertNormal_in;

// Specify the Uniforms of the vertex shader
uniform mat4 transformation;
uniform mat4 projection;
uniform mat3 normalTransformation;

// ka, kd, ks
//uniform vec3 material;
vec3 material = vec3(0.1f, 0.7f, 0.15f); // TODO remove this and use uniform

// x, y, z
//uniform vec3 lightPosition;
vec3 lightPosition = vec3( 2.0f, // TODO remove this and use uniform
                           6.0f,
                           8.0f);

// r, g, b
// TODO make uniform out of this to change colors?
vec3 lightColor = vec3(1, 1, 1);
vec3 materialColor = vec3(0.25f, 0.25f, 0.25f);

// x, y, z
vec3 viewAngle = vec3(0, 0, -1);

// Power used for specular component. TODO should this value be fetched from somewhere else?
float n = 2.0;

// Specify the output of the vertex stage
out vec3 vertColor;

vec3 calculateNormalizedVector(vec3 from, vec3 to) {
    vec3 result = to - from;
    return normalize(result);
}

vec3 calculateAmbientComponent(vec3 materialColor) {
    float ka = material[0];
    float Ia = 0.1;
    return Ia * ka * materialColor;
}

vec3 calculateDiffuseColor(vec3 orientedNormal, vec3 lightVector, vec3 materialColor) {
    float kd = material[1];

    float diffuseComponent = dot(orientedNormal, lightVector);
    diffuseComponent = normalize(diffuseComponent);
    if (diffuseComponent < 0) {
        diffuseComponent = 0;
    }
    
    vec3 diffuseColor = diffuseComponent * kd * lightColor * materialColor;

    return diffuseColor;
}

vec3 calculateSpecularComponent(vec3 orientedNormal, vec3 lightVector) {
    float ks = material[2];

    vec3 R = 2 * (dot(orientedNormal, lightVector) * orientedNormal - lightVector);

    float specularComponent = dot(R, viewAngle);
    if (specularComponent < 0) {
        specularComponent = 0;
    }

    specularComponent = pow(specularComponent, n);
    vec3 specularColor = specularComponent * ks * lightColor;

    return specularColor;
}

vec3 getPhongColor(vec3 orientedNormal) {
    vec3 lightVector = calculateNormalizedVector(vertCoordinates_in, lightPosition);

    vec3 color = calculateAmbientComponent(materialColor);
    color += calculateDiffuseColor(orientedNormal, lightVector, materialColor);;
    color += calculateSpecularComponent(orientedNormal, lightVector);

    return color;
}

void main()
{
    // gl_Position is the output (a vec4) of the vertex shader
    gl_Position = projection * transformation * vec4(vertCoordinates_in, 1.0);

    vec3 orientedNormal = normalTransformation * vertNormal_in;

    vertColor = getPhongColor(orientedNormal);
}
