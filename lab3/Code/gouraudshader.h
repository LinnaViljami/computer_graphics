#ifndef GOURAUDSHADER_H
#define GOURAUDSHADER_H

#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLShaderProgram>
#include <shader.h>

class GouraudShader : public Shader
{
public:
    GouraudShader();
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

#endif // GOURAUDSHADER_H
