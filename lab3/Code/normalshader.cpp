#include "normalshader.h"

NormalShader::NormalShader()
{

}

ShadingMode NormalShader::type()
{
    return ShadingMode::NORMAL;
}

void NormalShader::createShaderPrograms()
{
    shaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex,
                                           ":/shaders/vertshader_normal.glsl");
    shaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment,
                                           ":/shaders/fragshader_normal.glsl");
    shaderProgram.link();

}

void NormalShader::setUniformLocations()
{
    transformationUniformLocation = shaderProgram.uniformLocation("transformation");
    projectionUniformLocation = shaderProgram.uniformLocation("projection");
    normalTransformationUniformLocation = shaderProgram.uniformLocation("normalTransformation");
    materialUniformLocation = shaderProgram.uniformLocation("material");
    lightPositionUniformLocation = shaderProgram.uniformLocation("lightPosition");
}
