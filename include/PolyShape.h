#ifndef CHIPMUNK_POLYSHAPE_H
#define CHIPMUNK_POLYSHAPE_H

#include "Shape.h"
#include <vector>

namespace Chipmunk
{
    class PolyShape : public Shape
    {
    public:
        PolyShape(std::shared_ptr<Body>, const std::vector<cpVect>& verts);
        
        int getNumVerts() const { return cpPolyShapeGetCount(_shape); };
        cpVect getVert(int i) { return cpPolyShapeGetVert(_shape, i); };
        cpFloat getRadius() { return cpPolyShapeGetRadius(_shape); };
    };
}

#endif /* CHIPMUNK_POLYSHAPE_H */
