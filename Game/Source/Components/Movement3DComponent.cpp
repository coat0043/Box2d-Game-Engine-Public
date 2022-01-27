#include "GamePCH.h"

#include "Movement3DComponent.h"
#include "Game.h"
#include "Objects/PlayerController.h"

Movement3DComponent::Movement3DComponent()
{
}

Movement3DComponent::~Movement3DComponent()
{
}

void Movement3DComponent::Save(fw::WriterType& writer)
{
    Component::Save(writer);
}

void Movement3DComponent::Load(rapidjson::Value& component)
{
}

void Movement3DComponent::AddToInspector()
{
    ImGui::DragFloat("Speed", &m_Speed);
    ImGui::DragFloat("Turn speed", &m_TurnSpeed);
}

void Movement3DComponent::Init()
{
    Game* pGame = (Game*)(m_pGameObject->GetScene()->GetGameCore());
    pGame->GetPlayerController();
    SetPlayerController(pGame->GetPlayerController());
}

void Movement3DComponent::Update(float deltaTime)
{
    if (m_pPlayerController == nullptr)
        return;

    fw::TransformComponent* pTransform = m_pGameObject->GetTransform();

    vec2 controls = vec2(0.f, 0.f);

    if (m_pPlayerController->IsHeld(PlayerController::Mask::Up))
    {
        controls.y += 1;
    }
    if (m_pPlayerController->IsHeld(PlayerController::Mask::Down))
    {
        controls.y -= 1;
    }
    if (m_pPlayerController->IsHeld(PlayerController::Mask::Left))
    {
        controls.x += 1;
    }
    if (m_pPlayerController->IsHeld(PlayerController::Mask::Right))
    {
        controls.x -= 1;
    }

    // Get scene camera's rotation values.
    vec3 camRot = 0;
    fw::CameraComponent* pCamera = m_pGameObject->GetScene()->GetActiveCameraComponent();
    if (pCamera)
    {
        camRot = pCamera->GetGameObject()->GetTransform()->GetRotation();
    }

    // Snap object rotation to camera rotation.
    vec3 rot = pTransform->GetRotation();
    rot.y = camRot.y;
    pTransform->SetRotation(rot);

    // Move relative to the scene's camera.
    camRot.y += 90.0f;
    camRot *= PI / 180.0f;
    vec3 dir(cos(camRot.y), 0, sin(camRot.y));
    pTransform->SetPosition(pTransform->GetPosition() + dir * controls.y * m_Speed * deltaTime);
    dir.Set(-sin(camRot.y), 0, cos(camRot.y));
    pTransform->SetPosition(pTransform->GetPosition() + dir * controls.x * m_Speed * deltaTime);
}

void Movement3DComponent::SetPlayerController(PlayerController* pController)
{
    m_pPlayerController = pController;
}
