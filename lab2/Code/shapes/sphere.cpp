#include "sphere.h"

#include <cmath>


std::pair<double, double> solve_quadratic_equation(double a, double b, double c);
std::pair<bool, double> get_intersection_point(std::pair<double, double> roots);

using namespace std;

Hit Sphere::intersect(Ray const &ray)
{
    /****************************************************
    * RT1.1: INTERSECTION CALCULATION
    *
    * Given: ray, position, r
    * Sought: intersects? if true: *t
    *
    * Insert calculation of ray/sphere intersection here.
    *
    * You have the sphere's center (C) and radius (r) as well as
    * the ray's origin (ray.O) and direction (ray.D).
    *
    * If the ray does not intersect the sphere, return false.
    * Otherwise, return true and place the distance of the
    * intersection point from the ray origin in *t (see example).
    ****************************************************/


    //ray.O origin of the ray = O
    // ray.D = v, ray vector
    // C = this->position
    // r = this->r

    // solve possible intersection points
    double a = ray.D.dot(ray.D);
    double b = 2*(ray.O-this->position).dot(ray.D);
    double c = (ray.O-this->position).dot(ray.O-this->position) - pow(this->r, 2);
    std::pair<double, double> roots = solve_quadratic_equation(a,b,c);
    std::pair<bool, double> intersection_info = get_intersection_point(roots);
    bool hit_detected = intersection_info.first;
    double t = intersection_info.second;

    if (hit_detected){
        return Hit(t, get_normal_vector(ray.O + t*ray.D, ray));
    }
    else{

        return Hit::NO_HIT();
    }

}


Sphere::Sphere(Point const &pos, double radius)
:
    position(pos),
    r(radius)
{}

Vector Sphere::get_normal_vector(Point in_surface, Ray ray)
{
    Vector normal_vector = (in_surface - position).normalized();
    if(ray.D.dot(normal_vector) >= 0){
        return normal_vector;
    }
    else {
        return -1*normal_vector;
    }
}


std::pair<bool, double> get_intersection_point(std::pair<double, double> roots){

    // intersection origin + root*vector with smallest positive root
    std::pair<bool, double> result;
    double root1 = roots.first;
    double root2 = roots.second;

    if (root1 >= 0 && root2 < 0){
        result = std::pair<bool, double>(true, root1);
    }

    else if (root2 >= 0 && root1 < 0){
        result = std::pair<bool, double>(true, root2);
    }

    else if (root1 >= 0 && root2 >= 0){
        double smaller = root1;
        if (root1>root2){
            smaller = root2;
        }
        result = std::pair<bool, double>(true, smaller);
    }
    else{
        result = std::pair<bool, double>(false, -1);
    }

    return result;
}

std::pair<double, double> solve_quadratic_equation(double a, double b, double c){

    double discriminant, root1, root2;
    root1 = -1;
    root2 = -1;
    discriminant = b * b - 4 * a * c;

    // condition for real and different roots
    if (discriminant > 0) {
        root1 = (-b + sqrt(discriminant)) / (2 * a);
        root2 = (-b - sqrt(discriminant)) / (2 * a);
    }
    // condition for real and equal roots
    else if (discriminant >= 0) {
        root1 = root2 = -b / (2 * a);
    }
    return std::pair<double, double>(root1, root2);
}
