#include "catmull.h"

void Catmull::insert(Vertex v){
    this->verts.push_back(v);
}


Vertex Catmull::getAt(float t){
    int i = (int)t;
    t -= (int)t;
    return Vertex::catmull(
        std::array<Vertex, 4>{
              this->verts[i%this->verts.size()]
            , this->verts[(i+1)%this->verts.size()]
            , this->verts[(i+2)%this->verts.size()]
            , this->verts[(i+3)%this->verts.size()]
            }, t);
}