#include "mesh.h"

#include "../objloader.h"
#include "../vertex.h"
#include "triangle.h"

#include <cmath>
#include <iostream>
#include <limits>

using namespace std;

Hit Mesh::intersect(Ray const &ray)
{
    bool hitFound = false;
    Hit nearestHit = Hit::NO_HIT();
    for (auto object : d_tris) {
        Hit currentHit = object->intersect(ray);
        if (!hitFound) {
            hitFound = true;
            nearestHit = currentHit;
        } else if (currentHit.t < nearestHit.t) {
            nearestHit = currentHit;
        }
    }

    if (!hitFound) {
        std::cout << "No hit found\n";
    } else {
        std::cout << "Hit found\n";
    }
    return nearestHit;
}

Mesh::Mesh(string const &filename, Point const &position, Vector const &rotation, Vector const &scale)
{
    OBJLoader model(filename);
    d_tris.reserve(model.numTriangles());
    vector<Vertex> vertices = model.vertex_data();
    for (size_t tri = 0; tri != model.numTriangles(); ++tri)
    {
        Vertex one = vertices[tri * 3];
        Point v0(one.x, one.y, one.z);

        Vertex two = vertices[tri * 3 + 1];
        Point v1(two.x, two.y, two.z);

        Vertex three = vertices[tri * 3 + 2];
        Point v2(three.x, three.y, three.z);

        // Apply non-uniform scaling, rotation and translation to the three points
        // of the triangle (v0, v1, and v2) here.

        // Non-uniform scaling
        scalePoint(v0, scale);
        scalePoint(v1, scale);
        scalePoint(v2, scale);

        // Rotation
        xRotatePoint(v0, rotation);
        xRotatePoint(v1, rotation);
        xRotatePoint(v2, rotation);

        yRotatePoint(v0, rotation);
        yRotatePoint(v1, rotation);
        yRotatePoint(v2, rotation);

        zRotatePoint(v0, rotation);
        zRotatePoint(v1, rotation);
        zRotatePoint(v2, rotation);

        // Translation
        translatePoint(v0, position);
        translatePoint(v1, position);
        translatePoint(v2, position);

        d_tris.push_back(ObjectPtr(new Triangle(v0, v1, v2)));
    }

    cout << "Loaded model: " << filename << " with " <<
        model.numTriangles() << " triangles.\n";
}

void Mesh::scalePoint(Point &point, Vector scale) {
    point.x = point.x * scale.x;
    point.y = point.y * scale.y;
    point.z = point.z * scale.z;
}

void Mesh::xRotatePoint(Point &point, Vector rotation) {
    point.y = cos(rotation.x)*point.y - sin(rotation.x)*point.z;
    point.z = sin(rotation.x)*point.y + cos(rotation.x)*point.z;
}

void Mesh::yRotatePoint(Point &point, Vector rotation) {
    point.x = cos(rotation.y)*point.x + sin(rotation.y)*point.z;
    point.z = -sin(rotation.y)*point.x + cos(rotation.y)*point.z;
}

void Mesh::zRotatePoint(Point &point, Vector rotation) {
    point.x = cos(rotation.z)*point.x - sin(rotation.z)*point.y;
    point.y = sin(rotation.z)*point.x + cos(rotation.z)*point.y;
}

void Mesh::translatePoint(Point &point, Vector position) {
    point.x = point.x + position.x;
    point.y = point.y + position.y;
    point.z = point.z + position.z;
}
