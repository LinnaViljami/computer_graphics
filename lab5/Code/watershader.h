#ifndef WATERSHADER_H
#define WATERSHADER_H

#include "shader.h"
#include "wave.h"
#include <vector>

const int MAX_WAVES = 10;

class WaterShader : public Shader
{

public:
    WaterShader();
    GLint transformationUniformLocation;
    GLint viewTransformationUniformLocation;
    GLint projectionUniformLocation;
    GLint lightPositionUniformLocation;
    GLint normalTransformationUniformLocation;
    GLint useTexturesUniformLocation;
    GLint amplitudeUniformLocation;
    GLint frequencyUniformLocation;
    GLint phaseUniformLocation;
    GLint materialUniformLocation;
    GLint lightColorUniformLocation;


    // Shader interface
public:
    void setUniformData(QMatrix4x4 transformationMatrix
                                      , QMatrix4x4 perspectiveTransformationMatrix
                                      , QMatrix4x4 viewTransformationMatrix
                                      , QMatrix3x3 normalTransformationMatrix
                                      , QVector3D lightPosition
                                      , QVector3D lightColor
                                      , QVector4D material
                                      , float phase
                                      , std::vector<Wave> waves);
    ShadingMode type() override;

private:
    void createShaderPrograms() override;
    void setUniformLocations() override;
    void setWaveUniformsData(std::vector<Wave> waves);
};

#endif // WATERSHADER_H
