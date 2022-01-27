#include "FrameworkPCH.h"
#include "TextureManager.h"
#include "Objects/Texture.h"
#include "Objects/GameObject.h"
#include "Scene/Scene.h"

namespace fw
{

TextureManager::TextureManager(const char* Texture_Path, std::map<std::string, Texture*>* pTextures, Scene* pActiveScene)
    : m_pTextures(pTextures), m_pScene(pActiveScene), m_TexturePath(Texture_Path)
{
    LoadFromDisk(Texture_Path);
}

TextureManager::~TextureManager()
{

}

void TextureManager::LoadFromDisk(const char* Texture_Path)
{
    // Scans Directory for all texture files
    for (const auto& entry : std::filesystem::directory_iterator(Texture_Path))
    {
        // Getting all images from directory
        if (entry.path().extension().string() == ".png" ||
            entry.path().extension().string() == ".jpg" ||
            entry.path().extension().string() == ".jpeg")
        {
            std::filesystem::path filepath = entry.path();
            filepath.make_preferred();
            std::string s_filepath = filepath.string();

            // Avoiding duplicates
            if (!(std::find(m_pFilenames.begin(), m_pFilenames.end(), filepath.string()) != m_pFilenames.end()))
            {
                // Pushing all file paths into a vector			
                m_pFilenames.push_back(s_filepath);

                std::string filename = filepath.stem().string();

                // Pushing all filenames into a vector
                m_pTexNames.push_back(filename);

                std::string s_filepath = filepath.string();
                const char* c_filepath = s_filepath.c_str();

                // Creating a new Texture
                m_pTextures->operator[](filename) = new fw::Texture(c_filepath);
            }
        }
    }

    if (m_pTextures->empty())
    {
        LOG(ERROR, "No textures found on disk");
    }

    else
    {
        LOG(INFO, "All textures loaded from disk");
    }
}

void TextureManager::Update(Scene* pActiveScene)
{
    m_pScene = pActiveScene;
    m_pObjects = m_pScene->GetAllGameObjects();

    static GameObject* active_GO = m_pObjects->front();

    ImGui::Begin("Texture Manager",&IsOn);

    if (ImGui::Button("Refresh"))
    {
        LoadFromDisk(m_TexturePath);
    }

    ImGui::BeginGroup();

    // Listing all GameObjects.
    {
        std::vector<std::string> ObjectNames;
        for (auto& GO : *m_pObjects)
        {
            ImGui::BeginChild("left", ImVec2(130, 280), true);
            std::string name = GO->GetName();
            for (auto& existingName : ObjectNames)
            {
                if (name.compare(existingName) == 0)
                {
                    name += "(copy)";
                }
            }
            ObjectNames.push_back(name);
            bool active = active_GO == GO;

            // Setting active gameobject.

            if (ImGui::RadioButton(name.c_str(), active))
            {
                active_GO = GO;
            }
            if (name.size() > 6)
            {
                std::string subString = name.substr(name.length() - 6, name.length());
                if (subString.compare("(copy)") == 0)
                {
                    if (ImGui::IsItemHovered())
                    {
                        ImGui::BeginTooltip();
                        ImGui::Text("This object has the same name as one in this scene\nConsider renaming one of them");
                        ImGui::EndTooltip();
                    }
                }
            }
            ImGui::EndChild();
        }
    }
    ImGui::EndGroup();


    std::map<std::string, Texture*>::iterator it;

    ImGui::SameLine();

    ImGui::BeginGroup();

    // Listing all Textures.
    {
        for (it = m_pTextures->begin(); it != m_pTextures->end(); it++)
        {
            ImGui::BeginChild("right", ImVec2(200, 280), true);
            Texture* tex = it->second;
            ImTextureID tex_id = (void*)(intptr_t)tex->GetHandle();
            vec2 tex_size = tex->GetSize();

            ImGuiIO& io = ImGui::GetIO();

            ImVec2 size = ImVec2(32.0f, 32.0f);
            ImVec2 uv0 = ImVec2(0.0f, 0.0f);
            ImVec2 uv1 = ImVec2(1, -1);
            ImVec4 bg_col = ImVec4(0.0f, 0.0f, 0.0f, 1.0f);
            ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);

            //// Assigning texture to gameobject.
            //if (ImGui::ImageButton(tex_id, size, uv0, uv1, -1, bg_col, tint_col))
            //{
            //    active_GO->GetMaterial()->SetTexture(tex);
            //    LOG(INFO, "Texture Assigned");
            //}

            if (ImGui::IsItemHovered())
            {
                ImGui::BeginTooltip();
                float region_sz = 32.0f;

                float width = tex->GetSize().x;
                float height = tex->GetSize().y;

                // Displaying the image and its dimensions.
                ImGui::Text("Dimensions: %.0f * %.0f \n", width, height);
                ImGui::Image(tex_id, ImVec2(256, 256), uv0, uv1);
                ImGui::EndTooltip();
            }
            ImGui::SameLine();
            ImGui::Text(it->first.c_str());

            ImGui::EndChild();
        }

    }
    ImGui::EndGroup();
    ImGui::End();
}

}
