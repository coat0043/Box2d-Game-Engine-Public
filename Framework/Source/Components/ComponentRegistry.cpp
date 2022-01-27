#include "FrameworkPCH.h"

#include "ComponentRegistry.h"
#include "Components/ComponentManager.h"
#include "Components/AABBComponent.h"
#include "Components/CameraComponent.h"
#include "Components/CollisionComponent.h"
#if FW_USING_LUA
#include "Components/LuaScriptComponent.h"
#endif
#include "Components/AudioComponent.h"
#include "Components/AudioCueComponent.h"
#include "Components/MeshComponent.h"
#include "Components/ParticleEmitterComponent.h"
#include "Components/PhysicsRevoluteComponent.h"
#include "Components/TransformComponent.h"
#include "Components/PointLightComponent.h"
#include "Components/SpotLightComponent.h"
#include "Components/DirectionalLightComponent.h"
#include "Objects/GameObject.h"
#include "Objects/Mesh.h"
#include "Objects/AABB.h"
#include "Scene/Scene.h"

namespace fw {

ComponentRegistry::ComponentRegistry()
{
    Register(AudioComponent::GetStaticType(), &AudioComponent::Create);
    Register(AudioCueComponent::GetStaticType(), &AudioCueComponent::Create);
    Register(MeshComponent::GetStaticType(), &MeshComponent::Create);
    Register(AABBComponent::GetStaticType(), &AABBComponent::Create);
    Register(TransformComponent::GetStaticType(), &TransformComponent::Create);
    Register(CameraComponent::GetStaticType(), &CameraComponent::Create);
    Register(CollisionComponent::GetStaticType(), &CollisionComponent::Create);
    Register(ParticleEmitterComponent::GetStaticType(), &ParticleEmitterComponent::Create);
    Register(PhysicsRevoluteComponent::GetStaticType(), &PhysicsRevoluteComponent::Create);
    Register(PointLightComponent::GetStaticType(), &PointLightComponent::Create);
    Register(SpotLightComponent::GetStaticType(), &SpotLightComponent::Create);
    Register(DirectionalLightComponent::GetStaticType(), &DirectionalLightComponent::Create);


#if FW_USING_LUA
    Register(LuaScriptComponent::GetStaticType(), &LuaScriptComponent::Create);
#endif
}

void ComponentRegistry::Register(const std::string& Name, CreateComponentFn pfnCreate)
{
    m_ComponentTypes[Name] = pfnCreate;
}

Component* ComponentRegistry::CreateComponent(const std::string& componentName)
{
    std::map<const std::string, CreateComponentFn>::iterator it = m_ComponentTypes.find(componentName);
    if (it != m_ComponentTypes.end())
    {
        return it->second();
    }
    return nullptr;
}

void ComponentRegistry::AddInterfaceToCreateComponents(Scene* pScene, GameObject* pObject)
{
    for (auto& entry : m_ComponentTypes)
    {
        if (ImGui::BeginMenu("Add Component"))
        {
            if (ImGui::MenuItem(entry.first.c_str()))
            {
                Component* pComponent = entry.second();
                pObject->AddComponent(pComponent);

                if (pComponent->GetType() == CameraComponent::GetStaticType())
                {
                    pScene->SetActiveCameraComponent(static_cast<CameraComponent*>(pComponent));
                }

                LOG(INFO, "%s added to Object %s ", entry.first.c_str(), pObject->GetName().c_str());
            }
            ImGui::EndMenu();
        }
    }
}

} // namespace fw
