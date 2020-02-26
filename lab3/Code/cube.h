#ifndef CUBE_H
#define CUBE_H

#include "vertex3d.h"
#include <vector>
#include <GL/gl.h>
class Cube
{
public:
    Cube();
    std::vector<vertex3d>* getTriangles();
    GLuint vboId;
    GLuint vaoId;
private:
    std::vector<vertex3d> vertices;
    std::vector<vertex3d> triangles;
};

#endif // CUBE_H
