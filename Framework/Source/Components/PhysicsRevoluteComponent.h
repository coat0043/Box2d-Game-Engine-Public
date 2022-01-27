#pragma once

#include "Component.h"

namespace fw {

class PhysicsConstraint;

class PhysicsRevoluteComponent : public Component
{
public:
    PhysicsRevoluteComponent();
    ~PhysicsRevoluteComponent();

    static const char* GetStaticType() { return "PhysicsRevoluteComponent"; }
    virtual const char* GetType() override { return GetStaticType(); }
    static Component* Create() { return new PhysicsRevoluteComponent(); }

    virtual void Save(WriterType& writer) override;
    virtual void Load(rapidjson::Value& component) override;
    virtual void AddToInspector() override;

    virtual void Init() override;

    // Getters.

    // Setters.

protected:
    PhysicsConstraint* m_pConstraint = nullptr;
};

} // namespace fw
