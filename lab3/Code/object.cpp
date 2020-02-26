#include "object.h"
#include "model.h"

ImportedObject::ImportedObject() : vertices({})
{

}

ImportedObject::ImportedObject(float scaling_factor)
{
    Model model(":/models/sphere.obj");
    QVector<QVector3D> vertexLocations = model.getVertices();
    QVector<QVector3D> vertexNormals = model.getNormals();

    std::vector<vertex_3d> vertices_init(vertexLocations.size());
    for (int i = 0; i < vertexLocations.size(); i++) {
        QVector3D location = vertexLocations.at(i);
        QVector3D normal = vertexNormals.at(i);

        vertices_init.push_back({
            location.x() * scaling_factor,
            location.y() * scaling_factor,
            location.z() * scaling_factor,
            normal.x(),
            normal.y(),
            normal.z(),
       });
    }

    vertices = vertices_init;
}
