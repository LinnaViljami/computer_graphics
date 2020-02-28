#ifndef PHONGSHADER_H
#define PHONGSHADER_H

#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLShaderProgram>
#include <shader.h>
class PhongShader : public Shader
{
public:
    PhongShader();
    GLint transformationUniformLocation;
    GLint projectionUniformLocation;
    GLint normalTransformationUniformLocation;
    GLint materialUniformLocation;
    GLint lightPositionUniformLocation;
    ShadingMode type() override;
private:
    void createShaderPrograms() override;
    void setUniformLocations() override;
};

#endif // PHONGSHADER_H
