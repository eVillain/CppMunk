#ifndef CHIPMUNK_SEGMENTSHAPE_H
#define CHIPMUNK_SEGMENTSHAPE_H

#include "Shape.h"
#include "Body.h"
#include <memory>

namespace Chipmunk
{
    class SegmentShape : public Shape
    {
    public:
        SegmentShape(std::shared_ptr<Body>,
                     cpVect a,
                     cpVect b,
                     cpFloat radius);
        
        void setNeighbors(std::shared_ptr<Shape> shape, cpVect prev, cpVect next);
        cpVect getA(const std::shared_ptr<Shape> shape);
        cpVect getB(const std::shared_ptr<Shape> shape);
        cpVect getNormal(const std::shared_ptr<Shape> shape);
        cpFloat getRadius(const std::shared_ptr<Shape> shape);
    };
}

#endif /* CHIPMUNK_SEGMENTSHAPE_H */
