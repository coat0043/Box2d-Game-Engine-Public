#pragma once

#include "Component.h"
#include "MeshComponent.h"
#include "Utility/Color.h"

namespace fw {

class Texture;
class TransformComponent;

// The base class for any light component. The default type is treated as a point light.
class LightComponent : public Component
{
public:

    enum class Type
    {
        Pointlight = 0,
        Spotlight = 1,
        Directional = 2
    };

    LightComponent();
    virtual ~LightComponent();

    static const char* GetStaticType() { return "LightComponent"; }
    virtual const char* GetType() override { return GetStaticType(); }

    virtual void Save(WriterType& writer) override;
    virtual void Load(rapidjson::Value& component) override;
    virtual void AddToInspector() override;

    // Getters
    float GetIntensity() { return m_Intensity; }
    float GetRange() { return m_Range; }
    float GetAttenuation() { return m_Attenuation; }
    float GetAmbient() { return m_Ambient; }
    float GetSpecularIntensity() { return m_SpecularIntensity; }
    Texture* GetTextureCookie() { return m_texture_Cookie; }

    vec3 GetColor() { return vec3(m_Color.r, m_Color.g, m_Color.b); }
    TransformComponent* GetTransform();

    Type GetLightType() { return m_LightType; };

    // Setters
    void SetIntensity(float intensity) { m_Intensity = intensity; }
    void SetRange(float range) { m_Range = range; }
    void SetAttenuation(float attenuation) { m_Attenuation = attenuation; }
    void SetAmbient(float ambient) { m_Ambient = ambient; }
    void SetSpecularIntensity(float specular) { m_SpecularIntensity = specular; }
    void SetColor(Color color) { m_Color = color; }
    void SetTextureCookie(Texture* atextureCookie) {m_texture_Cookie = atextureCookie; }

protected:

    Type m_LightType = Type::Pointlight;

    Texture* m_texture_Cookie = nullptr;

    float m_Range = 5;
    float m_Intensity = 1;
    float m_Attenuation = 1;
    float m_Ambient = 0.01f;
    float m_SpecularIntensity = 1;

    Color m_Color = Color::White();
};

} // namespace fw
