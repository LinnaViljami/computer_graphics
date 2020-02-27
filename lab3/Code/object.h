#ifndef OBJECT_H
#define OBJECT_H

#include "vertex3d.h"
#include <vector>
#include <QString>
#include <GL/gl.h>
#include "model.h"


struct Material {
    float ka = 0.5f;
    float kd = 0.7f;
    float ks = 0.1f;
};

enum ImportedObjectType {cat, sphere, cube, flat_surface};

struct ImportedObjectProperties {
    Model model = Model(":/models/cat.obj");
    float scalingFactor = 1.0f;
    Material material = Material();
};


class ImportedObject
{
public:
    ImportedObject();
    ImportedObject(ImportedObjectType objectType, Material material = Material());
    std::vector<vertex3d> vertices;
    GLuint vboId;
    GLuint vaoId;
private:
    ImportedObjectProperties getModelProperties(ImportedObjectType modelType, Material material);

};

#endif // OBJECT_H
