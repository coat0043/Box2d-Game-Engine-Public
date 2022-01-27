#pragma once

#include "EventSystem/EventListener.h"
#include "Utility/JSONHelpers.h"
#include "Math/Vector.h"

namespace fw {

class GameObject;

class Component : public EventListener
{
public:
    Component() {}
    virtual ~Component() {}

    virtual const char* GetType() = 0;

    virtual void Save(WriterType& writer);
    virtual void Load(rapidjson::Value& component) {}
    virtual void FinalizeLoad(rapidjson::Value& component) {}
    virtual void AddToInspector() { ImGui::Text("No properties to edit."); }

    virtual void Init() {};
    virtual void Update(float deltaTime) {};

    // Getters.
    GameObject* GetGameObject() { return m_pGameObject; }

    // Setters.
    void SetGameObject(GameObject* pGameObject) { m_pGameObject = pGameObject; }

protected:
    GameObject* m_pGameObject = nullptr;
};

} // namespace fw
