#include "FrameworkPCH.h"

#include "ResourcesPanel.h"
#include "Components/CameraComponent.h"
#include "Components/Component.h"
#include "Components/MeshComponent.h"
#include "Components/AABBComponent.h"
#include "Core/GameCore.h"
#include "Objects/GameObject.h"
#include "Objects/Material.h"
#include "Objects/MaterialEditor.h"
#include "Objects/Mesh.h"
#include "Objects/ResourceManager.h"
#include "Objects/Sound.h"
#include "Objects/Texture.h"
#include "Objects/TextureManager.h"
#include "Scene/Scene.h"
#include "Utility/ShaderProgram.h"
#include "Utility/FrameBufferObject.h"

namespace fw {

ResourcesPanel::ResourcesPanel(ResourceManager* pActiveResourceManager, TextureManager* pTextureManager, const char* cPanelName, Scene* pActiveScene)
{
    //Defualts
    m_IsOn = true;
    m_FilterText = "...";
    m_pResourceManager = pActiveResourceManager;
    m_pTextureManager = pTextureManager;
    m_cPanelName = cPanelName;
    m_pScene = pActiveScene;
    m_AssetName = "YOU DID NOT CALL A DROP FUNCTION????";
    m_pMaterial = nullptr;

    m_pMeshBase = m_pResourceManager->GetMesh("Cube");
    m_pMaterialBase = m_pResourceManager->GetMaterial("Green");
    m_pShaderProgameBase = m_pResourceManager->GetShader("Basic");
    m_pTextureBase = m_pResourceManager->GetTexture("Green");

    //m_pCamera = new CameraComponent(nullptr, vec2(5, 5), vec2(2, 2), vec2(5, 5));

    Width = 64;
    Height = 64;
    Scale = 1.0f;
    m_pFBO = new FrameBufferObject((unsigned)Width, (unsigned)Width);
    LOG(INFO, "%s Initialized...", m_cPanelName);
}

ResourcesPanel::~ResourcesPanel()
{
    delete m_pCamera;
    delete m_pFBO;

    for (int i = 0; i < m_Resources.size(); i++)
    {
        delete m_Resources[i];
    }
}

void ResourcesPanel::Update(Scene* pActiveScene)
{
    m_pScene = pActiveScene;

    ImGui::SetNextWindowPos(ImVec2(765, 497), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(515, 225), ImGuiCond_FirstUseEver);
    ImGui::Begin(m_cPanelName, &m_IsOn); //, ImGuiWindowFlags_MenuBar);
    if (!m_IsOn)
    {
        ImGui::End();
        return;
    }

    //UpdateMenu();
    UpdateTabs();
    ImGui::End();
}

void ResourcesPanel::UpdateAssetList(ResourceType eResourceTypes)
{
    int Index = GetIndexByEnumtype(eResourceTypes);
    ClearAssetList(Index);

    //******************ADD LIST TO "ALL Name" in Asset Panel HERE****************************************************
    if (eResourceTypes == Materials)
        m_Resources[Index]->PopulateAssetList<Material>(m_pResourceManager->m_pMaterials);
    if (eResourceTypes == Textures)
        m_Resources[Index]->PopulateAssetList<Texture>(m_pResourceManager->m_pTextures);
    if (eResourceTypes == Shaders)
        m_Resources[Index]->PopulateAssetList<ShaderProgram>(m_pResourceManager->m_pShaders);
    if (eResourceTypes == Meshes)
        m_Resources[Index]->PopulateAssetList<Mesh>(m_pResourceManager->m_pMeshes);
    if (eResourceTypes == Sounds)
        m_Resources[Index]->PopulateAssetList<Sound>(m_pResourceManager->m_pSound);
}

std::string* ResourcesPanel::DropNode(const char* name, const char* toolTip, const char* nameId)
{
    ImGui::BeginChild(name, ImVec2(125, 30), true);
    if (ImGui::TreeNodeEx(name, ImGuiTreeNodeFlags_Leaf, "%s (%s)", name, nameId))
    {
        if (ImGui::BeginDragDropTarget())
        {
            ImGui::SetTooltip(toolTip);
            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(nameId))
            {
                std::string* pAssetData = static_cast<std::string*>(payload->Data);

                ImGui::EndDragDropTarget();
                ImGui::TreePop();
                ImGui::EndChild();

                return pAssetData;
            }
        }
        ImGui::TreePop();
    }
    ImGui::EndChild();
    return nullptr;
}

std::string* ResourcesPanel::DropNodeWithImage(Texture* pTexture, const char* name, const char* toolTip, const char* nameId)
{
    std::string* valueDropped = nullptr;

    if( pTexture != nullptr )
    {
        ImGui::BeginChild("#Button#", ImVec2(200, 55), true);
        ImTextureID tex_id = (void*)(intptr_t)pTexture->GetHandle();
        vec2 tex_size = pTexture->GetSize();

        ImGuiIO& io = ImGui::GetIO();

        ImVec2 size = ImVec2(32.0f, 32.0f);
        ImVec2 uv0 = ImVec2(0.0f, 0.0f);
        ImVec2 uv1 = ImVec2(1, -1);
        ImVec4 bg_col = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
        ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);

        //Show Image and Toggle TextureManager
        if (ImGui::ImageButton(tex_id, size, uv0, uv1, -1, bg_col, tint_col))
        {
            //m_pTextureManager->ToggleOnOff();
        }
        if (ImGui::IsItemHovered())
        {
            ImGui::BeginTooltip();
            float region_sz = 32.0f;

            float width = pTexture->GetSize().x;
            float height = pTexture->GetSize().y;

            // Displaying the image and its dimensions.
            ImGui::Text("Dimensions: %.0f * %.0f \n", width, height);
            ImGui::Image(tex_id, ImVec2(256, 256), uv0, uv1);
            ImGui::EndTooltip();
        }
        ImGui::SameLine(50, 2);

        valueDropped = DropNode(name, toolTip, nameId);

        ImGui::EndChild();
    }
    else
    {
        valueDropped = DropNode(name, toolTip, nameId);
    }

    return valueDropped;
}

std::string* ResourcesPanel::DropNodeWithImage(Material* pMaterial, const char* name, const char* toolTip, const char* nameId)
{
    //BindFBO();
    //glClearColor(0, 0, 0.2f, 1);
    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //glViewport(0, 0, m_pFBO->GetRequestedWidth(), m_pFBO->GetRequestedHeight());
    ////DrawFBO(AssetName, eRsourceType);
    //m_pScene->Draw();
    //UnBindFBO();

    //ImGui::BeginChild("ButtonImage", ImVec2(200, 55), true);
    //ImTextureID tex_id = (void*)(intptr_t)m_pFBO->GetColorTextureHandle();

    //ImGuiIO& io = ImGui::GetIO();

    //ImVec2 size = ImVec2(32, 32);
    //ImVec2 uvTopLeft(0, m_pFBO->GetHeightRatio());
    //ImVec2 uvBottomRight(m_pFBO->GetWidthRatio(), 0);
    //ImVec4 bg_col = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
    //ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);

    ////Show Image and Toggle TextureManager
    //if (ImGui::ImageButton(tex_id, size, uvTopLeft, uvBottomRight, -1, bg_col, tint_col))
    //{
    //    m_pMaterialManager->ToggelOnOff()
    //}

    if (ImGui::IsItemHovered())
    {
        ImGui::BeginTooltip();
        float region_sz = 32.0f;

        float width = pMaterial->GetTexture()->GetSize().x;
        float height = pMaterial->GetTexture()->GetSize().y;

        // Displaying the image and its dimensions.
        ImGui::Text("Dimensions: %.0f * %.0f \n", width, height);
        //ImGui::Image(tex_id, ImVec2(256, 256), uvBottomRight, uvBottomRight);
        ImGui::EndTooltip();
    }

    //ImGui::SameLine(50, 2);
    std::string* valueDropped = DropNode(name, toolTip, nameId);

    //ImGui::EndChild();
    return valueDropped;
}

void ResourcesPanel::DropNodeMesh(Mesh** ppMesh, GameObject* pGameObject)
{
    ResourceManager* pResources = pGameObject->GetScene()->GetGameCore()->GetResourceManager();

    std::string* meshNameDropped = nullptr;

    if (*ppMesh)
    {
        std::string meshName = pResources->FindMeshName(*ppMesh);
        meshNameDropped = ResourcesPanel::DropNode(meshName.c_str(), "", "Meshes");
    }
    else
    {
        meshNameDropped = ResourcesPanel::DropNode("No mesh", "", "Meshes");
    }

    if (meshNameDropped)
    {
        *ppMesh = pResources->GetMesh(*meshNameDropped);
    }
}

void ResourcesPanel::DropNodeMaterial(Material** ppMaterial, GameObject* pGameObject)
{
    ResourceManager* pResources = pGameObject->GetScene()->GetGameCore()->GetResourceManager();

    std::string* matNameDropped = nullptr;

    if (*ppMaterial)
    {
        std::string materialName = pResources->FindMaterialName(*ppMaterial);
        matNameDropped = ResourcesPanel::DropNodeWithImage((*ppMaterial)->GetTexture(), materialName.c_str(), "", "Materials");

        if (ImGui::BeginPopupContextItem("Material Context Menu"))
        {
            if (ImGui::MenuItem("Edit"))
            {
                pGameObject->GetScene()->GetGameCore()->GetMaterialEditor()->SetActiveMaterial(*ppMaterial);
                pGameObject->GetScene()->GetGameCore()->GetMaterialEditor()->MakeVisible();
            }
            ImGui::EndPopup();
        }
    }
    else
    {
        matNameDropped = ResourcesPanel::DropNode("No material", "", "Materials");
    }

    if (matNameDropped)
    {
        *ppMaterial = pResources->GetMaterial(*matNameDropped);
    }
}

void ResourcesPanel::DropNodeShaderProgram(ShaderProgram** ppShaderProgram, Material* pMaterial, Scene* pScene)
{
    ResourceManager* pResources = pScene->GetGameCore()->GetResourceManager();

    std::string* shaderNameDropped = nullptr;

    if (*ppShaderProgram)
    {
        std::string shaderName = pResources->FindShaderName(*ppShaderProgram);
        shaderNameDropped = ResourcesPanel::DropNode(shaderName.c_str(), "", "Shaders");
    }
    else
    {
        shaderNameDropped = ResourcesPanel::DropNode("No shader", "", "Shaders");
    }

    if (shaderNameDropped)
    {
        pMaterial->SetShader(pResources->GetShader(*shaderNameDropped));
    }
}

void ResourcesPanel::DropNodeTexture(Texture** ppTexture, Material* pMaterial, Scene* pScene)
{
    ResourceManager* pResources = pScene->GetGameCore()->GetResourceManager();

    std::string* textureNameDropped = nullptr;

    if (*ppTexture)
    {
        std::string textureName = pResources->FindTextureName(*ppTexture);
        textureNameDropped = ResourcesPanel::DropNode(textureName.c_str(), "", "Textures");
    }
    else
    {
        textureNameDropped = ResourcesPanel::DropNode("No texture", "", "Textures");
    }

    if (textureNameDropped)
    {
        pMaterial->SetTexture(pResources->GetTexture(*textureNameDropped));
    }
}

void ResourcesPanel::DropNodeSound(Sound** ppSound, GameObject* pGameObject)
{
    ResourceManager* pResources = pGameObject->GetScene()->GetGameCore()->GetResourceManager();

    std::string* matNameDropped = nullptr;

    if (*ppSound)
    {
        std::string materialName = pResources->FindSoundName(*ppSound);
        matNameDropped = ResourcesPanel::DropNode(materialName.c_str(), "Sound", "Sounds");
    }
    else
    {
        matNameDropped = ResourcesPanel::DropNode("No Sound", "", "Sounds");
    }

    if (matNameDropped)
    {
        *ppSound = pResources->GetSound(*matNameDropped);
    }
}

void ResourcesPanel::AddResource(const char* name, ResourceType eResourceTypes, bool open, const ImVec4& color)
{
    m_Resources.push_back(new MyResource(name, eResourceTypes, open, color));

    UpdateAssetList(eResourceTypes);

    LogAddedResource(name, open);
}

int ResourcesPanel::GetIndexByEnumtype(ResourceType eResourceTypes)
{
    int index = 0;
    for (int i = 0; i < (int)m_Resources.size(); i++)
    {
        if (m_Resources[i]->aResourceType == eResourceTypes)
        {
            //LOG(INFO, "aResourceType Num : %d", index);
            break;
        }
        index++;
    }

    return index;
}

void ResourcesPanel::MyResource::DisplayContents(MyResource* pMyResource, ResourcesPanel* pResourcesPanel)
{
    ImGui::PushID(this);
    //ImGui::Separator();
    //ImGui::Text("Assets Panel\n");
    //ImGui::PushStyleColor(ImGuiCol_Text, Color);
    //ImGui::TextWrapped("Drag and Drop Color");
    //ImGui::SameLine(0, 2);
    //ImGui::ColorEdit3(" ", &Color.x);  // Useful to test drag and drop and hold-dragged-to-open-tab behavior.
    //ImGui::PopStyleColor();

    std::string AllText = "All ";
    for (int i = 0; i < NameSize; i++)
    {
        AllText.push_back(Name[i]);
    }

    if (ImGui::TreeNodeEx("%s", ImGuiTreeNodeFlags_DefaultOpen, AllText.c_str()))
    {
        for (int i = 0; i < AssetNames.size(); i++)
        {
            ImGui::BeginChild(AssetNames[i].c_str(), ImVec2(200, 30), true);
            ImGui::PushID(AssetNames[i].c_str());
            if (ImGui::TreeNodeEx("%s", ImGuiTreeNodeFlags_Leaf, AssetNames[i].c_str()))
            {
                if (ImGui::BeginDragDropSource(ImGuiDragDropFlags_SourceNoHoldToOpenOthers))
                {
                    ImGui::SetDragDropPayload(Name, &AssetNames.at(i), sizeof(AssetNames.at(i)));

                    //Display preview
                    ImGui::PushStyleColor(ImGuiCol_Text, Color);
                    ImGui::Text("%s", AssetNames[i].c_str());
                    ImGui::PopStyleColor();
                    ImGui::PushStyleColor(ImGuiCol_Button, Color);
                    pResourcesPanel->Draw(AssetNames[i], pMyResource->aResourceType);
                    ImGui::PopStyleColor();
                    ImGui::EndDragDropSource();
                }

                ImGui::TreePop();
            }

            if (strcmp(Name, "Materials") == 0)
            {
                if (ImGui::BeginPopupContextItem("Material Context Menu"))
                {
                    if (ImGui::MenuItem("Edit"))
                    {
                        Material* pMat = pResourcesPanel->m_pResourceManager->GetMaterial(AssetNames[i]);
                        pResourcesPanel->m_pScene->GetGameCore()->GetMaterialEditor()->SetActiveMaterial(pMat);
                        pResourcesPanel->m_pScene->GetGameCore()->GetMaterialEditor()->MakeVisible();
                    }
                    ImGui::EndPopup();
                }
            }
            ImGui::PopID();
            ImGui::EndChild();
        }
        ImGui::TreePop();
    }
    ImGui::PopID();
}

void ResourcesPanel::UpdateMenu()
{
    //Menu
    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("Options"))
        {
            int openCount = 0;
            for (int i = 0; i < m_Resources.size(); i++)
                openCount += m_Resources[i]->Open ? 1 : 0;

            if (ImGui::BeginMenu("Open", openCount < m_Resources.size()))
            {
                for (int i = 0; i < m_Resources.size(); i++)
                {
                    if (!m_Resources[i]->Open)
                        if (ImGui::MenuItem(m_Resources[i]->Name))
                            m_Resources[i]->DoOpen();
                }
                ImGui::EndMenu();
            }

            if (ImGui::MenuItem("Close Tabs", NULL, false, openCount > 0))
            {
                for (int i = 0; i < m_Resources.size(); i++)
                {
                    m_Resources[i]->DoForceClose();
                }
            }

            if (ImGui::MenuItem("Exit", "Alt+F4"))
            {
                m_IsOn = false;
            }
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }
}

void ResourcesPanel::UpdateTabs()
{
    //Tabs
    if (ImGui::BeginTabBar("TABS", ImGuiTabBarFlags_Reorderable))
    {
        // Submit Tabs
        for (int i = 0; i < (int)m_Resources.size(); i++)
        {
            if (!m_Resources[i]->Open)
                continue;
            ImGui::PushStyleColor(ImGuiCol_TabActive, m_Resources[i]->Color);
            bool visible = ImGui::BeginTabItem(m_Resources[i]->Name, &m_Resources[i]->Open);

            if (visible)
            {
                //Display Text and list of stings to drag and drop;
                //bool Update = ImGui::Button("Load Assets");
                //ImGui::SameLine();
                //bool Clear = ImGui::Button("Clear Assets");

                //if (Update)
                //{
                //    UpdateAssetList(m_Resources[i]->aResourceType);
                //}
                //if (Clear)
                //{
                //    ClearAssetList(GetIndexByEnumtype(m_Resources[i]->aResourceType));
                //}
                //ImGui::SameLine();

                //std::string clipText = (ImGui::GetClipboardText() != nullptr) ? ImGui::GetClipboardText() : "";
                //size_t clipSize = clipText.length();
                //const size_t size = 32;
                //char newText[size];
                //strncpy_s(newText, size, m_FilterText.c_str(), sizeof(newText));
                //ImGui::SetNextItemWidth(250);
                //if (ImGui::InputTextWithHint("Filter", "possible Future Feature? Jimmy?", newText, size, ImGuiInputTextFlags_AutoSelectAll))
                //{
                //    m_FilterText = newText;
                //}

                m_Resources[i]->DisplayContents(m_Resources[i], this);

                ImGui::EndTabItem();
            }
            ImGui::PopStyleColor();
        }
        ImGui::EndTabBar();
    }
}

void ResourcesPanel::LogIndexOfResource(int index)
{
    //Debuging
    //LOG(INFO, "%s index : %d", m_Resources[index].Name, index);
}

void ResourcesPanel::LogAddedResource(const char* name, bool m_IsOn)
{
    //Debuging
    //std::string s_IsOn = ""; 
    //if (IsOn == true)
    //    s_IsOn = "True";
    //else
    //    s_IsOn = "False";
    //
    //LOG(INFO, "Added %s to MyResource : IsOpen = %s", name, s_IsOn.c_str());
}

//Internal in Update
void ResourcesPanel::DrawFBO(std::string& AssetName, ResourceType eRsourceType)
{
    if (eRsourceType == Materials)
    {
        DrawFBOM(m_pResourceManager->GetMaterial(AssetName));
    }
    else if (eRsourceType == Textures)
    {
        //DrawFBOT
    }
    else if (eRsourceType == Shaders)
    {
        DrawFBOS(m_pResourceManager->GetShader(AssetName));
    }
    else if (eRsourceType == Meshes)
    {
        //DrawFBOMh
    }
}

//Used for DropNodes;
void ResourcesPanel::DrawFBOM(Material* pMaterial)
{
    MyMatrix mat;
    mat.CreateSRT(vec3(5, 5, 0), vec2(2, 2), vec2(0, 1));

    if (m_pCamera)
    {
        m_pMeshBase->Draw(m_pCamera, &mat, pMaterial);
    }
}

void ResourcesPanel::DrawFBOS(ShaderProgram* pShaderProgram)
{
    //TODO
}

void ResourcesPanel::BindFBO()
{
    m_pFBO->Bind();
}

void ResourcesPanel::UnBindFBO()
{
    m_pFBO->Unbind();
}

void ResourcesPanel::Draw(std::string& AssetName, ResourceType eRsourceType)
{
    // Draw our resources into a texture;
    //BindFBO();
    //glClearColor(0, 0, 0.2f, 1);
    //glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //glViewport(0, 0, m_pFBO->GetRequestedWidth(), m_pFBO->GetRequestedHeight());
    //DrawFBO(AssetName, eRsourceType);
    //m_pScene->Draw();
    //UnBindFBO();

    //ImTextureID tex_id = (void*)(intptr_t)m_pFBO->GetColorTextureHandle();

    //ImVec2 size = ImVec2(m_pFBO->GetRequestedWidth() * Scale, m_pFBO->GetRequestedHeight() * Scale);
    //ImVec2 uvTopLeft(0, m_pFBO->GetHeightRatio());
    //ImVec2 uvBottomRight(m_pFBO->GetWidthRatio(), 0);

    //if (ImGui::ImageButton(tex_id, size, uvTopLeft, uvBottomRight))
    //{
    //}
}

void ResourcesPanel::ClearAllAssetLists()
{
    for (int i = 0; i < (int)m_Resources.size(); i++)
    {
        m_Resources[i]->AssetNames.clear();
    }
}

void ResourcesPanel::ClearAssetList(int index)
{
    m_Resources[index]->AssetNames.clear();
}

} // namespace fw
