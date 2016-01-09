#ifndef CHIPMUNK_CONSTRAINT_H
#define CHIPMUNK_CONSTRAINT_H

#include <chipmunk.h>
#include <memory>

namespace Chipmunk
{
    class Body;

    class Constraint
    {
    public:
        virtual ~Constraint();
        operator cpConstraint*() const;
        
        inline cpBool getCollideBodies() { return cpConstraintGetCollideBodies(_constraint); };
        inline cpFloat getErrorBias() { return cpConstraintGetErrorBias(_constraint); };
        inline cpFloat getImpulse() { return cpConstraintGetImpulse(_constraint); };
        inline cpFloat getMaxBias() { return cpConstraintGetMaxBias(_constraint); };
        inline cpFloat getMaxForce() { return cpConstraintGetMaxForce(_constraint); };
        inline cpDataPointer getUserData() { return cpConstraintGetUserData(_constraint); };

        inline void setCollideBodies(cpBool collideBodies) { cpConstraintSetCollideBodies(_constraint, collideBodies); };
        inline void setErrorBias(cpFloat errorBias) { cpConstraintSetErrorBias(_constraint, errorBias); };
        inline void setMaxBias(cpFloat data) { cpConstraintSetMaxBias(_constraint, data); };
        inline void setMaxForce(cpFloat data) { cpConstraintSetMaxForce(_constraint, data); };
        inline void setUserData(cpDataPointer data) { cpConstraintSetUserData(_constraint, data); };

        std::shared_ptr<Body> getBodyA() { return _bodyA; }
        std::shared_ptr<Body> getBodyB() { return _bodyB; }

    protected:
        Constraint(cpConstraint* constraint,
                   std::shared_ptr<Body> bodyA,
                   std::shared_ptr<Body> bodyB);

        cpConstraint* _constraint;
        
        std::shared_ptr<Body> _bodyA;
        std::shared_ptr<Body> _bodyB;
        void setBodyA(std::shared_ptr<Body> b);
        void setBodyB(std::shared_ptr<Body> b);

    private:
        Constraint(const Constraint&);
        const Constraint& operator=(const Constraint&);

    };
}

#endif /* CHIPMUNK_CONSTRAINT_H */
