#ifndef CATMULL_H
#define CATMULL_H

#include "vector"
#include "vertex.h"

class Catmull{
    public:
        std::vector<Vertex> verts;
    public:
        void insert(Vertex);
        Vertex getAt(float t);

};

#endif