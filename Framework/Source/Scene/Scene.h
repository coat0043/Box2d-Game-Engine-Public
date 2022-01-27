#pragma once

#include "../EventSystem/EventListener.h"

class PlayerController;

namespace fw {

class ComponentManager;
class CameraComponent;
class Event;
class GameCore;
class GameObject;
class Light;
class LuaGameState;
class Material;
class PhysicsWorld;
class ResourceManager;

class Scene : public EventListener
{
public:
    Scene(GameCore* pGameCore);
    virtual ~Scene();

    virtual void Init();
    virtual void StartFrame(float deltaTime) = 0;
    virtual void OnEvent(Event* pEvent);
    virtual void Update(float deltaTime);
    virtual void Draw();
    virtual void DrawMousePickingMeshes();
    virtual void DrawObjectList();
    bool DrawObject(GameObject* pObject);
    virtual void DrawNewObjectButtons();
    virtual void DrawImguiDemoWindow();
    virtual void DrawImGuiInspector();

    // Save/Load.
    void ClearScene();
    std::string GetPath() { return m_Path; }
    std::string GetName() { return m_Name; }
    virtual void SetPath(std::string path) { m_Path = path; }
    virtual void SetName(std::string name) { m_Name = name; }
    void LoadFromFile(std::string path, std::string sceneName);
    bool LoadFromFile(std::string filename);
    void SaveToFile();
    void SaveToFile(std::string filename);

    // Getters.
    GameCore* GetGameCore() { return m_pGameCore; }
    ComponentManager* GetComponentManager() { return m_pComponentManager; }

    bool GetIsObjectListOn() { return m_IsObjectListOn; }
    std::vector<GameObject*>* GetAllGameObjects() { return &m_Objects; }
    GameObject* GetGameObjectByName(std::string name);
    GameObject* GetSelectedGameObject() { return m_ImGuiSelectedObject; }

    CameraComponent* GetActiveCameraComponent() { return m_pActiveCameraComponent; }

    PhysicsWorld* GetWorld() { return m_pWorld; }
    bool Is2DPhysicsOnXZPlane() { return m_Is2DPhysicsOnXZPlane; }

    LuaGameState* GetLuaState() { return m_pLuaGameState; }

    // Setters.
    virtual void SelectObject(int selectedObject);

    void SetActiveCameraComponent(CameraComponent* pActiveCamera) { m_pActiveCameraComponent = pActiveCamera; }

    void ToggleObjectList() { m_IsObjectListOn = !m_IsObjectListOn; }
    void ToggleObjectDetails() { m_IsObjectInspectorOn = !m_IsObjectInspectorOn; }
    void ToggleImguiDemo() { m_IsImguiDemoWindowOn = !m_IsImguiDemoWindowOn; }

    void CheckAABBCollision();

protected:
    std::string m_Path = "";
    std::string m_Name = "Scene";

    GameCore* m_pGameCore = nullptr;
    std::vector<GameObject*> m_Objects;

    ComponentManager* m_pComponentManager = nullptr;
    CameraComponent* m_pActiveCameraComponent = nullptr;

    GameObject* m_ImGuiSelectedObject = nullptr;
    PlayerController* m_pPlayerController = nullptr;

    // Physics.
    PhysicsWorld* m_pWorld = nullptr;
    bool m_Is2DPhysicsOnXZPlane = false;

    // Lua.
    LuaGameState* m_pLuaGameState = nullptr;

    // For mouse picking.
    Material* m_pMousePickerMaterial = nullptr;

    // bootleg code to fix windows reappearing when switching scenes.
    inline static bool m_IsObjectListOn = true;
    inline static bool m_IsObjectSelectorOn = true;
    inline static bool m_IsObjectInspectorOn = true;
    inline static bool m_IsImguiDemoWindowOn = false;
};

} // namespace fw
