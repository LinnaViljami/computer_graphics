#ifndef OBJECT_H
#define OBJECT_H

#include "vertex_3d.h"
#include <vector>
#include <GL/gl.h>

class ImportedObject
{
public:
    ImportedObject();
    ImportedObject(float scaling_factor);
    std::vector<vertex_3d> vertices;
    GLuint vbo_id;
    GLuint vao_id;
private:
};

#endif // OBJECT_H
