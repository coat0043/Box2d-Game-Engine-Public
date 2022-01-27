#pragma once
#include "Component.h"
#include "LightComponent.h"

namespace fw {

    class SpotLightComponent : public LightComponent
    {
    public:
        SpotLightComponent();

        static const char* GetStaticType() { return "SpotLightComponent"; }
        virtual const char* GetType() override { return GetStaticType(); }
        static Component* Create() { return new SpotLightComponent(); }
     
        virtual void Save(WriterType& writer) override;
        virtual void Load(rapidjson::Value& component) override;
        virtual void AddToInspector() override;

        float GetSpotAngle() { return m_SpotAngle; };
        float GetSpotFalloff() { return m_SpotFalloff; };
        float SetSpotAngle(float spotAngle) { m_SpotAngle = spotAngle; };
        float SetSpotFalloff(float spotFalloff) { m_SpotFalloff = spotFalloff; };

        protected:
        float m_SpotAngle = 60;
        float m_SpotFalloff = 1;
    };

} // namespace fw
