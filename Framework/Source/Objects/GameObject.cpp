#include "FrameworkPCH.h"

#include "GameObject.h"
#include "Core/GameCore.h"
#include "Components/AABBComponent.h"
#include "Components/CameraComponent.h"
#include "Components/CollisionComponent.h"
#include "Components/ComponentManager.h"
#include "Components/ComponentRegistry.h"
#include "Components/MeshComponent.h"
#include "Components/TransformComponent.h"
#include "EventSystem/EventManager.h"
#include "Objects/AABB.h"
#include "Objects/Material.h"
#include "Objects/Mesh.h"
#include "Objects/ResourceManager.h"
#include "Physics/PhysicsBody.h"
#include "Scene/Scene.h"
#include "UI/ResourcesPanel.h"
#include "Utility/JSONHelpers.h"

namespace fw {

GameObject::GameObject(Scene* pScene, std::string name)
{
    m_pScene = pScene;
    m_Name = name;
}

GameObject::GameObject(GameObject& orgnlGO)
{
    *this = orgnlGO;
    m_Components.clear();

    SetName(std::string("New ") + GetName().c_str());

    for (int i = 0; i < orgnlGO.m_Components.size(); i++)
    {
        if (orgnlGO.m_Components[i]->GetType() == TransformComponent::GetStaticType())
        {
            TransformComponent* cComponent = static_cast<TransformComponent*>(orgnlGO.m_Components[i]);

            TransformComponent* nComponent = new TransformComponent();
            nComponent->SetPosition(cComponent->GetPosition());
            nComponent->SetRotation(cComponent->GetRotation());
            nComponent->SetScale(cComponent->GetScale());

            this->AddComponent(nComponent);
        }
        if (orgnlGO.m_Components[i]->GetType() == MeshComponent::GetStaticType())
        {
            MeshComponent* cComponent = static_cast<MeshComponent*>(orgnlGO.m_Components[i]);
            this->AddComponent(new MeshComponent(cComponent->GetMesh(), cComponent->GetMaterial()));
        }
        if (orgnlGO.m_Components[i]->GetType() == AABBComponent::GetStaticType())
        {
            AABBComponent* cComponent = static_cast<AABBComponent*>(orgnlGO.m_Components[i]);
            //this->AddComponent(new AABBComponent(cComponent->GetAABB()));
        }
        if (orgnlGO.m_Components[i]->GetType() == CollisionComponent::GetStaticType())
        {
            CollisionComponent* cComponent = static_cast<CollisionComponent*>(orgnlGO.m_Components[i]);
            this->AddComponent(new CollisionComponent(cComponent));
        }
    }
}

GameObject::~GameObject()
{
    if (m_pScene != nullptr)
    {
        m_pScene->GetGameCore()->GetEventManager()->UnregisterEventListenerCompletely(this);

        for (Component* pComp : m_Components)
        {
            m_pScene->GetComponentManager()->RemoveComponent(pComp);
            delete pComp;
        }
    }
}

void GameObject::Update(float deltaTime)
{
}

void GameObject::Draw(CameraComponent* pCamera)
{
    //m_pMesh->Draw( pCamera, m_Position, m_pShader, m_pTexture, m_Color, m_UVScale, m_UVOffset );
}

void GameObject::Save(WriterType& writer)
{
    ResourceManager* pResources = m_pScene->GetGameCore()->GetResourceManager();

    writer.Key("Name");
    writer.String(m_Name.c_str());

    if (!m_Components.empty())
    {
        writer.Key("GameObjectComponents");
        writer.StartArray();

        for (Component* pComponent : m_Components)
        {
            writer.StartObject();
            pComponent->Save(writer);
            writer.EndObject();
        }

        writer.EndArray();
    }
}

void GameObject::Load(rapidjson::Value& jsonObject)
{
    if (jsonObject.HasMember("GameObjectComponents"))
    {
        for (rapidjson::Value& component : jsonObject["GameObjectComponents"].GetArray())
        {
            std::string componentType = component["Type"].GetString();
            Component* pComponent = m_pScene->GetGameCore()->GetComponentRegistry()->CreateComponent(componentType);
            pComponent->SetGameObject(this);
            pComponent->Load(component);
            AddComponent(pComponent);

            if (componentType == CameraComponent::GetStaticType())
            {
                m_pScene->SetActiveCameraComponent(static_cast<CameraComponent*>(pComponent));
            }
        }
    }
}

void GameObject::FinalizeLoad(rapidjson::Value& jsonObject)
{
    if (jsonObject.HasMember("GameObjectComponents"))
    {
        int count = 0;
        for (rapidjson::Value& component : jsonObject["GameObjectComponents"].GetArray())
        {
            m_Components[count]->FinalizeLoad(component);
            count++;
        }
    }
}

void GameObject::AddComponent(Component* pComponent)
{
    //// Make sure there is only 1 Transform component.
    //assert(pComponent->GetType() != TransformComponent::GetStaticType() || GetTransform() == nullptr);

    pComponent->SetGameObject(this);
    pComponent->Init();
    if (m_AreComponentsRegistered)
    {
        m_pScene->GetComponentManager()->AddComponent(pComponent);
    }
    m_Components.push_back(pComponent);
}

void GameObject::RemoveAndDeleteComponent(Component* pComponent)
{
    auto& it = std::find(m_Components.begin(), m_Components.end(), pComponent);
    if (it != m_Components.end())
    {
        m_Components.erase(it);
    }
    m_pScene->GetComponentManager()->RemoveComponent(pComponent);
    delete pComponent;
}

void GameObject::RegisterAllComponents()
{
    if (m_AreComponentsRegistered)
        return;

    // Enable physics body.
    Component* pComponent = GetFirstComponentOfType(fw::CollisionComponent::GetStaticType());
    if (pComponent)
    {
        fw::CollisionComponent* pCollider = static_cast<fw::CollisionComponent*>(pComponent);
        pCollider->GetPhysicsBody()->SetEnabled(true);
    }

    for (Component* pComponent : m_Components)
    {
        m_pScene->GetComponentManager()->AddComponent(pComponent);
    }

    m_AreComponentsRegistered = true;
}

void GameObject::UnregisterAllComponents()
{
    if (m_AreComponentsRegistered == false)
        return;

    // Disable physics body.
    Component* pComponent = GetFirstComponentOfType(fw::CollisionComponent::GetStaticType());
    if (pComponent)
    {
        fw::CollisionComponent* pCollider = static_cast<fw::CollisionComponent*>(pComponent);
        pCollider->GetPhysicsBody()->SetEnabled(false);
    }

    for (Component* pComponent : m_Components)
    {
        m_pScene->GetComponentManager()->RemoveComponent(pComponent);
    }

    m_AreComponentsRegistered = false;
}

Component* GameObject::GetFirstComponentOfType(const char* type)
{
    for (Component* pComponent : m_Components)
    {
        if (pComponent->GetType() == type)
        {
            return pComponent;
        }
    }
    
    return nullptr;
}

TransformComponent* GameObject::GetTransform()
{
    Component* pComponent = GetFirstComponentOfType(TransformComponent::GetStaticType());
    return static_cast<TransformComponent*>(pComponent);
}

void GameObject::AddToInspector()
{
    // Display name and allow for renaming.
    ImGui::BeginGroup();
    {
        const int maxNameLen = 32;
        char text[maxNameLen];
        strncpy_s(text, maxNameLen, m_Name.c_str(), maxNameLen);
        if (ImGui::InputText("Name", text, maxNameLen))
        {
            m_Name = text;
        }
    }

    Component* pComponentToDelete = nullptr;
    for (Component* pComponent : m_Components)
    {
        ImGui::PushID(pComponent);
        bool expanded = ImGui::CollapsingHeader(pComponent->GetType(), ImGuiTreeNodeFlags_DefaultOpen);
        if (ImGui::BeginPopupContextItem(pComponent->GetType()))
        {
            if (ImGui::MenuItem("Remove"))
            {
                pComponentToDelete = pComponent;
            }
            ImGui::EndPopup();
        }
        if (expanded)
        {
            pComponent->AddToInspector();
        }
        ImGui::PopID();
    }

    if (pComponentToDelete)
    {
        RemoveAndDeleteComponent(pComponentToDelete);
    }

    if (m_Components.empty())
    {
        ImGui::Text("Object has no Components.");
    }

    ImGui::EndGroup();
}

#if FW_USING_LUA
void GameObject::LuaRegister(lua_State* luastate)
{
    luabridge::getGlobalNamespace(luastate)
        .beginClass<GameObject>("GameObject")
        //.addProperty("position", &GameObject::m_Position)
        .endClass();
}
#endif

} // namespace fw
