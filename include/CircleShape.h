#ifndef CHIPMUNK_CIRCLESHAPE_H
#define CHIPMUNK_CIRCLESHAPE_H

#include "Shape.h"

namespace Chipmunk
{
    class CircleShape : public Shape
    {
    public:
        CircleShape(std::shared_ptr<Body>, cpFloat radius, cpVect offset = cpv(0, 0));
        
        inline cpVect getOffset() const { return cpCircleShapeGetOffset(_shape); };
        inline cpFloat getRadius() const { return cpCircleShapeGetRadius(_shape); };
    };
}

#endif /* CHIPMUNK_CIRCLESHAPE_H */
