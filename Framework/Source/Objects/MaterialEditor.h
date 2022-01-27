#pragma once

namespace fw
{

class GameObject;
class Material;
class ResourceManager;
class Scene;

class MaterialEditor
{
public:
    MaterialEditor(ResourceManager* pResourceManager, Scene* pActiveScene);
    ~MaterialEditor();

    void ToggleOnOff() { m_IsOn = !m_IsOn; }
    void MakeVisible() { m_IsOn = true; }
    bool GetOnOffState() { return m_IsOn; }
    void Update(Scene* pActiveScene);

    Material* GetActiveMaterial() { return m_pActiveMaterial; }
    void SetActiveMaterial(Material* pMaterial) { m_pActiveMaterial = pMaterial; }

private:
    bool m_IsOn = true;

    Material* m_pActiveMaterial = nullptr;
    ResourceManager* m_pResourceManager;
    std::vector<GameObject*>* m_pObjects;

    Scene* m_pScene;
};

} // fw
