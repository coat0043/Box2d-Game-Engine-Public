#pragma once

#if FW_USING_LUA
#include "Component.h"

namespace fw {

class LuaScript;
class Event;
class EventManager;

class LuaScriptComponent : public Component
{
public:
    LuaScriptComponent();
    LuaScriptComponent(EventManager* pEventManager);
    LuaScriptComponent(EventManager* pEventManager, LuaScript* pLuaScript);

    virtual ~LuaScriptComponent();

    static const char* GetStaticType() { return "LuaScriptComponent"; }
    virtual const char* GetType() override { return GetStaticType(); }
    static Component* Create() { return new LuaScriptComponent(); }

    virtual void OnEvent(Event* pEvent);

    // Getters.
    //LuaScript* GetLuaScript() { return m_pLuaScript; }

    // Setters.
    //void SetLuaScript(LuaScript* pLuaScript) { m_pLuaScript = pLuaScript; }

protected:
    //LuaScript* m_pLuaScript = nullptr;

    bool bCanJump = true;

    unsigned int deviceState = 0;
    unsigned int deviceType = 0;
    unsigned int keyCode = 0;
};

} // namespace fw
#endif
