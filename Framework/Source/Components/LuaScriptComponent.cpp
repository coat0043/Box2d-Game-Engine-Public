#include "FrameworkPCH.h"

#if FW_USING_LUA
#include "LuaScriptComponent.h"
#include "EventSystem/Event.h"
#include "Scene/Scene.h"
#include "Objects/GameObject.h"
#include "Lua/LuaGameState.h"
#include "Core/GameCore.h"
#include "EventSystem/EventManager.h"

namespace fw
{

LuaScriptComponent::LuaScriptComponent()
{
}

LuaScriptComponent::LuaScriptComponent(EventManager* pEventManager)
{
    pEventManager->RegisterEventListener(this, fw::InputEvent::GetStaticEventType());
    pEventManager->RegisterEventListener(this, fw::PhysicsCollisionEvent::GetStaticEventType());
}

LuaScriptComponent::LuaScriptComponent(EventManager* pEventManager, LuaScript* pLuaScript)
{
    //m_pLuaScript = pLuaScript;
    pEventManager->RegisterEventListener(this, fw::InputEvent::GetStaticEventType());
    pEventManager->RegisterEventListener(this, fw::PhysicsCollisionEvent::GetStaticEventType());
}

LuaScriptComponent::~LuaScriptComponent()
{
    m_pGameObject->GetScene()->GetGameCore()->GetEventManager()->UnregisterEventListener(this, InputEvent::GetStaticEventType());
    m_pGameObject->GetScene()->GetGameCore()->GetEventManager()->UnregisterEventListenerCompletely(this);
}

void LuaScriptComponent::OnEvent(Event* pEvent)
{
    
    if (pEvent->GetType() == InputEvent::GetStaticEventType())
    {
        InputEvent* pInputEvent = static_cast<InputEvent*>(pEvent);
        
        deviceState = static_cast<unsigned int>(pInputEvent->GetDeviceState());
        deviceType = static_cast<unsigned int>(pInputEvent->GetDeviceType());
        keyCode = pInputEvent->GetKeyCode();

        m_pGameObject->GetScene()->GetLuaState()->OnInputEvent(pEvent, m_pGameObject, deviceType, deviceState, keyCode, bCanJump, "OnInputEvent");
    }
    if (pEvent->GetType() == PhysicsCollisionEvent::GetStaticEventType())
    {
        bCanJump = true;
    }
}

}

#endif