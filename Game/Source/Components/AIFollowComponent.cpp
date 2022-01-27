#include "GamePCH.h"

#include "AIFollowComponent.h"
#include "Game.h"

AIFollowComponent::AIFollowComponent()
{
}

AIFollowComponent::~AIFollowComponent()
{
}

void AIFollowComponent::Save(fw::WriterType& writer)
{
    Component::Save(writer);
}

void AIFollowComponent::Load(rapidjson::Value& component)
{
}

void AIFollowComponent::AddToInspector()
{
}

void AIFollowComponent::Init()
{
    m_pTarget = m_pGameObject->GetScene()->GetGameObjectByName("Player");
}

void AIFollowComponent::Update(float deltaTime)
{
    if (m_pTarget == nullptr)
        return;

    fw::TransformComponent* pTransform = m_pGameObject->GetTransform();

    // Target position - my position.
    m_Direction = m_pTarget->GetTransform()->GetPosition() - pTransform->GetPosition();

    m_Direction.Normalize();
    pTransform->SetPosition(pTransform->GetPosition() + m_Direction * m_Speed * deltaTime);
}

void AIFollowComponent::SetObjectToFollow(fw::GameObject* pGameObject)
{
    m_pTarget = pGameObject;
}
