#include "FrameworkPCH.h"

#include "MaterialEditor.h"
#include "Objects/GameObject.h"
#include "Objects/Material.h"
#include "Objects/ResourceManager.h"
#include "Scene/Scene.h"
#include "UI/ResourcesPanel.h"

namespace fw
{

MaterialEditor::MaterialEditor(ResourceManager* pResourceManager, Scene* pActiveScene)
    : m_pResourceManager(pResourceManager)
    , m_pScene(pActiveScene)
{
}

MaterialEditor::~MaterialEditor()
{

}

void MaterialEditor::Update(Scene* pActiveScene)
{
    m_pScene = pActiveScene;

    std::string active_MatName = m_pResourceManager->FindMaterialName(m_pActiveMaterial);
    Material* active_Mat = m_pActiveMaterial;

    ImGui::Begin("Material Editor", &m_IsOn);

    if( active_Mat == nullptr )
    {
        ImGui::Text("No material selected.");
    }
    else
    {
        // Listing all Material Properties.
        ImGui::Text("Name: %s", active_MatName.c_str());

        ShaderProgram* pShaderProgram = active_Mat->GetShader();
        Texture* pTexture = active_Mat->GetTexture();
        Color color = active_Mat->GetColor();
        vec2 UVScale = active_Mat->GetUVScale();
        vec2 UVOffset = active_Mat->GetUVOffset();

        if (ImGui::ColorEdit4("Color", &color.r))
        {
            active_Mat->SetColor(color);
        }
        if (ImGui::DragFloat2("UVScale", &UVScale.x, 0.05f))
        {
            active_Mat->SetUVScale(UVScale);
        }
        if (ImGui::DragFloat2("UVOffset", &UVOffset.x, 0.05f))
        {
            active_Mat->SetUVOffset(UVOffset);
        }

        ImGui::PushID(pShaderProgram);
        ResourcesPanel::DropNodeShaderProgram(&pShaderProgram, active_Mat, m_pScene);
        ImGui::PopID();

        ImGui::PushID(pTexture);
        ResourcesPanel::DropNodeTexture(&pTexture, active_Mat, m_pScene);
        ImGui::PopID();
    }
    ImGui::End();
}

}
