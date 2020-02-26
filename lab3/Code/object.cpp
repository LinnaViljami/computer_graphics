#include "object.h"

ImportedObject::ImportedObject() : vertices({})
{

}

ImportedObjectProperties ImportedObject::getModelProperties(ImportedObjectType modelType)
{
    ImportedObjectProperties properties;
    switch (modelType) {
    case cat:
        properties.model = Model(":/models/cat.obj");
        properties.scalingFactor = 8.0f;
        break;
    case sphere:
        properties.model = Model(":/models/sphere.obj");
        properties.scalingFactor = 0.04f;
        break;
    case cube:
        break;
    case flat_surface:
        break;
    }
    return properties;
}


ImportedObject::ImportedObject(ImportedObjectType objectType)
{
    ImportedObjectProperties modelProps = getModelProperties(objectType);
    QVector<QVector3D> vertexLocations = modelProps.model.getVertices();
    QVector<QVector3D> vertexNormals = modelProps.model.getNormals();

    std::vector<vertex3d> vertices_init = {};
    for (int i = 0; i < vertexLocations.size(); i++) {
        QVector3D location = vertexLocations.at(i);
        QVector3D normal = vertexNormals.at(i);
        vertices_init.push_back({
            location.x() * modelProps.scalingFactor,
            location.y() * modelProps.scalingFactor,
            location.z() * modelProps.scalingFactor,
            normal.x(),
            normal.y(),
            normal.z(),
       });
    }

    vertices = vertices_init;
}
