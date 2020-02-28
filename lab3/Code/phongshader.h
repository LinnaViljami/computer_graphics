#ifndef PHONGSHADER_H
#define PHONGSHADER_H

#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLShaderProgram>

class PhongShader
{
public:
    PhongShader();
    PhongShader(QOpenGLFunctions_3_3_Core* glFuncPointer);
    GLint transformationUniformLocation;
    GLint projectionUniformLocation;
    GLint normalTransformationUniformLocation;
    GLint materialUniformLocation;
    GLint lightPositionUniformLocation;
    void init(QOpenGLFunctions_3_3_Core* glFuncPointer);
private:
    QOpenGLFunctions_3_3_Core* glfunc;
    QOpenGLShaderProgram shaderProgram;
    void createShaderPrograms();
    void setUniformLocations();
};

#endif // PHONGSHADER_H
