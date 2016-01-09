#ifndef CHIPMUNK_SHAPE_H
#define CHIPMUNK_SHAPE_H

#include <chipmunk.h>
#include "BoundingBox.h"
#include <memory>

namespace Chipmunk
{
    class Body;
    class Space;
    
    class Shape
    {
    public:
        virtual ~Shape();
        operator cpShape*() const;
        
        inline void setFriction(cpFloat friction) { cpShapeSetFriction(_shape, friction); };
        inline void setElasticity(cpFloat elasticity) { cpShapeSetElasticity(_shape, elasticity); };
        inline void setCollisionType(cpCollisionType type) { cpShapeSetCollisionType(_shape, type); };
        inline void setUserData(cpDataPointer data) { cpShapeSetUserData(_shape, data); };
        inline void setSensor(cpBool sensor) { cpShapeSetSensor(_shape, sensor); };
        inline void setSurfaceVelocity(cpVect surfaceVelocity) { cpShapeSetSurfaceVelocity(_shape, surfaceVelocity); }
        inline void setFilter(cpShapeFilter filter) { cpShapeSetFilter(_shape, filter); };
        
        inline cpFloat getFriction() const { return cpShapeGetFriction(_shape); };
        inline cpFloat getElasticity() const { return cpShapeGetElasticity(_shape); };
        inline cpCollisionType getCollisionType() const { return cpShapeGetCollisionType(_shape); };
        inline cpDataPointer getUserData() { return cpShapeGetUserData(_shape); };
        inline cpBool getSensor() { return cpShapeGetSensor(_shape); };
        inline cpVect getSurfaceVelocity() const { return cpShapeGetSurfaceVelocity(_shape); };
        inline cpShapeFilter getShapeFilter() const { return cpShapeGetFilter(_shape); };

        bool pointQuery(cpVect) const;
        bool segmentQuery(cpVect a, cpVect b, cpSegmentQueryInfo* = nullptr);
        
        BoundingBox cacheBoundingBox();
        BoundingBox getBoundingBox();
        
        inline cpShape* getShape() { return _shape; };
        inline std::shared_ptr<Body> getBody() { return _body; };
    protected:
        Shape(cpShape*, std::shared_ptr<Body>);
        cpShape* _shape;
        
        std::shared_ptr<Body> _body;
        void setBody(std::shared_ptr<Body> b);
    
    private:
        Shape(const Shape&);
        const Shape& operator=(const Shape&);
    };
}


#endif /* CHIPMUNK_SHAPE_H */
