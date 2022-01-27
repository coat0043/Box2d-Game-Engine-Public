#include "FrameworkPCH.h"

#include "ComponentManager.h"
#include "Components/AudioComponent.h"
#include "Components/AudioCueComponent.h"
#include "Components/AABBComponent.h"
#include "Components/CameraComponent.h"
#include "Components/CollisionComponent.h"
#include "Components/LuaScriptComponent.h"
#include "Components/MeshComponent.h"
#include "Components/PointLightComponent.h"
#include "Components/SpotLightComponent.h"
#include "Components/DirectionalLightComponent.h"
#include "Components/ParticleEmitterComponent.h"
#include "Components/TransformComponent.h"
#include "Lua/LuaGameState.h"
#include "Objects/GameObject.h"
#include "Objects/Mesh.h"
#include "Objects/AABB.h"
#include "Particles/ParticleEmitter.h"
#include "Scene/Scene.h"
#include "Utility/FrameBufferObject.h"

namespace fw {

ComponentManager::ComponentManager()
{
}

ComponentManager::~ComponentManager()
{
    delete m_GBO;
    Mesh::ClearDeferredRenderTextures();
}

void ComponentManager::Draw(CameraComponent* pCamera)
{
    DrawMeshComponents(pCamera);
    DrawParticleEmitterComponents(pCamera);
    DrawDebugAABBComponents(pCamera);
}

void ComponentManager::Update(float deltaTime)
{
    std::vector<Component*>& list = m_Components[AABBComponent::GetStaticType()];
    for (int i = 0; i < list.size(); i++)
    {
        AABBComponent* pAABBComponent = static_cast<AABBComponent*>(list[i]); 
    }

#if FW_USING_LUA
    UpdateLuaScriptComponents(deltaTime);
#endif

    std::vector<Component*>& collisionComponentsList = m_Components[CollisionComponent::GetStaticType()];
    for (int i = 0; i < collisionComponentsList.size(); i++)
    {
        CollisionComponent* pCollisionComponent = static_cast<CollisionComponent*>(collisionComponentsList[i]);

        pCollisionComponent->Update(deltaTime);
    }

    std::vector<Component*>& AudioCueComponentList = m_Components[AudioCueComponent::GetStaticType()];
    for (int i = 0; i < AudioCueComponentList.size(); i++)
    {
        AudioCueComponent* pAudioCueComponent = static_cast<AudioCueComponent*>(collisionComponentsList[i]);
        pAudioCueComponent->Update(deltaTime);
    }

    UpdateParticleEmitterComponents(deltaTime);
}

void ComponentManager::AddToInspector()
{
    std::vector<Component*>& collisionComponentsList = m_Components[CollisionComponent::GetStaticType()];
    for (int i = 0; i < collisionComponentsList.size(); i++)
    {
        CollisionComponent* pCollisionComponent = static_cast<CollisionComponent*>(collisionComponentsList[i]);

        pCollisionComponent->AddToInspector();
    }
}

void ComponentManager::AddComponent(Component* pComponent)
{
    assert(pComponent != nullptr);

    const char* type = pComponent->GetType();

    assert(std::find(m_Components[type].begin(), m_Components[type].end(), pComponent) == m_Components[type].end());

    m_Components[type].push_back(pComponent);
}

void ComponentManager::RemoveComponent(Component* pComponent)
{
    const char* type = pComponent->GetType();

    std::vector<Component*>& list = m_Components[type];
    list.erase(std::remove(list.begin(), list.end(), pComponent), list.end());
}

std::vector<Component*>& ComponentManager::GetComponentList(const char* type)
{
    return m_Components[type];
}

void ComponentManager::UpdateParticleEmitterComponents(float deltaTime)
{
    std::vector<Component*>& list = m_Components[ParticleEmitterComponent::GetStaticType()];
    for (int i = 0; i < list.size(); i++)
    {
        ParticleEmitterComponent* pEmitterComponent = static_cast<ParticleEmitterComponent*>(list[i]);

        ParticleEmitter* pEmitter = pEmitterComponent->GetEmitter();
        pEmitter->Update(deltaTime);
    }

}

void ComponentManager::DrawMeshComponents(CameraComponent* pCamera)
{

        std::vector<Component*>& list = m_Components[MeshComponent::GetStaticType()];
        std::vector<Component*> pAllLights = m_Components[PointLightComponent::GetStaticType()];
        pAllLights.insert(pAllLights.end(), m_Components[SpotLightComponent::GetStaticType()].begin(), m_Components[SpotLightComponent::GetStaticType()].end());
        pAllLights.insert(pAllLights.end(), m_Components[DirectionalLightComponent::GetStaticType()].begin(), m_Components[DirectionalLightComponent::GetStaticType()].end());

        std::vector<MeshComponent*> deferredComps;
        std::vector<MeshComponent*> forwardComps;

        // Store the original FBO and restore it when we're done.
        GLint OriginalFBOHandle;
        glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &OriginalFBOHandle);

        // Store the original ClearColor and restore it when we're done.
        float bgColor[4] {};
        glGetFloatv(GL_COLOR_CLEAR_VALUE, bgColor);

        glClearColor(0, 0, 0, 1);

        if (m_GBO == nullptr) // Setup the GBO if we never have. This helps run this whole thing only once.
        {
            m_GBO = new FrameBufferObject(0,0);
            m_GBO->Bind();
        }
        else // We already have a GBO. Bind it and clear out old data from it.
        {
            m_GBO->Bind();
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        }

        for (int i = 0; i < list.size(); i++)
        {
            MeshComponent* pMeshComponent = static_cast<MeshComponent*>(list[i]);
            GameObject* pGO = pMeshComponent->GetGameObject();
                
            if ( pGO != nullptr && pGO->GetTransform() && pMeshComponent->GetMesh() && !pMeshComponent->GetIsHidden() && pMeshComponent->GetMaterial() )
            {
                // If we have a buffer shader defined in the material, the model is intended to be rendered in the deferred pipeline.
                if (pMeshComponent->GetMaterial()->GetDeferredBufferShader()) 
                {
                    deferredComps.push_back(pMeshComponent);
                    pMeshComponent->GetMesh()->DeferredRender_GeometryPass( pCamera, pGO->GetTransform(), pMeshComponent->GetMaterial() );
                }
                else // Regular forward render.
                {
                    forwardComps.push_back(pMeshComponent);
                }
            }
        }

        // Restore the FBO handle.
        glBindFramebuffer(GL_FRAMEBUFFER, OriginalFBOHandle);

        // Restore the original ClearColor.
        glClearColor(bgColor[0], bgColor[1], bgColor[2], bgColor[3]);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Calculate lighting on all deferred meshes.
        for (size_t i = 0; i < deferredComps.size(); i++)
        {
            deferredComps[i]->GetMesh()->DeferredRender_LightingPass(pCamera, &pAllLights, deferredComps[i]->GetMaterial()->GetShader());
        }

        // Copy the depth buffer from our deferred render down to the forward render.
        if (deferredComps.size() > 0)
        {
            glBindFramebuffer(GL_READ_FRAMEBUFFER, m_GBO->GetFramebufferHandle());
            glBindFramebuffer(GL_DRAW_FRAMEBUFFER, OriginalFBOHandle);
            glBlitFramebuffer(0, 0, pCamera->GetWindowSize().x, pCamera->GetWindowSize().y, 0, 0, pCamera->GetWindowSize().x, pCamera->GetWindowSize().y, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
            
            glBindFramebuffer(GL_FRAMEBUFFER, OriginalFBOHandle);
        }

        // Copy the depth buffer from our deferred render down to the forward render.
        for (size_t i = 0; i < forwardComps.size(); i++)
        {
            forwardComps[i]->GetMesh()->Draw(pCamera, forwardComps[i]->GetGameObject()->GetTransform(), forwardComps[i]->GetMaterial(), &pAllLights);
        }

        // Restore the original FBO.
        glBindFramebuffer(GL_FRAMEBUFFER, OriginalFBOHandle);

        // Do error checking for both the deferred and forward rendering here if you need to.
        // assert(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
        // assert(glGetError() == GL_NO_ERROR);
}

void ComponentManager::DrawParticleEmitterComponents(CameraComponent* pCamera)
{
    std::vector<Component*>& list = m_Components[ParticleEmitterComponent::GetStaticType()];
    for (int i = 0; i < list.size(); i++)
    {
        ParticleEmitterComponent* pEmitterComponent = static_cast<ParticleEmitterComponent*>(list[i]);

        GameObject* pGO = pEmitterComponent->GetGameObject();
        ParticleEmitter* pEmitter = pEmitterComponent->GetEmitter();
        pEmitter->Draw(pCamera, pGO->GetTransform()->GetPosition());
    }
}

void ComponentManager::DrawDebugAABBComponents(CameraComponent* pCamera)
{
    std::vector<Component*>& list = m_Components[AABBComponent::GetStaticType()];
    for (int i = 0; i < list.size(); i++)
    {
        AABBComponent* pAABBComponent = static_cast<AABBComponent*>(list[i]);

        GameObject* pGO = pAABBComponent->GetGameObject();
        if( pAABBComponent->GetMesh() )
        {
            //pAABBComponent->GetMesh()->Draw(pCamera,
            //                                pGO->GetTransform(),
            //                                pGO->GetMaterial());
        }
    }
}

#if FW_USING_LUA
void ComponentManager::UpdateLuaScriptComponents(float deltaTime)
{
    std::vector<Component*>& list = m_Components[LuaScriptComponent::GetStaticType()];
    for (int i = 0; i < list.size(); i++)
    {
        LuaScriptComponent* pLuaComponent = static_cast<LuaScriptComponent*>(list[i]);

        GameObject* pGO = pLuaComponent->GetGameObject();
        pGO->GetScene()->GetLuaState()->UpdateGameObjectFromLua(pGO, deltaTime, "Tick");
    }
}
#endif

} // namespace fw
