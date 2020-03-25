#ifndef PYRAMID_H
#define PYRAMID_H

#include "vertex3d.h"
#include <vector>
#include <GL/gl.h>
class Pyramid
{
public:
    Pyramid();
    std::vector<vertex3d>* getTriangles();
    GLuint vboId;
    GLuint vaoId;

private:
    std::vector<vertex3d> vertices;
    std::vector<vertex3d> triangles;

};

#endif // PYRAMID_H
