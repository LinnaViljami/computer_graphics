#include "phongshader.h"

PhongShader::PhongShader()
{

}



//void PhongShader::init(QOpenGLFunctions_3_3_Core *glFuncPointer)
//{
//    glfunc = glFuncPointer;
//    createShaderPrograms();
//    setUniformLocations();
//}

//void PhongShader::bind()
//{
//    shaderProgram.bind();
//}

//void PhongShader::release()
//{
//    shaderProgram.release();
//}

Shader::ShadingMode PhongShader::type()
{
    return ShadingMode::PHONG;
}

void PhongShader::setUniformData(QMatrix4x4 transformationMatrix,
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

