#include "cube.h"

Cube::Cube()
{
    vertex3d point1 = {-1, -1,-1, 1,0,0};
    vertex3d point2 = {-1, -1,1, 1,1,1};
    vertex3d point3 = {-1, 1, -1,1,0,1};
    vertex3d point4 = {1, -1, -1,1,1,0};
    vertex3d point5 = {-1, 1, 1,1,0,0};
    vertex3d point6 = {1, -1, 1,1,0,1};
    vertex3d point7 = {1, 1, -1,1,1,0};

    //opposite of 1
    vertex3d point8 = {1, 1, 1,0,1,0};
    vertices = {point1, point2, point3, point4, point5, point6, point7, point8};
    triangles
            =
    {point2, point1, point4,
    point6, point2, point4,
    point8, point4, point7,
    point6, point4, point8,
    point4, point1, point3,
    point7, point4, point3,
    point2, point5, point1,
    point3, point1, point5,
    point8, point5, point2,
    point6, point8, point2,
    point5, point8, point7,
    point5, point7, point3};
}

std::vector<vertex3d>* Cube::getTriangles()
{

    return &triangles;
}

