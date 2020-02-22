#include "triangle.h"
#include <cmath>
#include <iostream>
Hit Triangle::intersect(Ray const &ray)
{
    std::pair<bool, double> intersection_info = getIntersectionInfo(ray);
    bool is_intersection = intersection_info.first;
    double intersection_time = intersection_info.second;
    if (is_intersection){
        if(N.dot(ray.D) < 0){
            return Hit(intersection_time,  N);
        }
        else {
            return Hit(intersection_time, -1.0 * N);
        }

    }
    else{
        return Hit::NO_HIT();
    }
}

std::pair<bool, double> Triangle::getIntersectionInfo(Ray ray)
{

    if(!isIntersection(ray)){
        //std::cout << "no intersection" << std::endl;
        return std::pair<bool, double>(false, -1);
    }

    double t = -1.0 * ((ray.O - this->v0).dot(this->N))/(ray.D.dot(this->N));
    //std::cout << "intersection happened, t=" << t;
    return std::pair<bool, double>(true, t);
}

bool Triangle::isIntersection(Ray ray)
{
    Point q2 = ray.O + 10000000*ray.D;
    Point q1 = ray.O;
    Point p1 = v0;
    Point p2 = v1;
    Point p3 = v2;
    if(signedVolume(q1,p1,p2,p3) * signedVolume(q2,p1,p2,p3) >= 0){
        return false;
    }
    else if (signedVolume(q1,q2,p1,p2) * signedVolume(q1,q2,p2,p3) >= 0
             && signedVolume(q1,q2,p3,p1) * signedVolume(q1,q2,p2,p3) >= 0){
        return true;
    }
    else{
        return false;
    }

}

double Triangle::signedVolume(Point a, Point b, Point c, Point d)
{

    //std::cout << a << std::endl;
    //std::cout << b << std::endl;

//    std::cout << c << std::endl;

  //  std::cout << d << std::endl;

    Point cross_product = (b-a).cross(c-a);

   // std::cout << cross_product << std::endl;

    return (1.0/6.0) * cross_product.dot((d-a));

}

Triangle::Triangle(Point const &v0,
                   Point const &v1,
                   Point const &v2)
:
    v0(v0),
    v1(v1),
    v2(v2),
    N()
{

    // Calculate the surface normal here and store it in the N,
    // plane direction vectors
    Vector u = v1-v0;
    Vector v = v2-v0;
    N = u.cross(v).normalized();
    // which is declared in the header. It can then be used in the intersect function.
}
