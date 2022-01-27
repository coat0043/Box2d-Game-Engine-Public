#include "FrameworkPCH.h"

#include "PhysicsRevoluteComponent.h"
#include "Components/CollisionComponent.h"
#include "Objects/GameObject.h"
#include "Physics/PhysicsWorld.h"
#include "Physics/PhysicsConstraint.h"
#include "Scene/Scene.h"

namespace fw {

PhysicsRevoluteComponent::PhysicsRevoluteComponent()
{
}

PhysicsRevoluteComponent::~PhysicsRevoluteComponent()
{
    delete m_pConstraint;
}

void PhysicsRevoluteComponent::Save(WriterType& writer)
{
    Component::Save(writer);
}

void PhysicsRevoluteComponent::Load(rapidjson::Value& component)
{
}

void PhysicsRevoluteComponent::AddToInspector()
{
    bool motorEnabled = m_pConstraint->IsMotorEnabled();

    if (ImGui::Checkbox("Motor Enabled", &motorEnabled))
    {
        m_pConstraint->SetMotorEnabled(motorEnabled);
    }
}

void PhysicsRevoluteComponent::Init()
{
    PhysicsWorld* pWorld = m_pGameObject->GetScene()->GetWorld();

    Component* pComponent = m_pGameObject->GetFirstComponentOfType(CollisionComponent::GetStaticType());

    if (pComponent)
    {
        fw::CollisionComponent* pCollider = static_cast<fw::CollisionComponent*>(pComponent);
        m_pConstraint = pWorld->CreateConstraint(pCollider->GetPhysicsBody(), nullptr, vec3(5, 7));
    }
}

} // namespace fw
