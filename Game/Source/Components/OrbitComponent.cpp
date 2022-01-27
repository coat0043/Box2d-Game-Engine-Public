#include "GamePCH.h"

#include "OrbitComponent.h"
#include "Game.h"
#include "Objects/PlayerController.h"

OrbitComponent::OrbitComponent()
{
}

OrbitComponent::~OrbitComponent()
{
}

void OrbitComponent::Save(fw::WriterType& writer)
{
    Component::Save(writer);
}

void OrbitComponent::Load(rapidjson::Value& component)
{
}

void OrbitComponent::FinalizeLoad(rapidjson::Value& component)
{
    m_pObjectFollowing = m_pGameObject->GetScene()->GetGameObjectByName("Player");
}

void OrbitComponent::AddToInspector()
{
    if (m_pObjectFollowing)
    {
        ImGui::Text("Following: %s", m_pObjectFollowing->GetName().c_str());
    }
    else
    {
        ImGui::Text("Not following anything");
    }
    ImGui::DragFloat("Distance", &m_Distance, 0.05f);
    ImGui::DragFloat3("Offset", &m_Offset.x, 0.05f);
}

void OrbitComponent::Init()
{
    Game* pGame = (Game*)(m_pGameObject->GetScene()->GetGameCore());
    pGame->GetPlayerController();
    SetPlayerController(pGame->GetPlayerController());
}

void OrbitComponent::Update(float deltaTime)
{
    if (m_pPlayerController == nullptr)
        return;

    if (m_pObjectFollowing == nullptr)
        return;

    fw::TransformComponent* pTransform = m_pGameObject->GetTransform();
    if (pTransform == nullptr)
        return;

    fw::TransformComponent* pFollowTransform = m_pObjectFollowing->GetTransform();
    if (pFollowTransform == nullptr)
        return;

    vec3 rot = pTransform->GetRotation();
    vec3 rotRadians = rot * PI / 180.0f;

    if( false )
    {
        // Same as code below, but using matrices.
        // This is a *lot* more operations, but might be more readable.

        // Rotate the offset around the Y-axis.
        vec3 pivotPos = pFollowTransform->GetPosition();
        pivotPos.x += m_Offset.x*cos(rotRadians.y) - m_Offset.z*sin(rotRadians.y);
        pivotPos.y += m_Offset.y;
        pivotPos.z += m_Offset.x*sin(rotRadians.y) + m_Offset.z*cos(rotRadians.y);

        mat4 mat;
        mat.SetIdentity();
        mat.Translate( 0, 0, -m_Distance );
        mat.Rotate( rot.x, 1, 0, 0 );
        mat.Rotate( rot.y, 0, 1, 0 );
        mat.Translate( pivotPos );

        vec3 pos(0,0,0);
        pos = mat * pos;
    
        pTransform->SetPosition( pos );
    }
    else
    {
        float turnSpeed = 180.0f;

        // Calculate the point on the sphere based on the rotation of our transform.
        float cx = cos( rotRadians.x );
        float cy = cos( rotRadians.y );
        float sx = sin( rotRadians.x );
        float sy = sin( rotRadians.y );
        vec3 dir = vec3( cx*sy, -sx, -cx*cy );
        vec3 pos = dir * m_Distance;

        // Rotate the offset around the Y-axis.
        vec3 pivotPos = pFollowTransform->GetPosition();
        pivotPos.x += m_Offset.x*cy - m_Offset.z*sy;
        pivotPos.y += m_Offset.y;
        pivotPos.z += m_Offset.x*sy + m_Offset.z*cy;
        pos += pivotPos;

        pTransform->SetPosition( pos );
    }
}

void OrbitComponent::SetPlayerController(PlayerController* pController)
{
    m_pPlayerController = pController;
}
