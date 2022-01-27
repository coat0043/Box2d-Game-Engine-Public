#pragma once

namespace fw {

class PhysicsConstraint
{
public:
    virtual ~PhysicsConstraint() = 0 {}

    // Getters.
    virtual bool IsMotorEnabled() = 0;

    // Setters.
    virtual void SetMotorEnabled(bool enabled) = 0;
};

} // namespace fw
