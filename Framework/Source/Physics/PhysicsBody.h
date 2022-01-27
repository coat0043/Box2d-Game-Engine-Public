#pragma once

#include "../Math/Vector.h"
#include "PhysicsWorld.h"

namespace fw
{

class PhysicsBody
{
public:
    virtual ~PhysicsBody() = 0 {}

    virtual void CreateCircle(float radius) = 0;
    virtual void CreateBox(float width, float height) = 0;

    virtual void SetPosition(vec3 position) = 0;
    virtual void SetRotation(vec3 rotation) = 0;
    virtual void SetBodyType(BodyType bodyType) = 0;
    virtual void SetCanSleep(bool bCanSleep) = 0;
    virtual void SetDensity(float density) = 0;
    virtual void SetIsSensor(bool isSensor) = 0;
    virtual void SetFilterOptions(int category, int mask) = 0;

    virtual vec3 GetPosition() = 0;
    virtual vec3 GetRotation() = 0;
    virtual float GetMass() = 0;

    virtual void ApplyForceToCenter(vec3 force) = 0;
    virtual void ApplyImpulseToCenter(vec3 impulse) = 0;

    virtual void SetEnabled(bool enabled) = 0;
};

}

