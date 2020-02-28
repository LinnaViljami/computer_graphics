#include "shader.h"

Shader::Shader()
{

}

void Shader::init()
{
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
