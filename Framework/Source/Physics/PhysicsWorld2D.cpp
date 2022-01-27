#include "FrameworkPCH.h"

#include "PhysicsWorld.h"
#include "PhysicsBody.h"
#include "PhysicsConstraint2D.h"
#include "PhysicsWorld2D.h"
#include "PhysicsBody2D.h"
#include "box2d/box2d.h"
#include "Box2dDebugDraw.h"
#include "EventSystem/Event.h"
#include "EventSystem/EventManager.h"
#include "Utility/Color.h"

namespace fw
{

class OurContactListener : public b2ContactListener
{
public:
    OurContactListener(EventManager* pEventManager)
    {
        m_pEventManager = pEventManager;
    }

    virtual void BeginContact(b2Contact* contact) override
    {
        b2Fixture* pFixtureA = contact->GetFixtureA();
        b2Fixture* pFixtureB = contact->GetFixtureB();

        b2Body* pBodyA = pFixtureA->GetBody();
        b2Body* pBodyB = pFixtureB->GetBody();

        PhysicsBody* pPhysicsBodyA = reinterpret_cast<PhysicsBody*>(pBodyA->GetUserData().pointer);
        PhysicsBody* pPhysicsBodyB = reinterpret_cast<PhysicsBody*>(pBodyB->GetUserData().pointer);

        // Send a collision event.
        PhysicsCollisionEvent* pCollisionEvent = new PhysicsCollisionEvent(pPhysicsBodyA, pPhysicsBodyB);
        m_pEventManager->AddEvent(pCollisionEvent);
    }

protected:
    EventManager* m_pEventManager = nullptr;
};

PhysicsWorld2D::PhysicsWorld2D(EventManager* pEventManager) :
    m_pWorld(nullptr)
{
    m_pWorld = new b2World(b2Vec2(0.0f, -10.0f));
    m_pWorld->SetContactListener(new OurContactListener(pEventManager));
    m_pWorld->SetAutoClearForces( false );

    b2BodyDef bodyDef;
    m_pGround = m_pWorld->CreateBody(&bodyDef);

    m_pDebugDraw = new Box2dDebugDraw();
    uint32 flags = b2Draw::e_shapeBit;
    flags |= b2Draw::e_jointBit;
    flags |= b2Draw::e_centerOfMassBit;
    flags |= b2Draw::e_aabbBit;
    flags |= b2Draw::e_pairBit;
    m_pDebugDraw->SetFlags(flags);
    m_pWorld->SetDebugDraw(m_pDebugDraw);
}

PhysicsWorld2D::~PhysicsWorld2D()
{
    m_pWorld->DestroyBody(m_pGround);

    delete m_pWorld->GetContactManager().m_contactListener;
    delete m_pDebugDraw;
    delete m_pWorld;
}

void PhysicsWorld2D::Update(float deltaTime)
{
    m_UnusedTime += deltaTime;
    while( m_UnusedTime >= 1/60.0f )
    {
        m_pWorld->Step(1/60.0f, 8, 3);
        m_pWorld->ClearForces();

        m_UnusedTime -= 1/60.0f;
    }
}

PhysicsBody* PhysicsWorld2D::CreateBody(vec3 position, BodyType bodyType, bool bCanSleep)
{
    //Declare the bodyDef of default values for the b2Body.
    b2BodyDef bodyDef;

    //Sets the b2Body default position in the world space.
    bodyDef.position = b2Vec2(position.x, position.y);

    //Sets the b2Body type. Default is Static.
    switch (bodyType)
    {
    case StaticBody:
        bodyDef.type = b2_staticBody;
        break;

    case DynamicBody:
        bodyDef.type = b2_dynamicBody;
        break;
    };

    //Creates the b2Body in the world.
    b2Body* pBody = m_pWorld->CreateBody(&bodyDef);

    //Create PhysicsBody2D Wrapper around the b2Body.
    PhysicsBody2D* pBody2D = new PhysicsBody2D(pBody);

    //Return the new PhysicsBody to the world.
    return pBody2D;
}

PhysicsConstraint* PhysicsWorld2D::CreateConstraint(PhysicsBody* pBodyA, PhysicsBody* pBodyB, vec3 worldAnchorPoint)
{
    PhysicsBody2D* pBody2DA = static_cast<PhysicsBody2D*>(pBodyA);
    PhysicsBody2D* pBody2DB = static_cast<PhysicsBody2D*>(pBodyB);

    b2Body* p1stBody = pBody2DA->GetBody();
    b2Body* p2ndBody = pBody2DB ? pBody2DB->GetBody() : m_pGround;

    b2RevoluteJointDef jointDef;
    jointDef.Initialize(p1stBody, p2ndBody, b2Vec2(worldAnchorPoint.x, worldAnchorPoint.y));

    b2Joint* pJoint = m_pWorld->CreateJoint(&jointDef);

    PhysicsConstraint2D* pConstraint = new PhysicsConstraint2D();
    pConstraint->SetJoint( pJoint );

    return pConstraint;
}

vec3 PhysicsWorld2D::GetGravity()
{
    b2Vec2 gravity = m_pWorld->GetGravity();

    return vec3(gravity.x, gravity.y, 0);
}

void PhysicsWorld2D::DrawDebugData(CameraComponent* pCamera, bool drawOnXZPlane)
{
    m_pDebugDraw->SetCamera(pCamera, drawOnXZPlane);
    m_pDebugDraw->StartMeshes();
    m_pWorld->DebugDraw();
    m_pDebugDraw->EndMeshes();
}

void PhysicsWorld2D::SetGravity(vec3 gravity)
{
    //Set the world's global gravity vector
    m_pWorld->SetGravity(b2Vec2(gravity.x, gravity.y));
}

void PhysicsWorld2D::SetCanSleep(bool bCanSleep)
{
    m_pWorld->SetAllowSleeping(bCanSleep);
}

}
