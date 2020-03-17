#include "watershader.h"

WaterShader::WaterShader()
{

}

Shader::ShadingMode WaterShader::type()
{
    return ShadingMode::WATER;

}

void WaterShader::setUniformData(QMatrix4x4 transformationMatrix,
                                   QMatrix4x4 perspectiveTransformationMatrix,
                                   QMatrix3x3 normalTransformationMatrix,
                                   QVector3D material,
                                   QVector3D materialColor,
                                   QVector3D lightPosition,
                                   QVector3D lightColor,
                                   float phongExponent,
                                   bool useTextures)
{
    shaderProgram.setUniformValue(normalTransformationUniformLocation,normalTransformationMatrix);
    shaderProgram.setUniformValue(transformationUniformLocation, transformationMatrix);
    shaderProgram.setUniformValue(projectionUniformLocation,  perspectiveTransformationMatrix);
    shaderProgram.setUniformValue(lightPositionUniformLocation, lightPosition);
    shaderProgram.setUniformValue(materialUniformLocation, material);
    shaderProgram.setUniformValue(materialColorUniformLocation, materialColor);
    shaderProgram.setUniformValue(lightColorUniformLocation, lightColor);
    shaderProgram.setUniformValue(phongExponentUniformLocation, phongExponent);
    shaderProgram.setUniformValue(useTexturesUniformLocation, useTextures);
}

GLint *WaterShader::getTextureBufferLocation()
{
    return &textureUniformLocation;
}

void WaterShader::createShaderPrograms()
{
    shaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex,
                                           ":/shaders/vertshader_water.glsl");
    shaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment,
                                           ":/shaders/fragshader_water.glsl");
    shaderProgram.link();
}

void WaterShader::setUniformLocations()
{
    transformationUniformLocation = shaderProgram.uniformLocation("modelViewTransform");
    viewTransformationUniformLocation = shaderProgram.uniformLocation("viewTransform");
    projectionUniformLocation = shaderProgram.uniformLocation("projectionTransform");
    normalTransformationUniformLocation = shaderProgram.uniformLocation("normalTransform");
    materialUniformLocation = shaderProgram.uniformLocation("material");
    lightPositionUniformLocation = shaderProgram.uniformLocation("lightPosition");
    lightColorUniformLocation = shaderProgram.uniformLocation("lightColor");
    textureUniformLocation = shaderProgram.uniformLocation("textureSampler");
    useTexturesUniformLocation = shaderProgram.uniformLocation("useTextures");
}
