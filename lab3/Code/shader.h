#ifndef SHADER_H
#define SHADER_H

#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLShaderProgram>


enum ShadingMode : GLuint {
    PHONG = 0, NORMAL, GOURAUD
};

class Shader
{
public:
    Shader();
    virtual ShadingMode type() = 0;
    virtual ~Shader() = 0;
    void init(QOpenGLFunctions_3_3_Core* glFuncPointer);
    void bind();
    void release();
protected:
    QOpenGLFunctions_3_3_Core* glfunc;
    QOpenGLFunctions_3_3_Core* getGlfuncPtr();
    QOpenGLShaderProgram shaderProgram;
private:
    virtual void createShaderPrograms() = 0;
    virtual void setUniformLocations() = 0;
};

#endif // SHADER_H
