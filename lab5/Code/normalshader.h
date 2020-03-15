#ifndef NORMALSHADER_H
#define NORMALSHADER_H

#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLShaderProgram>
#include <shader.h>

class NormalShader : public Shader
{
public:
    NormalShader();
    GLint transformationUniformLocation;
    GLint projectionUniformLocation;
    GLint normalTransformationUniformLocation;
    GLint useTexturesUniformLocation;


    // Shader interface
public:
    ShadingMode type() override;
    void setUniformData(QMatrix4x4 transformationMatrix, QMatrix4x4 projectionMatrix, QMatrix3x3 normalTransformationMatrix);

private:
    void createShaderPrograms() override;
    void setUniformLocations() override;

    // Shader interface
};

#endif // NORMALSHADER_H
