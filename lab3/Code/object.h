#ifndef OBJECT_H
#define OBJECT_H

#include "vertex3d.h"
#include <vector>
#include <GL/gl.h>

class ImportedObject
{
public:
    ImportedObject();
    ImportedObject(float scalingFactor);
    std::vector<vertex3d> vertices;
    GLuint vboId;
    GLuint vaoId;
private:
};

#endif // OBJECT_H
