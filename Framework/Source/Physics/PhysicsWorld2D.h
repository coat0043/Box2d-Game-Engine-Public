#pragma once

#include "Physics/PhysicsWorld.h"

class b2_draw;
class b2Body;
class b2World;

namespace fw
{

class Box2dDebugDraw;
class CameraComponent;
class EventManager;
class Material;
class PhysicsBody2D;
class PhysicsConstraint;

class PhysicsWorld2D : public PhysicsWorld
{
protected:
    b2World* m_pWorld;
    Box2dDebugDraw* m_pDebugDraw;

    float m_UnusedTime = 0.0f;

    b2Body* m_pGround = nullptr;

public:
    PhysicsWorld2D(EventManager* pEventManager);
    virtual ~PhysicsWorld2D();

    virtual void Update(float deltaTime) override;
    virtual void DrawDebugData(CameraComponent* pCamera, bool drawOnXZPlane) override;

    virtual PhysicsBody* CreateBody(vec3 position, BodyType bodyType, bool bCanSleep) override;

    virtual PhysicsConstraint* CreateConstraint(PhysicsBody* pBodyA, PhysicsBody* pBodyB, vec3 worldAnchorPoint) override;

    // Getters.
    virtual vec3 GetGravity() override;

    // Setters.
    virtual void SetGravity(vec3 gravity) override;
    virtual void SetCanSleep(bool bCanSleep) override;
};

}
