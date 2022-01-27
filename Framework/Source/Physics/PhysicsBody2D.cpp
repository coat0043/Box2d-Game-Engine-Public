#include "FrameworkPCH.h"

#include "PhysicsWorld.h"
#include "PhysicsBody.h"
#include "PhysicsWorld2D.h"
#include "PhysicsBody2D.h"
#include "box2d/box2d.h"

namespace fw
{

PhysicsBody2D::PhysicsBody2D(b2Body* body) :
    m_pBody(body)
{
    m_pBody->GetUserData().pointer = reinterpret_cast<uintptr_t>(this);
}

PhysicsBody2D::~PhysicsBody2D()
{
    //TODO: Why is this not throwing an error?
    m_pBody->GetWorld()->DestroyBody(m_pBody);
}

void PhysicsBody2D::CreateCircle(float radius)
{
    //Create circle shape and assign radius.
    b2CircleShape circleShape;
    circleShape.m_radius = radius;

    //Create fixtureDef and replace default values.
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &circleShape;
    fixtureDef.density = 1.0f; //Default

    m_pBody->CreateFixture(&fixtureDef);
}

void PhysicsBody2D::CreateBox(float width, float height)
{
    //Create the box shape and assign dimensions.
    b2PolygonShape boxShape;
    boxShape.SetAsBox(width * 0.5f, height * 0.5f);

    //Create fixtureDef and replace default values.
    b2FixtureDef fixtureDef;
    fixtureDef.shape = &boxShape;
    fixtureDef.density = 1.0f; //Default

    m_pBody->CreateFixture(&fixtureDef);
}

void PhysicsBody2D::SetPosition(vec3 position)
{
    m_pBody->SetTransform(b2Vec2(position.x, position.y), m_pBody->GetAngle());
}

void PhysicsBody2D::SetRotation(vec3 rotation)
{
    m_pBody->SetTransform(m_pBody->GetPosition(), -rotation.z / 180.0f * PI);
}

void PhysicsBody2D::SetBodyType(BodyType bodyType)
{
    switch (bodyType)
    {
    case StaticBody:
        m_pBody->SetType(b2_staticBody);
        break;

    case DynamicBody:
        m_pBody->SetType(b2_dynamicBody);
        break;
    };
}

void PhysicsBody2D::SetCanSleep(bool bCanSleep)
{
    m_pBody->SetSleepingAllowed(bCanSleep);
}

void PhysicsBody2D::SetDensity(float density)
{
    for (b2Fixture* pFixture = m_pBody->GetFixtureList(); pFixture != nullptr; pFixture = pFixture->GetNext())
    {
        pFixture->SetDensity(density);
    }
    m_pBody->ResetMassData();
}

void PhysicsBody2D::SetIsSensor(bool isSensor)
{
    for (b2Fixture* pFixture = m_pBody->GetFixtureList(); pFixture != nullptr; pFixture = pFixture->GetNext())
    {
        pFixture->SetSensor(isSensor);
    }
}

void PhysicsBody2D::SetFilterOptions(int category, int mask)
{
    for (b2Fixture* pFixture = m_pBody->GetFixtureList(); pFixture != nullptr; pFixture = pFixture->GetNext())
    {
        b2Filter filterInfo;
        filterInfo.categoryBits = category;
        filterInfo.maskBits = mask;

        pFixture->SetFilterData(filterInfo);
    }
}

vec3 PhysicsBody2D::GetPosition()
{
    return vec3(m_pBody->GetPosition().x, m_pBody->GetPosition().y);
}

vec3 PhysicsBody2D::GetRotation()
{
    return vec3(0, 0, -m_pBody->GetAngle() * 180.0f/PI);
}

float PhysicsBody2D::GetMass()
{
    return m_pBody->GetMass();
}

void PhysicsBody2D::ApplyForceToCenter(vec3 force)
{
    m_pBody->ApplyForceToCenter( b2Vec2(force.x, force.y), true );
}

void PhysicsBody2D::ApplyImpulseToCenter(vec3 impulse)
{
    m_pBody->ApplyLinearImpulseToCenter(b2Vec2(impulse.x, impulse.y), true);
}

void PhysicsBody2D::SetEnabled(bool enabled)
{
    m_pBody->SetEnabled( enabled );
}

}
