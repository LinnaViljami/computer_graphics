#include "shader.h"

Shader::Shader() : glfunc(nullptr)
{

}

void Shader::init(QOpenGLFunctions_3_3_Core *glFuncPointer)
{
    glfunc = glFuncPointer;
    createShaderPrograms();
    setUniformLocations();
}

void Shader::bind()
{
    shaderProgram.bind();
}

void Shader::release()
{
    shaderProgram.release();
}

Shader::~Shader()
{

}
