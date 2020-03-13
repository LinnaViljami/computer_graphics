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
                                 QVector3D materialColor,
                                 QVector3D lightPosition,
                                 QVector3D lightColor,
                                 float phongExponent,
                                 bool useTextures
                                 )
{
    shaderProgram.setUniformValue(normalTransformationUniformLocation,normalTransformationMatrix);
    shaderProgram.setUniformValue(transformationUniformLocation, transformationMatrix);
    shaderProgram.setUniformValue(projectionUniformLocation,  perspectiveTransformationMatrix);
    shaderProgram.setUniformValue(lightPositionUniformLocation, lightPosition);
    shaderProgram.setUniformValue(materialUniformLocation, material);
    shaderProgram.setUniformValue(materialColorUniformLocation, materialColor);
    shaderProgram.setUniformValue(lightColorUniformLocation, lightColor);
    shaderProgram.setUniformValue(phongExponentUniformLocation, phongExponent);
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
    transformationUniformLocation = shaderProgram.uniformLocation("transformation");
    projectionUniformLocation = shaderProgram.uniformLocation("projection");
    normalTransformationUniformLocation = shaderProgram.uniformLocation("normalTransformation");
    materialUniformLocation = shaderProgram.uniformLocation("material");
    materialColorUniformLocation = shaderProgram.uniformLocation("materialColor");
    lightPositionUniformLocation = shaderProgram.uniformLocation("lightPosition");
    lightColorUniformLocation = shaderProgram.uniformLocation("lightColor");
    phongExponentUniformLocation = shaderProgram.uniformLocation("phongExponent");
    textureUniformLocation = shaderProgram.uniformLocation("textureUniform");
    useTexturesUniformLocation = shaderProgram.uniformLocation("useTextures");

}

