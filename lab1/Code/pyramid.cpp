#include "pyramid.h"

pyramid::pyramid()
{
    vertex_3d point1 = {-1, -1,-1, 1,0,0};
    vertex_3d point2 = {-1, 1,-1, 1,1,1};
    vertex_3d point3 = {1, -1, -1,1,0,1};
    vertex_3d point4 = {1, 1, -1,1,1,0};
    vertex_3d point5 = {0.5, 0.5, 1,1,0,0};
    _verticies = {point1, point2, point3, point4, point5};
    _triangles = {point1, point2, point5
                  , point1, point3, point5
                 ,point2, point4, point5
                 ,point3, point4, point5
                 ,point1,point2,point3
                 ,point4,point2,point3};

}

std::vector<vertex_3d> *pyramid::get_triangles()
{
    return &_triangles;
}
