#ifndef TRIANGLE_H_
#define TRIANGLE_H_

#include "../object.h"

class Triangle: public Object
{
    public:
        Triangle(Point const &v0,
                 Point const &v1,
                 Point const &v2);

        virtual Hit intersect(Ray const &ray);

        Point v0;
        Point v1;
        Point v2;
        Vector N;
        bool isIntersection(Ray ray);
    private:
        std::pair<bool, double> getIntersectionInfo(Ray ray);

        double signedVolume(Point a, Point b, Point c, Point d);
};

#endif
