#pragma once

#if FW_USING_LUA

namespace fw
{
class LuaPlayer;
class GameObject;
class Event;
class PlayerController;

class LuaGameState
{
public:
    LuaGameState();
    ~LuaGameState();

    void InitGameObjectFromLua(GameObject* go, const char* funcName);
    void UpdateGameObjectFromLua(GameObject* go, float delta, const char* funcName);

    void OnInputEvent(Event* pEvent, GameObject* go, int deviceType, int deviceState, int key, bool canJump, const char* funcName);

    lua_State* m_pLuaState = nullptr;

private:
    
    void RegisterClasses();
};

}

#endif