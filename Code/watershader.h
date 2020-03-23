#ifndef GOURAUDSHADER_H
#define GOURAUDSHADER_H

#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLShaderProgram>
#include <shader.h>

class WaterShader : public Shader
{
public:
    WaterShader();
    GLint transformationUniformLocation;
    GLint viewTransformationUniformLocation;
    GLint projectionUniformLocation;
    GLint normalTransformationUniformLocation;
    GLint materialUniformLocation;
    GLint materialColorUniformLocation;
    GLint lightPositionUniformLocation;
    GLint lightColorUniformLocation;
    GLint textureUniformLocation;
    GLint useTexturesUniformLocation;
    // Shader interface
public:
    ShadingMode type();
    void setUniformData(QMatrix4x4 transformationMatrix,
                        QMatrix4x4 viewTransformationMatrix,
                        QMatrix4x4 perspectiveTransformationMatrix,
                        QMatrix3x3 normalTransformationMatrix,
                        QVector4D material,
                        QVector3D lightPosition,
                        QVector3D lightColor,
                        bool useTextures);
    GLint * getTextureBufferLocation() override;
private:
    void createShaderPrograms() override;
    void setUniformLocations() override;

    // Shader interface
};

#endif // GOURAUDSHADER_H
