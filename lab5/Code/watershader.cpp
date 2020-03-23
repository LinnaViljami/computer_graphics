#include "watershader.h"

WaterShader::WaterShader()
{

}

Shader::ShadingMode WaterShader::type()
{
    return ShadingMode::WATER;
}

void WaterShader::setUniformData(QMatrix4x4 transformationMatrix
                                  , QMatrix4x4 perspectiveTransformationMatrix
                                  , QMatrix4x4 viewTransformationMatrix
                                  , QMatrix3x3 normalTransformationMatrix
                                 , QVector3D lightPosition
                                 , QVector3D lightColor
                                 , QVector4D material
                                 , float phase
                                 , std::vector<Wave> waves)
{
    shaderProgram.setUniformValue(normalTransformationUniformLocation,normalTransformationMatrix);
    shaderProgram.setUniformValue(transformationUniformLocation, transformationMatrix);
    shaderProgram.setUniformValue(viewTransformationUniformLocation, viewTransformationMatrix);
    shaderProgram.setUniformValue(lightPositionUniformLocation, lightPosition);
    shaderProgram.setUniformValue(lightColorUniformLocation, lightColor);
    shaderProgram.setUniformValue(projectionUniformLocation,  perspectiveTransformationMatrix);
    shaderProgram.setUniformValue(materialUniformLocation, material);
    shaderProgram.setUniformValue(phaseUniformLocation, phase);
    setWaveUniformsData(waves);
}

void WaterShader::createShaderPrograms()
{
    shaderProgram.addShaderFromSourceFile(QOpenGLShader::Vertex,
                                           ":/shaders/vertshader_water.glsl");
    shaderProgram.addShaderFromSourceFile(QOpenGLShader::Fragment,
                                           ":/shaders/fragshader_water.glsl");
    shaderProgram.link();

}

void WaterShader::setUniformLocations()
{
    transformationUniformLocation = shaderProgram.uniformLocation("transformation");
    viewTransformationUniformLocation = shaderProgram.uniformLocation("viewTransform");
    projectionUniformLocation = shaderProgram.uniformLocation("projection");
    lightPositionUniformLocation = shaderProgram.uniformLocation("lightPosition");
    lightColorUniformLocation = shaderProgram.uniformLocation("lightColor");
    normalTransformationUniformLocation = shaderProgram.uniformLocation("normalTransformation");
    amplitudeUniformLocation = shaderProgram.uniformLocation("amplitudes");
    frequencyUniformLocation = shaderProgram.uniformLocation("frequencies");
    phaseUniformLocation = shaderProgram.uniformLocation("phase");
    materialUniformLocation = shaderProgram.uniformLocation("material");
}

void WaterShader::setWaveUniformsData(std::vector<Wave> waves)
{
    float amplitudes[MAX_WAVES];
    float frequencies[MAX_WAVES];
    int amount_waves = waves.size();
    for(int i = 0; i<MAX_WAVES; i++){
        if(i < amount_waves){
            amplitudes[i] = waves.at(i).amplitude;
            frequencies[i] = waves.at(i).frequency;
        }
        else{
            amplitudes[i] = 0;
            frequencies[i] = 0;
        }
    }

    shaderProgram.setUniformValueArray(amplitudeUniformLocation, &amplitudes[0],10,1);
    shaderProgram.setUniformValueArray(frequencyUniformLocation, &frequencies[0], 10, 1);

}

