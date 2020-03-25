#ifndef SHADER_H
#define SHADER_H

#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLShaderProgram>



class Shader
{
public:

    enum ShadingMode : GLuint {
        PHONG = 0, NORMAL, GOURAUD, WATER
    };
    Shader();
    virtual ShadingMode type() = 0;
    virtual ~Shader() = 0;
    void init();
    void bind();
    void release();
    virtual GLint* getTextureBufferLocation();
protected:
    QOpenGLShaderProgram shaderProgram;
private:
    virtual void createShaderPrograms() = 0;
    virtual void setUniformLocations() = 0;
};

#endif // SHADER_H
