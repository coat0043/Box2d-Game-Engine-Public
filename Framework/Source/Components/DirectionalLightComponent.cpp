#include "FrameworkPCH.h"
#include "DirectionalLightComponent.h"
namespace fw {

DirectionalLightComponent::DirectionalLightComponent() : LightComponent()
{
    m_LightType = Type::Directional;
}

void DirectionalLightComponent::Save(WriterType& writer)
{
    LightComponent::Save(writer);
}

void DirectionalLightComponent::Load(rapidjson::Value& component)
{
    LightComponent::Load(component);
}

void DirectionalLightComponent::AddToInspector()
{
    ImGui::SliderFloat("Intensity", &m_Intensity, 0.0f, 3.0f, "%.2f", 1.0f);
    ImGui::SliderFloat("Ambient", &m_Ambient, 0.0f, 0.5f, "%.2f", 1.0f);
    ImGui::SliderFloat("Specularity", &m_SpecularIntensity, 0.005f, 1.5f, "%3f", 1.0f);
    ImGui::SliderFloat("SpecAttn.", &m_Attenuation, 0.0f, 0.06f, "%9f", 1.0f);
    ImGui::ColorEdit3("Color", &m_Color.r, 0);
}
}