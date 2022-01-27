#include "GamePCH.h"

#include "ExampleComponent.h"

void ExampleComponent::Save(fw::WriterType& writer)
{
}

void ExampleComponent::Load(rapidjson::Value& component)
{
}

void ExampleComponent::AddToInspector()
{
    ImGui::Text("TESTING");
}

void ExampleComponent::Update(float deltaTime)
{
    fw::Component* pComponent = m_pGameObject->GetFirstComponentOfType(fw::CollisionComponent::GetStaticType());

    if (pComponent)
    {
        fw::CollisionComponent* pCollider = static_cast<fw::CollisionComponent*>(pComponent);

        float mass = pCollider->GetPhysicsBody()->GetMass();
        pCollider->GetPhysicsBody()->ApplyForceToCenter(vec3(0, 10, 0));
    }
}
