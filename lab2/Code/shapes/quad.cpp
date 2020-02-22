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

//        if(intersection1.t < 0 && intersection2.t < 0){
//        return Hit::NO_HIT();
//    }
//    else if(intersection1.t > 0 && intersection2.t > 0){
//        //lambda, picks first positive intersection
//        auto smaller = [](auto i1, auto i2){
//            if(i1.t<i2.t){return i1;} else{return i2;}};

//        return smaller(intersection1, intersection2);
//    }
//    else if(intersection1.t < 0){
//        return intersection2;
//    }
//    else{
//        return intersection1;
//    }

}

Quad::Quad(Point const &v0,
           Point const &v1,
           Point const &v2,
           Point const &v3) : _triangles(std::pair<Triangle, Triangle>(Triangle(v0, v1, v3),Triangle(v1, v2, v3)))
{
}
