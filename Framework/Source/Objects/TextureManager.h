#pragma once

namespace fw
{

class Scene;
class Texture;
class GameObject;

class TextureManager
{
public:
    TextureManager(const char* Texture_Path, std::map<std::string, Texture*>* pTextures, Scene* pActiveScene);
    ~TextureManager();

    void ToggleOnOff() { IsOn = !IsOn; }
    bool GetOnOffState() { return IsOn; }
    void LoadFromDisk(const char* Texture_Path);
    void Update(Scene* pActiveScene);

private:
    const char* m_TexturePath;
    std::vector<std::string> m_pFilenames;
    std::vector<std::string> m_pTexNames;
    bool IsOn = false;

    std::map<std::string, fw::Texture*>* m_pTextures;
    std::vector<GameObject*>* m_pObjects;
    
    Scene* m_pScene;

};

} // fw
