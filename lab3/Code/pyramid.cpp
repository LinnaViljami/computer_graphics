#include "pyramid.h"

Pyramid::Pyramid()
{
    vertex_3d point1 = {-1, -1, -1, 1,0,0};
    vertex_3d point2 = {1, -1,-1, 1,1,1};
    vertex_3d point3 = {-1, -1, 1,1,0,1};
    vertex_3d point4 = {1, -1, 1,1,1,0};
    vertex_3d point5 = {0, 1, 0,1,0,0};
    vertices = {point1, point2, point3, point4, point5};
    triangles = {point3, point1, point2
                 ,point3, point2, point4
                 ,point2, point1, point5
                 ,point5, point1, point3
                 ,point5, point3, point4
                 ,point5, point4, point2};

}

std::vector<vertex_3d> *Pyramid::get_triangles()
{
    return &triangles;
}
