#pragma once
#include "Component.h"
#include "LightComponent.h"

namespace fw {

    class PointLightComponent : public LightComponent
    {
    public:

        PointLightComponent();

        static const char* GetStaticType() { return "PointLightComponent"; }
        virtual const char* GetType() override { return GetStaticType(); }
        static Component* Create() { return new PointLightComponent(); }
     
        virtual void Save(WriterType& writer) override;
        virtual void Load(rapidjson::Value& component) override;
        virtual void AddToInspector() override;

    };

} // namespace fw
