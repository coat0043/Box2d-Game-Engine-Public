#pragma once

#include "Math/Vector.h"

namespace fw {

class GameObject;
class PhysicsBody;

//============================================================================
//============================================================================

class Event
{
    friend class EventManager;

public:
    Event();
    virtual ~Event();

    virtual const char* GetType() = 0;

protected:
    void SetDelay(float delay) { m_Delay = delay; }

    float m_Delay = 0.0f;
};

//============================================================================
//============================================================================

class InputEvent : public Event
{
public:
    enum class DeviceType
    {
        Keyboard,
        Mouse,
        Controller,
    };

    enum class DeviceState
    {
        Pressed,
        Released,
        LeftStick,
        Typed,
        ScrollUp,
        ScrollDown,
    };

    enum class ControllerButton
    {
        DPadLeft,
        DPadRight,
        DPadUp,
        DPadDown,
        A,
        B,
        X,
        Y,
        LeftBumper,
        RightBumper,
        Start,
        Select,
        LeftThumbstick,
        RightThumbstick,
    };

    InputEvent(DeviceType deviceType, DeviceState deviceState, unsigned int keyCode)
    {
        m_DeviceType = deviceType;
        m_DeviceState = deviceState;
        m_KeyCode = keyCode;
    }

    InputEvent(DeviceState deviceState, ControllerButton buttonID)
    {
        m_DeviceType = DeviceType::Controller;
        m_DeviceState = deviceState;
        m_ButtonID = buttonID;
    }

    InputEvent(DeviceType deviceType, DeviceState deviceState, vec2 joystickDirection)
    {
        m_DeviceType = deviceType;
        m_DeviceState = deviceState;
        m_JoystickDirection = joystickDirection;
    }

    virtual ~InputEvent() {}

    static const char* GetStaticEventType() { return "InputEvent"; }
    virtual const char* GetType() override { return GetStaticEventType(); }

    DeviceType GetDeviceType() { return m_DeviceType; }
    DeviceState GetDeviceState() { return m_DeviceState; }
    unsigned int GetKeyCode() { return m_KeyCode; }
    ControllerButton GetButtonID() { return m_ButtonID; }
    vec2 GetJoystickDirection() { return m_JoystickDirection; }

protected:
    DeviceType m_DeviceType;
    DeviceState m_DeviceState;
    unsigned int m_KeyCode;
    ControllerButton m_ButtonID;

    vec2 m_JoystickDirection;
};

//============================================================================
//============================================================================

class RemoveFromGameEvent : public fw::Event
{
public:
    RemoveFromGameEvent(GameObject* pObject)
    {
        m_pObject = pObject;
    }
    virtual ~RemoveFromGameEvent() {}

    static const char* GetStaticEventType() { return "RemoveFromGameEvent"; }
    virtual const char* GetType() override { return GetStaticEventType(); }

    GameObject* GetGameObject() { return m_pObject; }

protected:
    GameObject* m_pObject;
};

//============================================================================
//============================================================================

class PhysicsCollisionEvent : public fw::Event
{
public:
    PhysicsCollisionEvent(PhysicsBody* pBodyA, PhysicsBody* pBodyB)
    {
        m_pBodyA = pBodyA;
        m_pBodyB = pBodyB;
    }
    virtual ~PhysicsCollisionEvent() {}

    static const char* GetStaticEventType() { return "PhysicsCollisionEvent"; }
    virtual const char* GetType() override { return GetStaticEventType(); }

    PhysicsBody* GetBodyA() { return m_pBodyA; }
    PhysicsBody* GetBodyB() { return m_pBodyB; }

protected:
    PhysicsBody* m_pBodyA;
    PhysicsBody* m_pBodyB;
};

//============================================================================
//============================================================================

class AABBCollisionEvent : public fw::Event
{
public:
    AABBCollisionEvent(fw::GameObject* pAObject, fw::GameObject* pBObject)
    {
        m_pAObject = pAObject;
        m_pBObject = pBObject;
    }
    virtual ~AABBCollisionEvent() {}

    static const char* GetStaticEventType() { return "AABBCollisionEvent"; }
    virtual const char* GetType() override { return GetStaticEventType(); }

    fw::GameObject* GetGameAObject() { return m_pAObject; }
    fw::GameObject* GetGameBObject() { return m_pBObject; }

protected:
    fw::GameObject* m_pAObject;
    fw::GameObject* m_pBObject;
};

//============================================================================
//============================================================================

class BeginPlayEvent : public fw::Event
{
public:
    BeginPlayEvent(){}
    virtual ~BeginPlayEvent() {}

    static const char* GetStaticEventType() { return "BeginPlayEvent"; }
    virtual const char* GetType() override { return GetStaticEventType(); }
};

class EndPlayEvent : public fw::Event
{
public:
    EndPlayEvent() {}
    virtual ~EndPlayEvent() {}

    static const char* GetStaticEventType() { return "EndPlayEvent"; }
    virtual const char* GetType() override { return GetStaticEventType(); }
};

} // namespace fw

