#include "watershader.h"

WaterShader::WaterShader()
{

}

Shader::ShadingMode WaterShader::type()
{
    return ShadingMode::WATER;

}

void WaterShader::setUniformData(QMatrix4x4 transformationMatrix,
                                 QMatrix4x4 viewTransformationMatrix,
                                   QMatrix4x4 perspectiveTransformationMatrix,
                                   QMatrix3x3 normalTransformationMatrix,
                                   QVector4D material,
                                   QVector3D lightPosition,
                                   QVector3D lightColor,
                                   bool useTextures)
{
    shaderProgram.setUniformValue(normalTransformationUniformLocation,normalTransformationMatrix);
    shaderProgram.setUniformValue(viewTransformationUniformLocation, viewTransformationMatrix);
    shaderProgram.setUniformValue(transformationUniformLocation, transformationMatrix);
    shaderProgram.setUniformValue(projectionUniformLocation,  perspectiveTransformationMatrix);
    shaderProgram.setUniformValue(lightPositionUniformLocation, lightPosition);
    shaderProgram.setUniformValue(materialUniformLocation, material);
    shaderProgram.setUniformValue(lightColorUniformLocation, lightColor);
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
    viewTransformationUniformLocation = shaderProgram.uniformLocation("viewTransformation");
    projectionUniformLocation = shaderProgram.uniformLocation("projectionTransform");
    normalTransformationUniformLocation = shaderProgram.uniformLocation("normalTransform");
    materialUniformLocation = shaderProgram.uniformLocation("material");
    lightPositionUniformLocation = shaderProgram.uniformLocation("lightPosition");
    lightColorUniformLocation = shaderProgram.uniformLocation("lightColor");
    textureUniformLocation = shaderProgram.uniformLocation("texturesampler");
    useTexturesUniformLocation = shaderProgram.uniformLocation("useTextures");
}
