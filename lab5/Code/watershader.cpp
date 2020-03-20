#include "watershader.h"

WaterShader::WaterShader()
{

}

Shader::ShadingMode WaterShader::type()
{
    return ShadingMode::NORMAL;
}

void WaterShader::setUniformData(QMatrix4x4 transformationMatrix
                                  , QMatrix4x4 perspectiveTransformationMatrix
                                  , QMatrix4x4 viewTransformationMatrix
                                  , QMatrix3x3 normalTransformationMatrix
                                 , float amplitude
                                 , float frequency
                                 , float phase)
{
    shaderProgram.setUniformValue(normalTransformationUniformLocation,normalTransformationMatrix);
    shaderProgram.setUniformValue(transformationUniformLocation, transformationMatrix);
    shaderProgram.setUniformValue(viewTransformationUniformLocation, viewTransformationMatrix);
    shaderProgram.setUniformValue(projectionUniformLocation,  perspectiveTransformationMatrix);
    shaderProgram.setUniformValue(amplitudeUniformLocation, amplitude);
    shaderProgram.setUniformValue(frequencyUniformLocation, frequency);
    shaderProgram.setUniformValue(phaseUniformLocation, phase);
}

void WaterShader::createShaderPrograms()
{
    shaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex,
                                           ":/shaders/vertshader_normal.glsl");
    shaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment,
                                           ":/shaders/fragshader_normal.glsl");
    shaderProgram.link();

}

void WaterShader::setUniformLocations()
{
    transformationUniformLocation = shaderProgram.uniformLocation("transformation");
    viewTransformationUniformLocation = shaderProgram.uniformLocation("viewTransform");
    projectionUniformLocation = shaderProgram.uniformLocation("projection");
    normalTransformationUniformLocation = shaderProgram.uniformLocation("normalTransformation");
    amplitudeUniformLocation = shaderProgram.uniformLocation("amplitude");
    frequencyUniformLocation = shaderProgram.uniformLocation("frequency");
    phaseUniformLocation = shaderProgram.uniformLocation("phase");
}

