#version 330 core

// Define constants
#define M_PI 3.141593

// Specify the inputs to the fragment shader
// These must have the same type and name!
in vec3 vertNormal;

//texture loading
in vec2 textureCoords;
uniform sampler2D textureUniform;
uniform bool useTextures;

// Specify the Uniforms of the fragment shader.
// WARNING make sure to update the setters of uniform locations in the shader_x.cpp files
// when changing any of these field names;
uniform vec3 material;              // ka, kd, ks
uniform vec3 materialColor;         // r, g, b
uniform vec3 lightPosition;         // x, y, z
uniform vec3 lightColor;            // r, g, b

// x, y, z
vec3 viewAngle = vec3(0, 0, -1);

// Power used for specular component.
uniform float phongExponent;

// Specify the output of the fragment shader
// Usually a vec4 describing a color (Red, Green, Blue, Alpha/Transparency)
out vec4 fColor;

vec3 calculateNormalizedVector(vec3 from, vec3 to) {
    vec3 result = to - from;
    return normalize(result);
}

vec3 calculateAmbientComponent(vec3 textureColor) {
    float ka = material[0];
    float Ia = 1.0;
    return Ia * ka * textureColor * lightColor;
}

vec3 calculateDiffuseColor(vec3 orientedNormal, vec3 lightVector, vec3 textureColor) {
    float kd = material[1];

    float diffuseComponent = dot(orientedNormal, lightVector);
    if (diffuseComponent < 0) {
        diffuseComponent = 0;
    }

    vec3 diffuseColor = diffuseComponent * kd * textureColor * lightColor;
    return diffuseColor;
}

vec3 calculateSpecularComponent(vec3 orientedNormal, vec3 lightVector, vec3 textureColor) {
    float ks = material[2];

    vec3 R = 2 * (dot(orientedNormal, lightVector) * orientedNormal - lightVector);

    float specularComponent = dot(R, viewAngle);
    if (specularComponent < 0) {
        specularComponent = 0;
    }

    specularComponent = pow(specularComponent, phongExponent);
    vec3 specularColor = specularComponent * ks * textureColor * lightColor;

    return specularColor;
}

vec3 getPhongColor(vec3 orientedNormal, vec3 textureColor) {
    vec3 currentPosition = vec3(
                gl_FragCoord.x,
                gl_FragCoord.y,
                gl_FragCoord.z);
    vec3 lightVector = calculateNormalizedVector(currentPosition, lightPosition);

    vec3 color = calculateAmbientComponent(textureColor);
    color += calculateDiffuseColor(orientedNormal, lightVector, textureColor);
    color += calculateSpecularComponent(orientedNormal, lightVector, textureColor);

    return color;
}

void main()
{
    if(useTextures){
        vec4 textureColor = texture(textureUniform, textureCoords);
        vec3 color = getPhongColor(normalize(vertNormal), textureColor.xyz);
        fColor = vec4(color, 1.0);
    }
    else{
        vec3 color = getPhongColor(normalize(vertNormal), materialColor);
        fColor = vec4(color, 1.0);
    }

}
