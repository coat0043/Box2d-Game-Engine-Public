#include "FrameworkPCH.h"

#include "ParticleEmitterComponent.h"
#include "Particles/ParticleEmitter.h"
#include "Core/GameCore.h"
#include "EventSystem/Event.h"
#include "EventSystem/EventManager.h"
#include "Objects/GameObject.h"
#include "Objects/ResourceManager.h"
#include "Scene/Scene.h"

namespace fw
{

ParticleEmitterComponent::ParticleEmitterComponent()
{
    m_pEmitter = new fw::ParticleEmitter( nullptr );
}

ParticleEmitterComponent::ParticleEmitterComponent(Material* pMaterial)
{
    m_pEmitter = new fw::ParticleEmitter( pMaterial );
}

ParticleEmitterComponent::~ParticleEmitterComponent()
{
    delete m_pEmitter;
    m_pGameObject->GetScene()->GetGameCore()->GetEventManager()->UnregisterEventListener(this, fw::InputEvent::GetStaticEventType());
}

void ParticleEmitterComponent::Save(WriterType& writer)
{
    Component::Save(writer);

    ResourceManager* pResources = m_pGameObject->GetScene()->GetGameCore()->GetResourceManager();

    m_pEmitter->Save(writer, pResources);
}

void ParticleEmitterComponent::Load(rapidjson::Value& component)
{
    ResourceManager* pResources = m_pGameObject->GetScene()->GetGameCore()->GetResourceManager();

    m_pEmitter->Load(component, pResources);
}

void ParticleEmitterComponent::AddToInspector()
{
    m_pEmitter->AddToInspector(m_pGameObject->GetScene()->GetGameCore()->GetResourceManager());
}

void ParticleEmitterComponent::Init()
{
    if (m_pGameObject->GetScene() != nullptr)
    {
        m_pGameObject->GetScene()->GetGameCore()->GetEventManager()->RegisterEventListener(this, InputEvent::GetStaticEventType());
    }
}

void ParticleEmitterComponent::OnEvent(fw::Event* pEvent)
{
    if (pEvent->GetType() == InputEvent::GetStaticEventType())
    {
        InputEvent* pInputEvent = static_cast<InputEvent*>(pEvent);

        if (pInputEvent->GetDeviceType() == InputEvent::DeviceType::Keyboard)
        {
            if (pInputEvent->GetDeviceState() == InputEvent::DeviceState::Pressed)
            {
                if (pInputEvent->GetKeyCode() == 'E')
                {
                    if (m_pGameObject->GetScene()->GetSelectedGameObject() == m_pGameObject)
                    {
                        m_pEmitter->ToggleOnState();
                    }
                }
            }
        }
    }
}

void ParticleEmitterComponent::SetMaterial(fw::Material* pMaterial)
{
    m_pEmitter->SetMaterial( pMaterial );
}

} // namespace fw
