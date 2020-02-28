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

ShadingMode PhongShader::type()
{
    return ShadingMode::PHONG;
}

void PhongShader::setUniformData(QMatrix3x3 normalTransformationMatrix
                                 , QMatrix4x4 transformationMatrix
                                 , QVector3D material
                                 , QMatrix4x4 perspectiveTransformationMatrix
                                 , QVector3D lightPosition)
{
//    QOpenGLFunctions_3_3_Core * eglfunc = getGlfuncPtr();
    shaderProgram.setUniformValue(normalTransformationUniformLocation,normalTransformationMatrix);
    shaderProgram.setUniformValue(transformationUniformLocation, transformationMatrix);
    shaderProgram.setUniformValue(projectionUniformLocation,  perspectiveTransformationMatrix);
    shaderProgram.setUniformValue(lightPositionUniformLocation, lightPosition);
    shaderProgram.setUniformValue(materialUniformLocation, material);
//    shaderProgram.setUniformValue(normalTransformationUniformLocation, 1, GL_FALSE, normalTransformationMatrix.data());
//    shaderProgram.setUniformValue(transformationUniformLocation, 1, GL_FALSE, transformationMatrix.data());
//    shaderProgram.setUniformValue(projectionUniformLocation, 1, GL_FALSE, perspectiveTransformationMatrix.data());
//    shaderProgram.setUniformValue(lightPositionUniformLocation, lightPosition.size(), lightPosition.data());
//    shaderProgram.setUniformValue(materialUniformLocation, 1, material.data());
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

