#include "FrameworkPCH.h"

#include "MeshComponent.h"
#include "Core/GameCore.h"
#include "Objects/GameObject.h"
#include "Objects/Mesh.h"
#include "Objects/ResourceManager.h"
#include "UI/ResourcesPanel.h"
#include "Scene/Scene.h"
#include "EventSystem/Event.h"
#include "EventSystem/EventManager.h"

namespace fw {

MeshComponent::~MeshComponent()
{
    m_pGameObject->GetScene()->GetGameCore()->GetEventManager()->UnregisterEventListenerCompletely(this);
}

void MeshComponent::Save(WriterType& writer)
{

    Component::Save(writer);

    ResourceManager* pResources = m_pGameObject->GetScene()->GetGameCore()->GetResourceManager();

    JSONSaveCharArray(writer, "Mesh", pResources->FindMeshName(m_pMesh).c_str());
    JSONSaveCharArray(writer, "Material", pResources->FindMaterialName(m_pMaterial).c_str());
    
    writer.Key("Hidden");
    writer.Bool(m_Hidden);

    writer.Key("HideOnPlay");
    writer.Bool(m_HideOnPlay);

    //writer.Key("Verts");
    //writer.StartArray();
    //{
    //    if (m_pMesh != nullptr)
    //    {
    //        for (int i = 0; i < m_pMesh->GetVerts().size(); i++)
    //        {
    //            VertexFormat vertex = m_pMesh->GetVerts().at(i);
    //            writer.StartArray();
    //            {
    //                writer.Double(vertex.x);
    //                writer.Double(vertex.y);
    //                writer.Double(vertex.z);
    //                writer.Double(vertex.u);
    //                writer.Double(vertex.v);
    //                writer.EndArray();
    //            }
    //        }
    //    }
    //    writer.EndArray();
    //}
}


void MeshComponent::Load(rapidjson::Value& component)
{
    ResourceManager* pResources = m_pGameObject->GetScene()->GetGameCore()->GetResourceManager();

    if (component.HasMember("Mesh"))
    {
        m_pMesh = pResources->GetMesh(component["Mesh"].GetString());
    }

    if (component.HasMember("Material"))
    {
        m_pMaterial = pResources->GetMaterial(component["Material"].GetString());
    }

    if (component.HasMember("Hidden"))
    {
        m_Hidden = component["Hidden"].GetBool();
    }

    if (component.HasMember("HideOnPlay"))
    {
        m_HideOnPlay = component["HideOnPlay"].GetBool();

        if (m_HideOnPlay)
        {
            m_pGameObject->GetScene()->GetGameCore()->GetEventManager()->RegisterEventListener(this, BeginPlayEvent::GetStaticEventType());
            m_pGameObject->GetScene()->GetGameCore()->GetEventManager()->RegisterEventListener(this, EndPlayEvent::GetStaticEventType());
            m_RegisteredAsListener = true;
        }
    }
}

void MeshComponent::OnEvent(Event* pEvent)
{
    if (m_HideOnPlay)
    {
        if (pEvent->GetType() == BeginPlayEvent::GetStaticEventType())
        {
            m_Hidden = true;
        } 
        else if (pEvent->GetType() == EndPlayEvent::GetStaticEventType())
        {
            m_Hidden = false;
        }
    }
}

void MeshComponent::SetHiddenOnPlay(bool hidden)
{
    if (!m_RegisteredAsListener)
    {
        m_pGameObject->GetScene()->GetGameCore()->GetEventManager()->RegisterEventListener(this, BeginPlayEvent::GetStaticEventType());
        m_pGameObject->GetScene()->GetGameCore()->GetEventManager()->RegisterEventListener(this, EndPlayEvent::GetStaticEventType());
        m_RegisteredAsListener = true;
    }

    m_HideOnPlay = hidden;
}

void MeshComponent::AddToInspector()
{
    ResourcesPanel::DropNodeMaterial(&m_pMaterial, m_pGameObject);
    ResourcesPanel::DropNodeMesh(&m_pMesh, m_pGameObject);
    ImGui::Checkbox("Hidden", &m_Hidden);
}

void MeshComponent::Init()
{
}

} // namespace fw
