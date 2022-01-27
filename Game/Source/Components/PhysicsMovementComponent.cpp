#include "GamePCH.h"
#include "Game.h"
#include "PhysicsMovementComponent.h"
#include "Objects/PlayerController.h"

PhysicsMovementComponent::PhysicsMovementComponent()
{
}

PhysicsMovementComponent::~PhysicsMovementComponent()
{
    m_pGameObject->GetScene()->GetGameCore()->GetEventManager()->UnregisterEventListener(this, fw::PhysicsCollisionEvent::GetStaticEventType());
}

void PhysicsMovementComponent::Save(fw::WriterType& writer)
{
    Component::Save(writer);
}

void PhysicsMovementComponent::Load(rapidjson::Value& component)
{
}

void PhysicsMovementComponent::AddToInspector()
{
}

void PhysicsMovementComponent::Init()
{
    Game* pGame = (Game*)(m_pGameObject->GetScene()->GetGameCore());
    pGame->GetPlayerController();
    SetPlayerController(pGame->GetPlayerController());

    if (m_pGameObject->GetFirstComponentOfType(fw::CollisionComponent::GetStaticType()) == nullptr)
    {
        m_pGameObject->AddComponent(new fw::CollisionComponent(vec3(0, 0, 0), fw::ShapeType::Box, fw::BodyType::DynamicBody, 1));
    }

    if (m_pGameObject->GetScene() != nullptr)
    {
        m_pGameObject->GetScene()->GetGameCore()->GetEventManager()->RegisterEventListener(this, fw::PhysicsCollisionEvent::GetStaticEventType());
    }
}

void PhysicsMovementComponent::Update(float deltaTime)
{
    fw::Component* pComponent = m_pGameObject->GetFirstComponentOfType(fw::CollisionComponent::GetStaticType());
    if( pComponent == nullptr )
        return;

    fw::CollisionComponent* pCollider = static_cast<fw::CollisionComponent*>(pComponent);
    fw::PhysicsBody* pBody = pCollider->GetPhysicsBody();

    if (m_pPlayerController->IsHeld(PlayerController::Mask::Right))
    {
        pBody->ApplyForceToCenter(vec3(1, 0, 0) * m_Speed);
    }

    if (m_pPlayerController->IsHeld(PlayerController::Mask::Left))
    {
        pBody->ApplyForceToCenter(vec3(-1, 0, 0) * m_Speed);
    }

    if (m_pPlayerController->WasNewlyPressed(PlayerController::Mask::Up))
    {
        if (CanJump() == true)
        {
            pBody->ApplyImpulseToCenter(vec3(0, 1, 0) * m_Speed);
            m_AllowedToJump = false;
        }
    }
}

void PhysicsMovementComponent::SetPlayerController(PlayerController* pController)
{
    m_pPlayerController = pController;
}

void PhysicsMovementComponent::OnEvent(fw::Event* pEvent)
{
    if (pEvent->GetType() == fw::PhysicsCollisionEvent::GetStaticEventType())
    {
        m_AllowedToJump = true;

    }
}

bool PhysicsMovementComponent::CanJump()
{
    return m_AllowedToJump;
}
