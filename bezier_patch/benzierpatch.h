#ifndef BENZIER_PATCH_H
#define BENZIER_PATCH_H

#include <array>
#include "point.h"
#include "controlpoints.h"

class BenzierPatch {
    public:
        std::array<std::array<Point, 4>, 4> ps;

    public:
        BenzierPatch(){};
        BenzierPatch(std::array<std::array<Point, 4>, 4> ps);

        Point evalBenzier(float u, float v);
        
};

#endif