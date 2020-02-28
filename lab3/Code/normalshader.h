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
    GLint materialUniformLocation;
    GLint lightPositionUniformLocation;

    // Shader interface
public:
    ShadingMode type();

private:
    void createShaderPrograms();
    void setUniformLocations();

    // Shader interface
};

#endif // NORMALSHADER_H
