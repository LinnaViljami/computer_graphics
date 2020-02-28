#include "gouraudshader.h"

GouraudShader::GouraudShader()
{

}

ShadingMode GouraudShader::type()
{
    return ShadingMode::GOURAUD;

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
