#include "FrameworkPCH.h"

#include "box2d/box2d.h"
#include "PhysicsConstraint2D.h"
#include "PhysicsWorld2D.h"
#include "Math/Vector.h"

namespace fw {

PhysicsConstraint2D::PhysicsConstraint2D()
{
}

PhysicsConstraint2D::~PhysicsConstraint2D()
{
}

bool PhysicsConstraint2D::IsMotorEnabled()
{
    b2RevoluteJoint* pRevolute = static_cast<b2RevoluteJoint*>(m_pJoint);
    return pRevolute->IsMotorEnabled();
}

void PhysicsConstraint2D::SetMotorEnabled(bool enabled)
{
    b2RevoluteJoint* pRevolute = static_cast<b2RevoluteJoint*>(m_pJoint);

    pRevolute->EnableMotor(enabled);
    pRevolute->SetMaxMotorTorque(1000.0);
    pRevolute->SetMotorSpeed(1.0f);
}

} // namespace fw
