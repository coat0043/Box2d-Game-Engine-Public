#pragma once

namespace fw {

class AABB;
class CameraComponent;
class Component;
class GameObject;
class Mesh;
class Scene;
class Sound;

class ComponentRegistry
{
public:
    typedef Component* (*CreateComponentFn)(void);

public:
    ComponentRegistry();
    ComponentRegistry(const Component &) {};
    ~ComponentRegistry() { m_ComponentTypes.clear(); }

    ComponentRegistry& operator=(const Component&) { return *this; }

    void AddInterfaceToCreateComponents(Scene* pScene, GameObject* pObject);

    void Register(const std::string& Name, CreateComponentFn pfnCreate);
    Component* CreateComponent(const std::string& componentName);
    
protected:
    std::map<const std::string, CreateComponentFn> m_ComponentTypes;
};

} // namespace fw
