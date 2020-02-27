#ifndef OBJECT_H
#define OBJECT_H

#include "vertex3d.h"
#include <vector>
#include <QString>
#include <GL/gl.h>
#include "model.h"

enum ImportedObjectType {cat, sphere, cube, flat_surface};

struct ImportedObjectProperties {
    Model model = Model(":/models/cat.obj");
    float scalingFactor = 1.0f;
};

class ImportedObject
{
public:
    ImportedObject();
    ImportedObject(ImportedObjectType objectType);
    std::vector<vertex3d> vertices;
    GLuint vboId;
    GLuint vaoId;
private:
    ImportedObjectProperties getModelProperties(ImportedObjectType modelType);
};

#endif // OBJECT_H
