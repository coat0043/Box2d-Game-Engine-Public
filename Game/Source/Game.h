#pragma once

class PlayerController;

class Game : public fw::GameCore
{
public:
    Game(fw::FWCore* pFramework);
    virtual ~Game();

    void Init();
    virtual void StartFrame(float deltaTime) override;
    virtual void OnEvent(fw::Event* pEvent) override;
    virtual void Update(float deltaTime) override;
    virtual void Draw() override;

    void GetGameRenderWindowInfo(vec2* windowPos = nullptr, vec2* windowSize = nullptr) override;

    void MousePicker();
    void Editor_InitFileDialog();
    void Editor_SaveDialog();
    void Editor_LoadDialog();
    void Editor_GameWindow();
    void Editor_MainMenu();
    void Editor_SetupDocking();
    void Editor_TransformGizmo();

    fw::ShaderProgram* GetShader(std::string name) { return m_pResourceManager->GetShader(name); }
    fw::Mesh* GetMesh(std::string name) { return m_pResourceManager->GetMesh(name); }
    fw::Texture* GetTexture(std::string name) { return m_pResourceManager->GetTexture(name); }
    fw::Material* GetMaterial(std::string name) { return m_pResourceManager->GetMaterial(name); }
    fw::Sound* GetSound(std::string name) { return m_pResourceManager->GetSound(name);}

    PlayerController* GetPlayerController() { return m_pPlayerController; }

protected:
    fw::ImGuiManager* m_pImGuiManager = nullptr;
    ifd::FileDialog* m_pFileDialog = nullptr;
    bool m_SaveDialogVisible = false;
    bool m_LoadDialogVisible = false;

    PlayerController* m_pPlayerController = nullptr;

    Color m_ClearColor = Color(0, 0, 0.2f, 1);

    fw::FrameBufferObject* m_pFBO = nullptr;
    fw::FrameBufferObject* m_pMousePickerFBO = nullptr;

    std::map<std::string, fw::AABB*> m_pAABB;
    fw::Scene* m_pScene = nullptr;

    // Settings.
    bool m_VSyncEnabled = true;
    bool m_LogOpen = true;
    Log* m_Log;
    bool m_ShowQuitDialog = false;

    fw::TextureManager* m_pTextureManager;
    fw::ResourcesPanel* m_pResourcePanel;
    fw::TransformGizmo3D* m_pTransformGizmo3D = nullptr;

    // Misc.
    vec2 m_GameWindowPosition = vec2(0, 0);
    vec2 m_GameWindowSize = vec2(0, 0);
};
