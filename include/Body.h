#ifndef CHIPMUNK_BODY_H
#define CHIPMUNK_BODY_H

#include <chipmunk.h>

namespace Chipmunk
{
    class Body
    {
    public:
        Body(cpFloat mass, cpFloat inertia);
        Body(Body&&);
        explicit Body(cpBody* body);
        ~Body();
        operator cpBody*() const;

        void updateVelocity(cpVect gravity, cpFloat damping, cpFloat dt) { cpBodyUpdateVelocity(_body, gravity, damping, dt); };
        
        void updatePosition(cpFloat dt) { cpBodyUpdatePosition(_body, dt); };
        
        inline cpVect localToWorld(cpVect point) const { return cpBodyLocalToWorld(_body, point); };
        inline cpVect worldToLocal(cpVect point) const { return cpBodyWorldToLocal(_body, point); };
        
        void applyImpulseAtWorldPoint(cpVect impulse, cpVect point) { cpBodyApplyImpulseAtWorldPoint(_body, impulse, point); };
        void applyImpulseAtLocalPoint(cpVect impulse, cpVect point) { cpBodyApplyImpulseAtLocalPoint(_body, impulse, point); };
        
        void applyForceAtWorldPoint(cpVect force, cpVect point) { cpBodyApplyForceAtWorldPoint(_body, force, point); };
        void applyForceAtLocalPoint(cpVect force, cpVect point) { cpBodyApplyForceAtWorldPoint(_body, force, point); };
        
        inline void setBodyType(cpBodyType type) { cpBodySetType(_body, type); }
        inline void setPosition(cpVect position) { cpBodySetPosition(_body, position); };
        inline void setVelocity(cpVect velocity) { cpBodySetVelocity(_body, velocity); };
        inline void setAngle(cpFloat angle) { cpBodySetAngle(_body, angle); };
        inline void setMass(cpFloat mass) { cpBodySetMass(_body, mass); };
        inline void setMoment(cpFloat moment) { cpBodySetMoment(_body, moment); };
        inline void setAngularVelocity(cpFloat velocity) { cpBodySetAngularVelocity(_body, velocity); };
        inline void setTorque(cpFloat torque) { cpBodySetTorque(_body, torque); };
        inline void setUserData(cpDataPointer data) { cpBodySetUserData(_body, data); }
        
        inline cpBodyType getBodyType() const { return cpBodyGetType(_body); };
        inline cpVect getPosition() const { return cpBodyGetPosition(_body); };
        inline cpVect getVelocity() const { return cpBodyGetVelocity(_body); };
        inline cpFloat getAngle() const { return cpBodyGetAngle(_body); };
        inline cpFloat getAngularVelocity() const { return cpBodyGetAngularVelocity(_body); };
        inline cpFloat getMass() const { return cpBodyGetMass(_body); };
        inline cpFloat getMoment() const { return cpBodyGetMoment(_body); };
        inline cpVect getVecAngle() const { return cpBodyGetRotation(_body); };
        inline cpVect getForce() const { return cpBodyGetForce(_body); };
        inline cpFloat getTorque() const { return cpBodyGetTorque(_body); };
        inline cpDataPointer getUserData() const { return cpBodyGetUserData(_body); }
        inline cpBody* getBody() const { return _body; };

        inline cpVect getVelocityAtWorldPoint(cpVect point) const { return cpBodyGetVelocityAtWorldPoint(_body, point); };
        inline cpVect getVelocityAtLocalPoint(cpVect point) const { return cpBodyGetVelocityAtLocalPoint(_body, point); };

        
    protected:
        cpBody* _body;
        bool _dirty;
        
    private:
        Body(const Body&);
        const Body& operator=(const Body&);
    };
}

#endif /* Body_hpp */
