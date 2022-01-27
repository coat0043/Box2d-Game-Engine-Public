#include "FrameworkPCH.h"
#include "SpotLightComponent.h"
namespace fw {

    SpotLightComponent::SpotLightComponent() : LightComponent()
    {
        m_LightType = Type::Spotlight;
    }

    void SpotLightComponent::Save(WriterType& writer)
    {
        LightComponent::Save(writer);
 
        writer.Key("ConeAngle");
        writer.Double(m_SpotAngle);
        writer.Key("Falloff");
        writer.Double(m_SpotFalloff);
    }

    void SpotLightComponent::Load(rapidjson::Value& component)
    {
        LightComponent::Load(component);

        if (component.HasMember("ConeAngle"))
        {
            m_SpotAngle = (float)component["ConeAngle"].GetDouble();
        }

        if (component.HasMember("Falloff"))
        {
            m_SpotFalloff = (float)component["Falloff"].GetDouble();
        }
    }

    void SpotLightComponent::AddToInspector()
    {
        ImGui::SliderFloat("ConeAngle", &m_SpotAngle, 0.0f, 90.0f, "%.2f", 1.0f);
        ImGui::SliderFloat("Falloff", &m_SpotFalloff, 0.0f, 50.0f, "%.2f", 1.0f);
        ImGui::Separator();

        LightComponent::AddToInspector();
    }
}