#pragma once

#include "Component.h"

namespace fw {

class Material;
class ParticleEmitter;

class ParticleEmitterComponent : public Component
{
public:
    ParticleEmitterComponent();
    ParticleEmitterComponent(Material* pMaterial);
    virtual ~ParticleEmitterComponent();

    static const char* GetStaticType() { return "ParticleEmitterComponent"; }
    virtual const char* GetType() override { return GetStaticType(); }
    static Component* Create() { return new ParticleEmitterComponent(); }

    virtual void Save(WriterType& writer) override;
    virtual void Load(rapidjson::Value& component) override;
    virtual void AddToInspector() override;
    virtual void Init() override;

    virtual void OnEvent(fw::Event* pEvent) override;

    // Getters.
    ParticleEmitter* GetEmitter() { return m_pEmitter; }

    // Setters.
    void SetMaterial(Material* pMaterial);

protected:
    ParticleEmitter* m_pEmitter = nullptr;
};

} // namespace fw
