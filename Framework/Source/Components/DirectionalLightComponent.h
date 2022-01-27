#pragma once
#include "Component.h"
#include "LightComponent.h"

namespace fw {

class DirectionalLightComponent : public LightComponent
{
public:

    DirectionalLightComponent();

    static const char* GetStaticType() { return "DirectionalLightComponent"; }
    virtual const char* GetType() override { return GetStaticType(); }
    static Component* Create() { return new DirectionalLightComponent(); }

    virtual void Save(WriterType& writer) override;
    virtual void Load(rapidjson::Value& component) override;
    virtual void AddToInspector() override;

};

} // namespace fw
