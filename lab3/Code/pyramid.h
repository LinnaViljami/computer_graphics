#ifndef PYRAMID_H
#define PYRAMID_H

#include "vertex_3d.h"
#include <vector>
#include <GL/gl.h>
class Pyramid
{
public:
    Pyramid();
    std::vector<vertex_3d>* get_triangles();
    GLuint vbo_id;
    GLuint vao_id;

private:
    std::vector<vertex_3d> vertices;
    std::vector<vertex_3d> triangles;

};

#endif // PYRAMID_H
