#ifndef MESH_H_
#define MESH_H_

#include "../object.h"

#include <string>
#include <vector>

class Mesh: public Object
{
    std::vector<ObjectPtr> d_tris;

    public:
        Mesh(std::string const &filename,
             Point const &position,
             Vector const &rotation,
             Vector const &scale);

        virtual Hit intersect(Ray const &ray);

    private: 
        void scalePoint(Point &point, Vector scale);
        void xRotatePoint(Point &point, Vector rotation);
        void yRotatePoint(Point &point, Vector rotation);
        void zRotatePoint(Point &point, Vector rotation);
        void translatePoint(Point &point, Vector position);
};

#endif
