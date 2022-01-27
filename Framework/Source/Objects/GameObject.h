#pragma once

#include "Math/Vector.h"
#include "Utility/Color.h"
#include "EventSystem/EventListener.h"
#include "Physics/PhysicsWorld.h"
#include "Material.h"
#include "Utility/JSONHelpers.h"

namespace fw {

class CameraComponent;
class Component;
class GameCore;
class Material;
class Mesh;
class Scene;
class ShaderProgram;
class PhysicsBody;
class ResourcesPanel;
class Texture;
class TransformComponent;

class GameObject : public EventListener
{
public:
    GameObject(GameObject& orgnlGO);
    GameObject(Scene* pScene, std::string name);
    virtual ~GameObject();

    virtual void Update(float deltaTime);
    virtual void Draw(CameraComponent* pCamera);

    virtual void Save(WriterType& writer);
    virtual void Load(rapidjson::Value& jsonObject);
    virtual void FinalizeLoad(rapidjson::Value& jsonObject);

    virtual void AddToInspector();

    void AddComponent(Component* pComponent);
    void RemoveAndDeleteComponent(Component* pComponent);
    void RegisterAllComponents();
    void UnregisterAllComponents();

    // Getters.
    std::string GetName() { return m_Name; }
    Scene* GetScene() { return m_pScene; }

    Component* GetFirstComponentOfType(const char* type);

    TransformComponent* GetTransform();

#if FW_USING_LUA
    static void LuaRegister(lua_State* luastate);
#endif

    // Setters.
    void SetName(std::string name) { m_Name = name; }

protected:
    Scene* m_pScene = nullptr;

    std::vector<Component*> m_Components;
    bool m_AreComponentsRegistered = true;
    std::string m_Name;
};

} // namespace fw
