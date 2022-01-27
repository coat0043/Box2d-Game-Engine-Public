#include "FrameworkPCH.h"

#include "LightComponent.h"
#include "Core/GameCore.h"
#include "Objects/GameObject.h"
#include "TransformComponent.h"
#include "Scene/Scene.h"

namespace fw {

LightComponent::LightComponent()
{
}

LightComponent::~LightComponent()
{
}

void LightComponent::Save(WriterType& writer)
{
    Component::Save(writer);

    ResourceManager* pResources = m_pGameObject->GetScene()->GetGameCore()->GetResourceManager();

    JSONSaveVec3(writer, "Color", vec3(m_Color.r, m_Color.g, m_Color.b));

    writer.Key("LightType");
    writer.Int((int)m_LightType);
    writer.Key("Intensity");
    writer.Double(m_Intensity);
    writer.Key("Range");
    writer.Double(m_Range);
    writer.Key("Attenuation");
    writer.Double(m_Attenuation);
    writer.Key("Ambient");
    writer.Double(m_Ambient);
    writer.Key("Specularity");
    writer.Double(m_SpecularIntensity);
}

void LightComponent::Load(rapidjson::Value& component)
{
    ResourceManager* pResources = m_pGameObject->GetScene()->GetGameCore()->GetResourceManager();

    if (component.HasMember("LightType"))
    {
        m_LightType = (Type)(float)component["LightType"].GetInt();
    }

    if (component.HasMember("Intensity"))
    {
        m_Intensity = (float)component["Intensity"].GetDouble();
    }

    if (component.HasMember("Range"))
    {
        m_Range = (float)component["Range"].GetDouble();
    }

    if (component.HasMember("Attenuation"))
    {
        m_Attenuation = (float)component["Attenuation"].GetDouble();
    }

    if (component.HasMember("Ambient"))
    {
        m_Ambient = (float)component["Ambient"].GetDouble();
    }

    if (component.HasMember("Specularity"))
    {
        m_SpecularIntensity = (float)component["Specularity"].GetDouble();
    }

    if (component.HasMember("Color"))
    {
        vec3 color;
        JSONLoadVec3(component, "Color", &color);
        m_Color.r = color.x;
        m_Color.g = color.y;
        m_Color.b = color.z;
    }
}

void LightComponent::AddToInspector()
{
    ImGui::SliderFloat("Range", &m_Range, 0.0f, 50.0f, "%.2f", 1.0f);
    ImGui::SliderFloat("Intensity", &m_Intensity, 0.0f, 3.0f, "%.2f", 1.0f);
    ImGui::SliderFloat("Attenuation", &m_Attenuation, 0.0f, 10.0f, "%.2f", 1.0f);
    ImGui::SliderFloat("Ambient", &m_Ambient, 0.0f, 0.5f, "%.2f", 1.0f);
    ImGui::SliderFloat("Specularity", &m_SpecularIntensity, 0.0f, 2.0f, "%.2f", 1.0f);
    ImGui::ColorEdit3("Color", &m_Color.r, 0);
}

TransformComponent* LightComponent::GetTransform()
{
    Component* transform = m_pGameObject->GetFirstComponentOfType("TransformComponent");
    
    if (transform != nullptr)
    {
        return static_cast<TransformComponent*>(transform);
    }
    else
    {
        LOG(fw::LogType::Warning, "LightComponent object has no transform attached. Generating a TransformComponent at world 0");
        TransformComponent* NewTransformComp = new TransformComponent(0,0,0);
        m_pGameObject->AddComponent(NewTransformComp);
        return NewTransformComp;
    }
}


} // namespace fw
