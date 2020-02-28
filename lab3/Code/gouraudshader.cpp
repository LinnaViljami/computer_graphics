#include "gouraudshader.h"

GouraudShader::GouraudShader()
{

}

Shader::ShadingMode GouraudShader::type()
{
    return ShadingMode::GOURAUD;

}

void GouraudShader::setUniformData(QMatrix4x4 transformationMatrix,
                                   QMatrix4x4 perspectiveTransformationMatrix,
                                   QMatrix3x3 normalTransformationMatrix,
                                   QVector3D material,
                                   QVector3D lightPosition)
{
    shaderProgram.setUniformValue(normalTransformationUniformLocation,normalTransformationMatrix);
    shaderProgram.setUniformValue(transformationUniformLocation, transformationMatrix);
    shaderProgram.setUniformValue(projectionUniformLocation,  perspectiveTransformationMatrix);
    shaderProgram.setUniformValue(lightPositionUniformLocation, lightPosition);
    shaderProgram.setUniformValue(materialUniformLocation, material);

}

void GouraudShader::createShaderPrograms()
{
    shaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex,
                                           ":/shaders/vertshader_gouraud.glsl");
    shaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment,
                                           ":/shaders/fragshader_gouraud.glsl");
    shaderProgram.link();

}

void GouraudShader::setUniformLocations()
{

    transformationUniformLocation = shaderProgram.uniformLocation("transformation");
    projectionUniformLocation = shaderProgram.uniformLocation("projection");
    normalTransformationUniformLocation = shaderProgram.uniformLocation("normalTransformation");
    materialUniformLocation = shaderProgram.uniformLocation("material");
    lightPositionUniformLocation = shaderProgram.uniformLocation("lightPosition");

}
