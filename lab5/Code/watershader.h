#ifndef WATERSHADER_H
#define WATERSHADER_H

#include "shader.h"

class WaterShader : public Shader
{
public:
    WaterShader();
    GLint transformationUniformLocation;
    GLint viewTransformationUniformLocation;
    GLint projectionUniformLocation;
    GLint normalTransformationUniformLocation;
    GLint useTexturesUniformLocation;
    GLint amplitudeUniformLocation;
    GLint frequencyUniformLocation;
    GLint phaseUniformLocation;


    // Shader interface
public:
    void setUniformData(QMatrix4x4 transformationMatrix
                                      , QMatrix4x4 perspectiveTransformationMatrix
                                      , QMatrix4x4 viewTransformationMatrix
                                      , QMatrix3x3 normalTransformationMatrix
                                      , float amplitude, float frequency, float phase);
    ShadingMode type() override;

private:
    void createShaderPrograms() override;
    void setUniformLocations() override;
};

#endif // WATERSHADER_H
