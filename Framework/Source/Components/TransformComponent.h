#pragma once

#include "Component.h"
#include "../Math/MyMatrix.h"

namespace fw {

class TransformComponent : public Component
{
public:
    TransformComponent();
    TransformComponent(vec3 pos, vec3 rot, vec3 scale);

    static const char* GetStaticType() { return "TransformComponent"; }
    virtual const char* GetType() override { return GetStaticType(); }
    static Component* Create() { return new TransformComponent(); }

    virtual void Save(WriterType& writer) override;
    virtual void Load(rapidjson::Value& component) override;
    virtual void FinalizeLoad(rapidjson::Value& component) override;
    virtual void AddToInspector() override;

    // Getters.

    vec3 GetForward() { return GetWorldTransform()->GetAt(); }

    vec3 GetPosition() { return m_Position; }
    vec3 GetRotation() { return m_Rotation; }
    vec3 GetScale() { return m_Scale; }
    GameObject* GetParent() { return m_pParent; }
    std::vector<GameObject*>& GetChildren() { return m_Children; }
    GameObject* GetChild(int index) { return m_Children[index]; }

    MyMatrix* GetWorldTransform();

    // Setters.
    void SetParent(GameObject* aParent);
    void SetChild(GameObject* aChild);
    void RemoveChild(GameObject* aChild);

    void SetPosition(vec3 pos) { m_Position = pos; }
    void SetPosition(float x, float y, float z) { m_Position.Set(x, y, z); }
    void SetRotation(vec3 rot) { m_Rotation = rot; }
    void SetRotation(float x, float y, float z) { m_Rotation.Set(x, y, z); }
    void SetScale(vec3 rot) { m_Scale = rot; }
    void SetScale(float x, float y, float z) { m_Scale.Set(x, y, z); }

protected:
    GameObject* m_pParent = nullptr;
    std::vector<GameObject*> m_Children;

    MyMatrix m_WorldMatrix;

    vec3 m_Position;
    vec3 m_Rotation;
    vec3 m_Scale;
};

} // namespace fw
