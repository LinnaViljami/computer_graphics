#include "cube.h"

Cube::Cube()
{
    vertex_3d point1 = {-1, -1,-1, 1,0,0};
    vertex_3d point2 = {-1, -1,1, 1,1,1};
    vertex_3d point3 = {-1, 1, -1,1,0,1};
    vertex_3d point4 = {1, -1, -1,1,1,0};
    vertex_3d point5 = {-1, 1, 1,1,0,0};
    vertex_3d point6 = {1, -1, 1,1,0,1};
    vertex_3d point7 = {1, 1, -1,1,1,0};

    //opposite of 1
    vertex_3d point8 = {1, 1, 1,0,1,0};
    _verticies = {point1, point2, point3, point4, point5, point6, point7, point8};
    _triangles
            =
    {point1, point2, point4,
    point2, point6, point4,
    point4, point8, point7,
    point4, point6, point8,
    point1, point4, point3,
    point4, point7, point3,
    point5, point2, point1,
    point1, point3, point5,
    point5, point8, point2,
    point8, point6, point2,
    point8, point5, point7,
    point7, point5, point3};
}

std::vector<vertex_3d>* Cube::get_triangles()
{

    return &_triangles;
}

