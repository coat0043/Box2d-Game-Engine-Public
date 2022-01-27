#pragma once

namespace fw
{

class AABBComponent;
class CameraComponent;
class Component;
class FrameBufferObject;
class GameObject;
class Material;
class Mesh;
class MeshComponent;
class ResourceManager;
class TextureManager;
class MaterialEditor;
class Scene;
class ShaderProgram;
class Texture;
class Sound;

enum ResourceType
{
    //*************ADD ResourceType HERE************************************************************************************
    Materials,
    Textures,
    Shaders,
    Meshes,
    Sounds
};

class ResourcesPanel
{
    struct MyResource
    {
        const char* Name;        // Document title
        int NameSize;
        bool        Open;       // Set when open (we keep an array of all available MyResource)
        ImVec4      Color;      // An arbitrary variable associated to the MyResource
        ResourceType aResourceType;
        std::vector<std::string> AssetNames;     //String of Assets

        MyResource(const char* nameID, ResourceType eResourceTypes, bool open = true, const ImVec4& color = ImVec4(1.0f, 1.0f, 1.0f, 1.0f))
        {
            Name = nameID;
            Open = open;
            Color = color;
            aResourceType = eResourceTypes;

            const char* str = Name;
            NameSize = 0;
            while (str[NameSize] != '\0') NameSize++;
        }

        void DoOpen() { Open = true; }
        void DoForceClose() { Open = false; }
        void DisplayContents(MyResource* pMyResource, ResourcesPanel* pResourcesPanel);

        template <class T>
        void PopulateAnyAssetList(std::vector<std::string>& OutList, std::map<std::string, T*> InMap)
        {
            std::map<std::string, T*>::iterator it = InMap.begin();

            while (it != InMap.end())
            {
                OutList.push_back(it->first);
                it++;
            }
        }

        template <class T>
        void PopulateAssetList(std::map<std::string, T*> InMap)
        {
            std::map<std::string, T*>::iterator it = InMap.begin();

            while (it != InMap.end())
            {
                AssetNames.push_back(it->first);
                it++;
            }
        }
    };

public:
    ResourcesPanel(ResourceManager* pActiveResourceManager, TextureManager* pTextureManager, const char* cPanelName, Scene* pActiveScene);
    ~ResourcesPanel();

    static std::string* DropNode(const char* name, const char* toolTip, const char* nameId);
    static std::string* DropNodeWithImage(Texture* pTexture, const char* name, const char* toolTip, const char* nameId);
    static std::string* DropNodeWithImage(Material* pMaterial, const char* name, const char* toolTip, const char* nameId); //future
    static void DropNodeMesh(Mesh** ppMesh, GameObject* pGameObject);
    static void DropNodeMaterial(Material** ppMaterial, GameObject* pGameObject);
    static void DropNodeShaderProgram(ShaderProgram** ppShaderProgram, Material* pMaterial, Scene* pScene);
    static void DropNodeTexture(Texture** ppTexture, Material* pMaterial, Scene* pScene);
    static void DropNodeSound(Sound** ppSound, GameObject* pGameObject);

    //Visual
    void ToggleOnOff() { m_IsOn = !m_IsOn; }
    bool GetOnOffState() { return m_IsOn; }
    const char* GetPanelName() { return m_cPanelName; }

    //MAIN FUNCTION
    void Update(Scene* pActiveScene);

    //pop_back on the vector array of MyResouce struct, AKA initializes and auto adds to Tabs of the Panel
    void AddResource(const char* name, ResourceType eResourceTypes, bool open = true, const ImVec4& color = ImVec4(1.0f, 1.0f, 1.0f, 1.0f));

    //CALL THIS WHEN EVER YOU CHANGE THE ASSET LIST;
    void UpdateAssetList(ResourceType eResourceTypes);
    void ClearAssetList(int index);
    void ClearAllAssetLists();

    //Get or set resource 
    int GetIndexByEnumtype(ResourceType eResourceTypes);
    ResourceManager* GetResourceManager() { return m_pResourceManager; }
    TextureManager* GetTextureManager() { return m_pTextureManager; }
    std::string GetAssetName() { return m_AssetName; }

private:
    //Defualts
    bool m_IsOn = false;
    ResourceManager* m_pResourceManager = nullptr;
    TextureManager* m_pTextureManager = nullptr;
    const char* m_cPanelName;
    Scene* m_pScene = nullptr;
    std::string m_FilterText;

    //FBO
    FrameBufferObject* m_pFBO = nullptr;
    float Width;
    float Height;
    float Scale;
    Mesh* m_pMeshBase = nullptr;
    Material* m_pMaterial = nullptr;
    Material* m_pMaterialBase = nullptr;
    Texture* m_pTextureBase = nullptr;
    ShaderProgram* m_pShaderProgameBase = nullptr;
    CameraComponent* m_pCamera = nullptr;
    //UI Features
    std::vector<MyResource*> m_Resources;

    //Interal - DO NOT CALL
    std::string m_AssetName;
    void UpdateMenu();
    void UpdateTabs();
    void LogIndexOfResource(int index);
    void LogAddedResource(const char* name, bool m_IsOn);
    void Draw(std::string& AssetName, ResourceType eRsourceType); // Future
    void DrawFBO(std::string& AssetName, ResourceType eRsourceType); //Future
    void DrawFBOM(Material* pMaterial); //Future
    void DrawFBOS(ShaderProgram* pShaderProgram); //Future
    void BindFBO();
    void UnBindFBO();
};

} // fw