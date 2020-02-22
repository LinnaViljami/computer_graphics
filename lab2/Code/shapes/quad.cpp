#include "quad.h"

#include <limits>

using namespace std;

Hit Quad::intersect(Ray const &ray)
{
    Triangle triangle1 = _triangles.first;
    Triangle triangle2 = _triangles.second;
    bool intersectWith1 = triangle1.isIntersection(ray);
    bool intersectWith2 = triangle2.isIntersection(ray);
    Hit intersection1 = triangle1.intersect(ray);
    Hit intersection2 = triangle2.intersect(ray);

    if(!intersectWith1 && !intersectWith2){
        return Hit::NO_HIT();
    }
    else if(intersectWith1 && intersection1.t > 0){
        return intersection1;
    }
    else if(intersectWith2 && intersection2.t > 0){
        return intersection2;
    }
    else{
        return Hit::NO_HIT();
    }
}

Quad::Quad(Point const &v0,
           Point const &v1,
           Point const &v2,
           Point const &v3) : _triangles(std::pair<Triangle, Triangle>(Triangle(v0, v1, v3),Triangle(v1, v2, v3)))
{
}
