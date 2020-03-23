#version 330 core

// Define constants
#define M_PI 3.141593

// Specify the input locations of attributes
layout (location = 0) in vec3 vertCoordinates_in;
layout (location = 1) in vec3 vertNormal_in;
layout (location = 2) in vec2 textureCoords_in;
// Specify the Uniforms of the vertex shader.
// WARNING make sure to update the setters of uniform locations in the shader_x.cpp files
// when changing any of these field names;
uniform mat4 transformation;
uniform mat4 projection;
uniform mat3 normalTransformation;
uniform vec3 material;              // ka, kd, ks
uniform vec3 lightPosition;         // x, y, z
uniform vec3 lightColor;            // r, g, b

// x, y, z
vec3 viewAngle = vec3(0, 0, -1);

// Power used for specular component.
uniform float phongExponent;

// Specify the output of the vertex stage
out vec3 illuminationComponent;
out vec2 textureCoords;

vec3 calculateNormalizedVector(vec3 from, vec3 to) {
    vec3 result = to - from;
    return normalize(result);
}

vec3 calculateAmbientComponent() {
    float ka = material[0];
    float Ia = 0.1;
    return Ia * ka * lightColor;
}

vec3 calculateDiffuseComponent(vec3 orientedNormal, vec3 lightVector) {
    float kd = material[1];

    float diffuseComponent = dot(orientedNormal, lightVector);
    if (diffuseComponent < 0) {
        diffuseComponent = 0;
    }
    
    vec3 diffuseColor = diffuseComponent * kd * lightColor;

    return diffuseColor;
}

vec3 calculateSpecularComponent(vec3 orientedNormal, vec3 lightVector) {
    float ks = material[2];

    vec3 R = 2 * (dot(orientedNormal, lightVector) * orientedNormal - lightVector);

    float specularComponent = dot(R, viewAngle);
    if (specularComponent < 0) {
        specularComponent = 0;
    }

    specularComponent = pow(specularComponent, phongExponent);
    vec3 specularColor = specularComponent * ks * lightColor;

    return specularColor;
}

vec3 getPhongIllumination(vec3 orientedNormal) {
    vec3 lightVector = calculateNormalizedVector(vertCoordinates_in, lightPosition);

    vec3 ambient = calculateAmbientComponent();
    vec3 diffuse = calculateDiffuseComponent(orientedNormal, lightVector);;
    vec3 specular = calculateSpecularComponent(orientedNormal, lightVector);

    vec3 color = (ambient+diffuse+specular);
    return color;
}

void main()
{
    // gl_Position is the output (a vec4) of the vertex shader
    gl_Position = projection * transformation * vec4(vertCoordinates_in, 1.0);
    textureCoords = textureCoords_in;
    vec3 orientedNormal = normalTransformation * vertNormal_in;

    illuminationComponent = getPhongIllumination(normalize(orientedNormal));
}
