#ifndef CUBE_H
#define CUBE_H

#include "vertex_3d.h"
#include <vector>
#include <GL/gl.h>
class Cube
{
public:
    Cube();
    std::vector<vertex_3d>* get_triangles();
    GLuint vbo_id;
    GLuint vao_id;
private:
    std::vector<vertex_3d> _verticies;
    std::vector<vertex_3d> _triangles;
};

#endif // CUBE_H
