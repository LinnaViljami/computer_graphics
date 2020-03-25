#include "phongshader.h"

PhongShader::PhongShader()
{

}

Shader::ShadingMode PhongShader::type()
{
    return ShadingMode::PHONG;
}

void PhongShader::setUniformData(QMatrix4x4 transformationMatrix,
                                 QMatrix4x4 viewTransformationMatrix,
                                 QMatrix4x4 perspectiveTransformationMatrix,
                                 QMatrix3x3 normalTransformationMatrix,
                                 QVector4D material,
                                 QVector3D lightPosition,
                                 QVector3D lightColor,
                                 bool useTextures
                                 )
{
    shaderProgram.setUniformValue(normalTransformationUniformLocation,normalTransformationMatrix);
    shaderProgram.setUniformValue(transformationUniformLocation, transformationMatrix);
    shaderProgram.setUniformValue(viewTransformationUniformLocation, viewTransformationMatrix);
    shaderProgram.setUniformValue(projectionUniformLocation,  perspectiveTransformationMatrix);
    shaderProgram.setUniformValue(lightPositionUniformLocation, lightPosition);
    shaderProgram.setUniformValue(materialUniformLocation, material);
    shaderProgram.setUniformValue(lightColorUniformLocation, lightColor);
    shaderProgram.setUniformValue(useTexturesUniformLocation, useTextures);
}

GLint *PhongShader::getTextureBufferLocation()
{
    return &textureUniformLocation;
}


void PhongShader::createShaderPrograms()
{
    shaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex,
                                           ":/shaders/vertshader_phong.glsl");
    shaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment,
                                           ":/shaders/fragshader_phong.glsl");

    shaderProgram.link();
}

void PhongShader::setUniformLocations()
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

