#include "Space.h"
#include "Shape.h"
#include "Body.h"
#include "Constraint.h"
#include "Arbiter.h"

namespace Chipmunk
{
    Space::Space() :
    _space(cpSpaceNew()),
    _staticBody(std::make_shared<Body>(cpSpaceGetStaticBody(_space)))
    { }
    
    Space::~Space()
    {
        for (auto& shape : _shapes)
        {
            cpSpaceRemoveShape(_space, *shape);
        }
        _shapes.clear();
        cpSpaceFree(_space);
    }
    
    Space::operator cpSpace*()
    {
        return _space;
    }
    
    void Space::step(cpFloat dt)
    {
        cpSpaceStep(_space, dt);
    }
    
    void Space::add(std::shared_ptr<Shape> shape)
    {
        cpSpaceAddShape(_space, *shape);
        _shapes.push_back(shape);
    }
    
    void Space::add(std::shared_ptr<Body> body)
    {
        cpSpaceAddBody(_space, *body);
        _bodies.push_back(body);
    }
 
    void Space::add(std::shared_ptr<Constraint> constraint)
    {
        cpSpaceAddConstraint(_space, *constraint);
        _constraints.push_back(constraint);
    }
    
    void Space::remove(std::shared_ptr<Shape> shape)
    {
        cpSpaceRemoveShape(_space, *shape);
        _shapes.erase(find(_shapes.begin(), _shapes.end(), shape));
    }
    
    void Space::remove(std::shared_ptr<Body> body)
    {
        cpSpaceRemoveBody(_space, *body);
        _bodies.erase(find(_bodies.begin(), _bodies.end(), body));
    }

    void Space::remove(std::shared_ptr<Constraint> constraint)
    {
        cpSpaceRemoveConstraint(_space, *constraint);
        _constraints.erase(find(_constraints.begin(), _constraints.end(), constraint));
    }
    
    std::shared_ptr<Shape> Space::findShape(cpShape* shape) const
    {
        if (!shape) {
            return std::shared_ptr<Shape>((Shape*)0);
        }
        auto it = find_if(_shapes.begin(), _shapes.end(),
                          [&shape](const std::shared_ptr<Shape>& s)
                          {
                              return *s == shape;
                          });
        assert(it != _shapes.end());
        return *it;
    }
    
    std::shared_ptr<Body> Space::findBody(cpBody* body) const
    {
        if (!body) {
            return std::shared_ptr<Body>((Body*)0);
        }
        auto it = find_if(_bodies.begin(), _bodies.end(),
                          [&body](const std::shared_ptr<Body>& b)
                          {
                              return *b == body;
                          });
        assert(it != _bodies.end());
        return *it;
    }
    
    std::shared_ptr<Constraint> Space::findConstraint(cpConstraint* constraint) const
    {
        if (!constraint)
        {
            return std::shared_ptr<Constraint>((Constraint*)0);
        }
        auto it = find_if(_constraints.begin(), _constraints.end(),
                          [&constraint](const std::shared_ptr<Constraint>& c)
        {
                              return *c == constraint;
                          });
        assert(it != _constraints.end());
        return *it;
    }
    
    void Space::segmentQueryFunc(cpShape* shape,
                                 cpVect point,
                                 cpVect normal,
                                 cpFloat alpha,
                                 void* data)
    {
        auto d = reinterpret_cast<SegmentQueryData*>(data);
        d->func(d->self->findShape(shape), alpha, normal);
    }
    
    void Space::segmentQuery(cpVect a,
                             cpVect b,
                             LayerMask layers,
                             cpGroup group,
                             SegmentQueryFunc func) const
    {
        SegmentQueryData data = { this, func };
        cpShapeFilter filter{static_cast<cpGroup>(group),
            static_cast<cpBitmask>(layers),
            static_cast<cpBitmask>(layers)};
        cpSpaceSegmentQuery(_space, a, b, 0, filter, segmentQueryFunc, &data);
    }
    
    std::shared_ptr<Shape> Space::segmentQueryFirst(cpVect a,
                                                    cpVect b,
                                                    LayerMask layers,
                                                    cpGroup group,
                                                    cpSegmentQueryInfo* const info) const
    {
        cpSegmentQueryInfo i;
        cpShapeFilter filter {
            static_cast<cpGroup>(group),
            static_cast<cpBitmask>(layers),
            static_cast<cpBitmask>(layers)
        };
        auto rtn = cpSpaceSegmentQueryFirst(_space, a, b, 0, filter, &i);
        if (info)
        {
            info->shape = i.shape;
            info->point = i.point;
            info->alpha = i.alpha;
            info->normal = i.normal;
        }
        return findShape(rtn);
    }
    
    std::shared_ptr<Shape> Space::pointQueryNearest(cpVect p,
                                                  LayerMask layers,
                                                  cpGroup group) const
    {
        cpShapeFilter filter{static_cast<cpGroup>(group),
            static_cast<cpBitmask>(layers),
            static_cast<cpBitmask>(layers)};
        cpPointQueryInfo i;
        return findShape(cpSpacePointQueryNearest(_space, p, 100, filter, &i));
    }
    
    cpBool Space::helperBegin(cpArbiter* arb, cpSpace* s, void* d)
    {
        CallbackData& data = *reinterpret_cast<CallbackData*>(d);
        return data.begin(arb, data.self);
    }
    
    cpBool Space::helperPreSolve(cpArbiter* arb, cpSpace* s, void* d)
    {
        CallbackData& data = *reinterpret_cast<CallbackData*>(d);
        return data.preSolve(arb, data.self);
    }
    
    void Space::helperPostSolve(cpArbiter* arb, cpSpace* s, void* d)
    {
        CallbackData& data = *reinterpret_cast<CallbackData*>(d);
        return data.postSolve(arb, data.self);
    }
    
    void Space::helperSeparate(cpArbiter* arb, cpSpace* s, void* d)
    {
        CallbackData& data = *reinterpret_cast<CallbackData*>(d);
        return data.separate(arb, data.self);
    }
    
    void Space::addCollisionHandler(cpCollisionType a,
                                    cpCollisionType b,
                                    std::function<int(Arbiter, Space&)> begin,
                                    std::function<int(Arbiter, Space&)> preSolve,
                                    std::function<void(Arbiter, Space&)> postSolve,
                                    std::function<void(Arbiter, Space&)> separate)
    {
        auto data = new CallbackData(begin, preSolve, postSolve, separate, *this);
        callbackDatas[std::make_pair(a, b)] = std::unique_ptr<CallbackData>(data);
        cpCollisionHandler* handler = cpSpaceAddCollisionHandler(_space, a, b);
        handler->beginFunc = begin == nullptr ? nullptr : helperBegin;
        handler->preSolveFunc = preSolve == nullptr ? nullptr : helperPreSolve;
        handler->postSolveFunc = postSolve == nullptr ? nullptr : helperPostSolve;
        handler->separateFunc = separate == nullptr ? nullptr : helperSeparate;
        handler->userData = data;
    }

    void Space::reindexShapesForBody(std::shared_ptr<Body> body)
    {
        cpSpaceReindexShapesForBody(_space, *body);
    }
    
    void Space::reindexShape(std::shared_ptr<Shape> shape)
    {
        cpSpaceReindexShape(_space, *shape);
    }

    void Space::clearSpace()
    {
        // Must remove these BEFORE freeing the bodies or you will access dangling pointers.
        cpSpaceEachShape(_space, (cpSpaceShapeIteratorFunc)helperPostShapeFree, _space);
        cpSpaceEachConstraint(_space, (cpSpaceConstraintIteratorFunc)helperPostConstraintFree, _space);
        cpSpaceEachBody(_space, (cpSpaceBodyIteratorFunc)helperPostBodyFree, _space);
    }
    
    void Space::helperShapeFreeWrap(cpSpace *space, cpShape *shape, void *unused)
    {
        cpSpaceRemoveShape(space, shape);
        cpShapeFree(shape);
    }
    
    void Space::helperPostShapeFree(cpShape *shape, cpSpace *space)
    {
        cpSpaceAddPostStepCallback(space, (cpPostStepFunc)helperShapeFreeWrap, shape, NULL);
    }
    
    void Space::helperConstraintFreeWrap(cpSpace *space, cpConstraint *constraint, void *unused)
    {
        cpSpaceRemoveConstraint(space, constraint);
        cpConstraintFree(constraint);
    }
    
    void Space::helperPostConstraintFree(cpConstraint *constraint, cpSpace *space)
    {
        cpSpaceAddPostStepCallback(space, (cpPostStepFunc)helperConstraintFreeWrap, constraint, NULL);
    }
    
    void Space::helperBodyFreeWrap(cpSpace *space, cpBody *body, void *unused)
    {
        cpSpaceRemoveBody(space, body);
        cpBodyFree(body);
    }
    
    void Space::helperPostBodyFree(cpBody *body, cpSpace *space)
    {
        cpSpaceAddPostStepCallback(space, (cpPostStepFunc)helperBodyFreeWrap, body, NULL);
    }
    
    void Space::helperShapeAddWrap(cpSpace *space, cpShape *shape, void *unused)
    {
        cpSpaceAddShape(space, shape);
    }
    
    void Space::helperPostShapeAdd(cpShape *shape, cpSpace *space)
    {
        cpSpaceAddPostStepCallback(space, (cpPostStepFunc)helperShapeAddWrap, shape, NULL);
    }
    
    void Space::helperConstraintAddWrap(cpSpace *space, cpConstraint *constraint, void *unused)
    {
        cpSpaceAddConstraint(space, constraint);
    }
    
    void Space::helperPostConstraintAdd(cpConstraint *constraint, cpSpace *space)
    {
        cpSpaceAddPostStepCallback(space, (cpPostStepFunc)helperConstraintAddWrap, constraint, NULL);
    }
    
    void Space::helperBodyAddWrap(cpSpace *space, cpBody *body, void *unused)
    {
        cpSpaceAddBody(space, body);
    }
    
    void Space::helperPostBodyAdd(cpBody *body, cpSpace *space)
    {
        cpSpaceAddPostStepCallback(space, (cpPostStepFunc)helperBodyAddWrap, body, NULL);
    }
}