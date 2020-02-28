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
    void setUniformData(QMatrix3x3 normalTransformationMatrix
                        , QMatrix4x4 transformationMatrix
                        , QVector3D material
                        , QMatrix4x4 perspectiveTransformationMatrix
                        , QVector3D lightPosition);
private:
    void createShaderPrograms() override;
    void setUniformLocations() override;

};

#endif // PHONGSHADER_H
