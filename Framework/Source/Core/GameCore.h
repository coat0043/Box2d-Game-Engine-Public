#pragma once

#include "EventSystem/EventListener.h"
#include "Math/Vector.h"

namespace fw {

class AudioEngine;
class ComponentRegistry;
class Event;
class EventManager;
class FWCore;
class MaterialEditor;
class ResourceManager;

class GameCore : public EventListener
{
public:
    enum class GameplayState
    {
        Stopped,
        Playing,
    };

public:
    GameCore(FWCore* pFramework);
    virtual ~GameCore();

    virtual void StartFrame(float deltaTime) = 0;
    virtual void OnEvent(Event* pEvent) = 0;
    virtual void Update(float deltaTime) = 0;
    virtual void Draw() = 0;
    
    virtual void GetGameRenderWindowInfo(vec2* windowPos, vec2* windowSize) = 0;

    FWCore* GetFramework() { return m_pFramework; }
    EventManager* GetEventManager() { return m_pEventManager; }
    ResourceManager* GetResourceManager() { return m_pResourceManager; }
    AudioEngine* GetAudioEngine() { return m_pAudioEngine; }
    ComponentRegistry* GetComponentRegistry() { return m_pComponentRegistry; }
    MaterialEditor* GetMaterialEditor() { return m_pMaterialEditor; }
    bool GetDrawPhysicsDebugShapes() { return m_DrawDebugPhysicsShapes; }

    GameplayState GetGameplayState() { return m_GameplayState; }

protected:
    FWCore* m_pFramework = nullptr;

    EventManager* m_pEventManager = nullptr;
    ResourceManager* m_pResourceManager = nullptr;
    AudioEngine* m_pAudioEngine = nullptr;
    ComponentRegistry* m_pComponentRegistry = nullptr;
    MaterialEditor* m_pMaterialEditor = nullptr;
    bool m_DrawDebugPhysicsShapes = true;

    GameplayState m_GameplayState = GameplayState::Stopped;
};

} // namespace fw
