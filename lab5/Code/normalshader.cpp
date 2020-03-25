#include "normalshader.h"

NormalShader::NormalShader()
{

}

Shader::ShadingMode NormalShader::type()
{
    return ShadingMode::NORMAL;
}

void NormalShader::setUniformData(QMatrix4x4 transformationMatrix
                                  , QMatrix4x4 perspectiveTransformationMatrix
                                  , QMatrix4x4 viewTransformationMatrix
                                  , QMatrix3x3 normalTransformationMatrix)
{
    shaderProgram.setUniformValue(normalTransformationUniformLocation,normalTransformationMatrix);
    shaderProgram.setUniformValue(transformationUniformLocation, transformationMatrix);
    shaderProgram.setUniformValue(viewTransformationUniformLocation, viewTransformationMatrix);
    shaderProgram.setUniformValue(projectionUniformLocation,  perspectiveTransformationMatrix);
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
    viewTransformationUniformLocation = shaderProgram.uniformLocation("viewTransform");
    projectionUniformLocation = shaderProgram.uniformLocation("projection");
    normalTransformationUniformLocation = shaderProgram.uniformLocation("normalTransformation");
}
