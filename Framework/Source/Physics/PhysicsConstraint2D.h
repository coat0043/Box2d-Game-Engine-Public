#pragma once

#include "PhysicsConstraint.h"

class b2Joint;

namespace fw {

class PhysicsConstraint2D : public PhysicsConstraint
{
public:
    PhysicsConstraint2D();
    virtual ~PhysicsConstraint2D() override;

    // Getters.
    virtual bool IsMotorEnabled() override;

    // Setters.
    void SetJoint(b2Joint* pJoint) { m_pJoint = pJoint; }
    virtual void SetMotorEnabled(bool enabled) override;

protected:
    b2Joint* m_pJoint = nullptr;
};

} // namespace fw
