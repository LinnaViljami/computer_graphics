#include "quad.h"

#include <limits>

using namespace std;

Hit Quad::intersect(Ray const &ray)
{
    // placeholder
    return Hit::NO_HIT();
}

Quad::Quad(Point const &v0,
           Point const &v1,
           Point const &v2,
           Point const &v3) : _triangles(std::pair<Triangle, Triangle>(Triangle(v0, v1, v2),Triangle(v1, v2, v3)))
{
}
