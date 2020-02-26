#include "object.h"
#include "model.h"

ImportedObject::ImportedObject() : vertices({})
{

}

ImportedObject::ImportedObject(float scaling_factor)
{
    Model model(":/models/sphere.obj");
    QVector<QVector3D> vertexLocations = model.getVertices();

    std::vector<vertex_3d> vertices_init(vertexLocations.size());
    for (QVector3D vector: vertexLocations) {
        float randomRed = std::rand() / (static_cast<float>(RAND_MAX));
        float randomGreen = std::rand() / (static_cast<float>(RAND_MAX));
        float randomBlue = std::rand() / (static_cast<float>(RAND_MAX));
        vertices_init.push_back({
           vector.x() * scaling_factor,
           vector.y() * scaling_factor,
           vector.z() * scaling_factor,
           randomRed,
           randomGreen,
           randomBlue
       });
    }
    vertices = vertices_init;
}
