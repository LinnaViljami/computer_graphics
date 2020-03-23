#version 330 core

// Define constants
#define M_PI 3.141593

// Specify the inputs to the fragment shader
// These must have the same type and name!
in vec3 vertNormal;
in vec2 uvCoordinates;
in vec3 relativeLightPosition;
in vec3 vertPosition;
in float finalHeight;

// Illumination model constants.
uniform vec4 material;
uniform vec3 lightColor;

// Specify the Uniforms of the fragment shaders
// uniform vec3 lightPosition; // for example

// Specify the output of the fragment shader
// Usually a vec4 describing a color (Red, Green, Blue, Alpha/Transparency)
out vec4 fColor;

void main()
{
    float smoothingValue = smoothstep(-1, 1, finalHeight);
    vec3 blueColor = vec3(0.1, 0.1, 1.0);
    vec3 whiteColor = vec3(0.7, 0.7, 0.7);
    vec3 materialColor = mix(blueColor, whiteColor, smoothingValue);
    vec3 color;
    vec3 colorForCalcs;

    colorForCalcs = materialColor;

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


    fColor = vec4(color, 1.0F);


//    vec3 normalizedVertNormal = normalize(vertNormal);
//    vec3 colorMapping = vec3(0.5,0.5,0.5);
//    vec3 mappedColors = colorMapping*normalizedVertNormal + colorMapping;
//    //fColor = vec4(uvCoordinates.x,uvCoordinates.y, 0.0, 1.0);
//    fColor = vec4(mappedColors, 1.0);
}
