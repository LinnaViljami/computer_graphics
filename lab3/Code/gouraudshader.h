#ifndef GOURAUDSHADER_H
#define GOURAUDSHADER_H

#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLShaderProgram>
#include <shader.h>

class GouraudShader : Shader
{
public:
    GouraudShader();
    GLint transformationUniformLocation;
    GLint projectionUniformLocation;
    GLint normalTransformationUniformLocation;
    GLint materialUniformLocation;
    GLint lightPositionUniformLocation;
};

#endif // GOURAUDSHADER_H
