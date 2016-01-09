#include "Shape.h"
#include "Body.h"

namespace Chipmunk
{
    Shape::Shape(cpShape* s, std::shared_ptr<Body> b) :
    _body(b),
    _shape(s)
    { }
    
    Shape::~Shape()
    {
        if (_shape != NULL)
            cpShapeFree(_shape);
    }
    
    Shape::operator cpShape*() const
    {
        return _shape;
    }
    
    bool Shape::pointQuery(cpVect p) const
    {
        return cpShapePointQuery(_shape, p, nullptr) == cpTrue;
    }
    
    bool Shape::segmentQuery(cpVect a, cpVect b, cpSegmentQueryInfo* const info)
    {
        cpSegmentQueryInfo i;
        bool rtn = cpShapeSegmentQuery(_shape, a, b, 0, &i) == cpTrue;
        if (info) {
            info->shape = i.shape;
            info->point = i.point;
            info->alpha = i.alpha;
            info->normal = i.normal;
        }
        return rtn;
    }
    
    BoundingBox Shape::cacheBoundingBox()
    {
        return BoundingBox(cpShapeCacheBB(_shape));
    }
    
    BoundingBox Shape::getBoundingBox()
    {
        return BoundingBox(cpShapeGetBB(_shape));
    }

    void Shape::setBody(std::shared_ptr<Body> b) {
        cpShapeSetBody(_shape, b ? (*b) : (cpBody*)0);
        _body = b;
    }
}


