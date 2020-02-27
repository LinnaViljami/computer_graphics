#version 330 core

// Define constants
#define M_PI 3.141593

// Specify the inputs to the fragment shader
// These must have the same type and name!
in vec3 vertNormal;

// ka, kd, ks
uniform vec3 material;

// x, y, z
uniform mat3 lightPosition;

// r, g, b
// TODO make uniform out of this to change colors?
vec3 lightColor = (1, 1, 1);
vec3 materialColor = (1, 0, 0);

// x, y, z
vec3 viewAngle = (0, 0, -1); // or (0, 0, 0)?

// Power used for specular component. TODO should this value be fetched from somewhere else?
int n = 5;

// Specify the output of the fragment shader
// Usually a vec4 describing a color (Red, Green, Blue, Alpha/Transparency)
out vec4 fColor;

vec3 calculateNormalizedVector(vec3 from, vec3 to) {
    vec3 result = to - from;
    return normalize(result);
}

vec3 calculateAmbientComponent(vec3 materialColor) {
    float ka = material[0];
    float Ia = 1.0;
    return Ia * ka * materialColor;
}

vec3 calculateDiffuseComponent(vec3 orientedNormal, vec3 lightVector, vec3 materialColor) {
    float kd = material[1];

    double diffuseComponent = dot(orientedNormal, lightVector);
    if (diffuseComponent < 0) {
        diffuseComponent = 0;
    }

    vec3 diffuseColor = diffuseComponent * kd * lightColor * materialColor;

    return diffuseColor;
}

vec3 calculateSpecularComponent(vec3 orientedNormal, vec3 lightVector) {
    float ks = material[2];

    vec3 R = 2 * (dot(orientedNormal, lightVector) * orientedNormal - lightVector);

    double specularComponent = dot(R, viewAngle);
    if (specularComponent < 0) {
        specularComponent = 0;
    }

    double specularComponent = pow(specularComponent, n);
    vec3 specularColor = specularComponent * ks * lightColor;

    return specularColor;
}

vec3 getPhongColor(vec3 orientedNormal) {
    // glFragCoord is the coordinate of the current pixel.
    vec3 lightVector = calculateNormalizedVector(gl_FragCoord, lightPosition);

    vec3 color = calculateAmbientComponent(materialColor);
    color += calculateDiffuseComponent(orientedNormal, lightVector, materialColor);
    color += calculateSpecularComponent(orientedNormal, lightVector);
    return color;
}

void main()
{
    vec3 normalizedVertNormal = normalize(vertNormal);
    vec3 colorMapping = vec3(0.5,0.5,0.5);
    vec3 mappedColors = colorMapping*normalizedVertNormal + colorMapping;
    fColor = vec4(mappedColors, 1.0);
}
