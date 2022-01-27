#include "GamePCH.h"

#include "DirectMovementComponent.h"
#include "Game.h"
#include "Objects/PlayerController.h"

DirectMovementComponent::DirectMovementComponent()
{
}

DirectMovementComponent::~DirectMovementComponent()
{
}

void DirectMovementComponent::Save(fw::WriterType& writer)
{
    Component::Save(writer);
}

void DirectMovementComponent::Load(rapidjson::Value& component)
{
}

void DirectMovementComponent::AddToInspector()
{
}

void DirectMovementComponent::Init()
{
    Game* pGame = (Game*)(m_pGameObject->GetScene()->GetGameCore());
    pGame->GetPlayerController();
    SetPlayerController(pGame->GetPlayerController());
}

void DirectMovementComponent::Update(float deltaTime)
{
    if (m_pPlayerController == nullptr)
        return;

    vec2 dir = vec2(0.f, 0.f);

    if (m_pPlayerController->IsHeld(PlayerController::Mask::Up))
    {
        dir.y += 1;
    }
    if (m_pPlayerController->IsHeld(PlayerController::Mask::Down))
    {
        dir.y += -1;
    }
    if (m_pPlayerController->IsHeld(PlayerController::Mask::Left))
    {
        dir.x += -1;
    }
    if (m_pPlayerController->IsHeld(PlayerController::Mask::Right))
    {
        dir.x += 1;
    }

    dir.Normalize();

    fw::TransformComponent* pTransform = m_pGameObject->GetTransform();
    pTransform->SetPosition(pTransform->GetPosition() + dir * m_Speed * deltaTime);
}

void DirectMovementComponent::SetPlayerController(PlayerController* pController)
{
    m_pPlayerController = pController;
}
