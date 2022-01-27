#include "FrameworkPCH.h"
#include "PointLightComponent.h"
namespace fw {

    PointLightComponent::PointLightComponent() : LightComponent()
    {
        m_LightType = Type::Pointlight;
    }

    void PointLightComponent::Save(WriterType& writer)
    {
        LightComponent::Save(writer);
    }

    void PointLightComponent::Load(rapidjson::Value& component)
    {
        LightComponent::Load(component);
    }

    void PointLightComponent::AddToInspector()
    {
        LightComponent::AddToInspector();
    }
}