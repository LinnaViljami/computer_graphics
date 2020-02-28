#include "phongshader.h"

PhongShader::PhongShader() : glfunc(nullptr)
{

}



void PhongShader::init(QOpenGLFunctions_3_3_Core *glFuncPointer)
{
    glfunc = glFuncPointer;
    createShaderPrograms();
    setUniformLocations();
}

void PhongShader::bind()
{
    shaderProgram.bind();
}

void PhongShader::release()
{
    shaderProgram.release();
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
    transformationUniformLocation = shaderProgram.uniformLocation("transformation");
    projectionUniformLocation = shaderProgram.uniformLocation("projection");
    normalTransformationUniformLocation = shaderProgram.uniformLocation("normalTransformation");
    materialUniformLocation = shaderProgram.uniformLocation("material");
    lightPositionUniformLocation = shaderProgram.uniformLocation("lightPosition");


}

