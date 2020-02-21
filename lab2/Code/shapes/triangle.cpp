#include "triangle.h"
#include <cmath>
Hit Triangle::intersect(Ray const &ray)
{

    return Hit::NO_HIT(); // placeholder
}

std::pair<bool, Point> Triangle::getIntersectionPoint(Ray ray)
{

    if(!isIntersection(ray)){
        return std::pair<bool, Point>(false, Point());
    }

    Point q2 = ray.D;
    Point q1 = Point(0,0,0);
    double t = (-1 * (q1 - this->v0).dot(this->N))/((q2 - q1).dot(this->N));
    Point intersectionPoint = q1 + t*(q2-q1);
    return std::pair<bool, Point>(true, intersectionPoint);
}

bool Triangle::isIntersection(Ray ray)
{
    Point q2 = ray.D;
    Point q1 = Point(0,0,0);
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
    Point cross_product = (b-a).cross(c-a);
    return (1/6) * cross_product.dot((d-a));

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
    N = u.cross(v);
    // which is declared in the header. It can then be used in the intersect function.
}
