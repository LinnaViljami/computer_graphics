#ifndef OBJECT_H
#define OBJECT_H

#include "vertex3d.h"
#include <vector>
#include <QString>
#include <QObject>
#include <GL/gl.h>
#include "model.h"
#include <QMatrix4x4>
#include "TextureType.h"

struct Material {
    float ka = 0.5f;
    float kd = 0.5f;
    float ks = 0.5f;
    float phongExponent = 5.0f;
};

enum ImportedObjectType {cat, sphere, cube, flat_surface};

struct ImportedObjectProperties {
    Model model = Model(":/models/cat.obj");
    float scalingFactor = 1.0f;
    Material material = Material();
    GLsizei nTextures = 1;
};


class ImportedObject
{
public:
    ImportedObject();
    ImportedObject(ImportedObjectType objectType, TextureType texture, Material material = Material());
    std::vector<vertex3d> vertices;
    QVector<QVector2D> textureCoordinates;
    GLuint vboId;
    GLuint vaoId;
    TextureType textureType;
    ImportedObjectProperties modelProps;
    QMatrix4x4 translationMatrix;
    QMatrix4x4 rotationMatrix;
    QMatrix4x4 scalingMatrix;
    QMatrix4x4 getModelTransformationMatrix();
    QVector4D getMaterialVector();
    QVector3D getMaterialColorVector();
    void setRotation(int rotateX, int rotateY, int rotateZ);
    void setScale(int scale);
    void setTranslation(float translateX, float translateY, float translateZ);

private:
    ImportedObjectProperties getModelProperties(ImportedObjectType modelType, Material material);

};

#endif // OBJECT_H
