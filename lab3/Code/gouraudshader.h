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
    GLint materialColorUniformLocation;
    GLint lightPositionUniformLocation;
    GLint lightColorUniformLocation;
    GLint phongExponentUniformLocation;
    GLint textureUniformLocation;
    GLint useTexturesUniformLocation;
    // Shader interface
public:
    ShadingMode type();
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
    void createShaderPrograms();
    void setUniformLocations();

    // Shader interface
};

#endif // GOURAUDSHADER_H
