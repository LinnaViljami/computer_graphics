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
    GLint materialColorUniformLocation;
    GLint lightPositionUniformLocation;
    GLint lightColorUniformLocation;
    GLint phongExponentUniformLocation;
    GLint textureUniformLocation;
    GLint useTexturesUniformLocation;

    ShadingMode type() override;
    void setUniformData(QMatrix4x4 transformationMatrix,
                        QMatrix4x4 perspectiveTransformationMatrix,
                        QMatrix3x3 normalTransformationMatrix,
                        QVector3D material,
                        QVector3D materialColor,
                        QVector3D lightPosition,
                        QVector3D lightColor,
                        float phongExponent,
                        bool useTextures);
    GLint * getTextureBufferLocation() override;
private:
    void createShaderPrograms() override;
    void setUniformLocations() override;

};

#endif // PHONGSHADER_H
