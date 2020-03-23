#version 330 core

// Define constants.
#define M_PI 3.141593

// The input from the vertex shader.
in vec3 vertNormal;
in vec3 vertPosition;
in vec3 relativeLightPosition;
in vec2 texCoords;

// Illumination model constants.
uniform vec4 material;
uniform vec3 lightColor;

// Texture sampler.
uniform sampler2D textureSampler;
uniform bool useTextures;
// Specify the output of the fragment shader.
out vec4 fColor;

void main()
{
    // Ambient color does not depend on any vectors.
    vec3 materialColor = vec3(0.5, 0.7, 0.2);
    vec3 texColor = texture(textureSampler, texCoords).xyz;

    vec3 color;
    vec3 colorForCalcs;
    if(useTextures){
        colorForCalcs = texColor;
    }else{
        colorForCalcs = materialColor;
    }
    color = material.x * colorForCalcs;
    // Calculate light direction vectors in the Phong illumination model.
    vec3 lightDirection    = normalize(relativeLightPosition - vertPosition);
    vec3 normal            = normalize(vertNormal);

    // Diffuse color.
    float diffuseIntensity = max(dot(normal, lightDirection), 0.0F);
    color += colorForCalcs * material.y * diffuseIntensity;

    // Specular color.
    vec3 viewDirection      = normalize(-vertPosition); // The camera is always at (0, 0, 0).
    vec3 reflectDirection   = reflect(-lightDirection, normal);
    float specularIntensity = max(dot(reflectDirection, viewDirection), 0.0F);
    color += lightColor * material.z * pow(specularIntensity, material.w);

//    fColor = vec4(color, 1.0F);
    fColor = vec4(texCoords, 0.0f, 1.0F);
}
