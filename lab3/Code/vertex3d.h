#ifndef VERTEX_3D_H
#define VERTEX_3D_H

#include <limits>

struct vertex3d {
    float x;
    float y;
    float z;
    float normalX = std::numeric_limits<float>::min();
    float normalY = std::numeric_limits<float>::min();
    float normalZ = std::numeric_limits<float>::min();
};
#endif // VERTEX_3D_H
