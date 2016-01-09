#ifndef CHIPMUNK_SPACE_H
#define CHIPMUNK_SPACE_H

#include <chipmunk.h>
#include "LayerMask.h"
#include <functional>
#include <memory>
#include <vector>
#include <map>

namespace Chipmunk
{
    class Body;
    class Arbiter;

    class Constraint;
    class Shape;
    
    typedef std::function<void(std::shared_ptr<Shape>, cpFloat, cpVect)> SegmentQueryFunc;
    
    class Space
    {
    public:
        Space();
        explicit Space(cpSpace*);
        ~Space();
        operator cpSpace*();
        
        virtual void step(cpFloat dt);

        void add(std::shared_ptr<Shape>);
        void add(std::shared_ptr<Body>);
        void add(std::shared_ptr<Constraint>);
        void remove(std::shared_ptr<Shape>);
        void remove(std::shared_ptr<Body>);
        void remove(std::shared_ptr<Constraint>);
        
        void segmentQuery(cpVect a, cpVect b, LayerMask, cpGroup, SegmentQueryFunc) const;
        std::shared_ptr<Shape> segmentQueryFirst(cpVect a, cpVect b, LayerMask, cpGroup, cpSegmentQueryInfo* = nullptr) const;
        std::shared_ptr<Shape> pointQueryFirst(cpVect p, LayerMask, cpGroup) const;
        
        void addCollisionHandler(cpCollisionType a, cpCollisionType b,
                                 std::function<int(Arbiter, Space&)> begin,
                                 std::function<int(Arbiter, Space&)> preSolve,
                                 std::function<void(Arbiter, Space&)> postSolve,
                                 std::function<void(Arbiter, Space&)> separate);

        
        inline int getIterations() const { return cpSpaceGetIterations(_space); };
        inline cpVect getGravity() const { return cpSpaceGetGravity(_space); };
        inline cpFloat getDamping() const { return cpSpaceGetDamping(_space); };
        inline cpFloat getTimeStamp() const { return cpSpaceGetCurrentTimeStep(_space); };
        
        void setIterations(int iterations) { cpSpaceSetIterations(_space, iterations); };
        void setGravity(cpVect gravity) { cpSpaceSetGravity(_space, gravity); };
        void setDamping(cpFloat damping) { cpSpaceSetDamping(_space, damping); };
        
        void reindexStatic() { cpSpaceReindexStatic(_space); };
        void reindexShapesForBody(std::shared_ptr<Body> body);
        void reindexShape(std::shared_ptr<Shape> shape);
        
        
        virtual void clearSpace();
        
        inline cpSpace* getSpace() const { return _space; };
    protected:
        cpSpace* _space;
        std::shared_ptr<Body> _staticBody;
        
    private:
        Space(const Space&);
        const Space& operator=(const Space&);
        static void segmentQueryFunc(cpShape*, cpVect, cpVect, cpFloat, void*);
        std::shared_ptr<Shape> findShape(cpShape*) const;
        std::shared_ptr<Body> findBody(cpBody*) const;
        std::shared_ptr<Constraint> findConstraint(cpConstraint*) const;

        std::vector<std::shared_ptr<Shape>> _shapes;
        std::vector<std::shared_ptr<Body>> _bodies;
        std::vector<std::shared_ptr<Constraint>> _constraints;

        struct SegmentQueryData
        {
            const Space* const self;
            SegmentQueryFunc& func;
        };
        
        struct CallbackData
        {
            std::function<int(Arbiter, Space&)> begin;
            std::function<int(Arbiter, Space&)> preSolve;
            std::function<void(Arbiter, Space&)> postSolve;
            std::function<void(Arbiter, Space&)> separate;
            Space& self;
            
            CallbackData(std::function<int(Arbiter, Space&)> begin, std::function<int(Arbiter, Space&)> preSolve,
                         std::function<void(Arbiter, Space&)> postSolve, std::function<void(Arbiter, Space&)> separate,
                         Space& self)
            : begin(begin), preSolve(preSolve), postSolve(postSolve), separate(separate), self(self)
            {}
        };
        
        std::map<std::pair<cpCollisionType, cpCollisionType>, std::unique_ptr<CallbackData>> callbackDatas;
        
        static cpBool helperBegin(cpArbiter* arb, cpSpace* s, void* d);
        static cpBool helperPreSolve(cpArbiter* arb, cpSpace* s, void* d);
        static void helperPostSolve(cpArbiter* arb, cpSpace* s, void* d);
        static void helperSeparate(cpArbiter* arb, cpSpace* s, void* d);
        
        static void helperShapeFreeWrap(cpSpace *space, cpShape *shape, void *unused);
        static void helperPostShapeFree(cpShape *shape, cpSpace *space);
        static void helperConstraintFreeWrap(cpSpace *space, cpConstraint *constraint, void *unused);
        static void helperPostConstraintFree(cpConstraint *constraint, cpSpace *space);
        static void helperBodyFreeWrap(cpSpace *space, cpBody *body, void *unused);
        static void helperPostBodyFree(cpBody *body, cpSpace *space);
        static void helperShapeAddWrap(cpSpace *space, cpShape *shape, void *unused);
        static void helperPostShapeAdd(cpShape *shape, cpSpace *space);
        static void helperConstraintAddWrap(cpSpace *space, cpConstraint *constraint, void *unused);
        static void helperPostConstraintAdd(cpConstraint *constraint, cpSpace *space);
        static void helperBodyAddWrap(cpSpace *space, cpBody *body, void *unused);
        static void helperPostBodyAdd(cpBody *body, cpSpace *space);
    };
}


#endif /* CHIPMUNK_SPACE_H */
