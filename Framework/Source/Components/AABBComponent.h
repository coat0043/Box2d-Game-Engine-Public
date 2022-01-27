#pragma once

#include "Component.h"
#include "Objects/AABB.h"

namespace fw {

class Mesh;

class AABBComponent : public Component
{
public:
    AABBComponent() {}
    AABBComponent(AABB aabb, Mesh* pMesh) { m_AABB = aabb; m_pMesh = pMesh; }

    static const char* GetStaticType() { return "AABBComponent"; }
    virtual const char* GetType() override { return GetStaticType(); }
    static Component* Create() { return new AABBComponent(); }

    virtual void Save(WriterType& writer) override;
    virtual void Load(rapidjson::Value& component) override;

    // Getters.
    AABB& GetAABB() { return m_AABB; }
    Mesh* GetMesh() { return m_pMesh; }

    // Setters.
    void SetAABB(const AABB& aabb) { m_AABB = aabb; }
    void SetMesh(Mesh* pMesh) { m_pMesh = pMesh; }

protected:
    AABB m_AABB;
    Mesh* m_pMesh = nullptr;
};

} // namespace fw
