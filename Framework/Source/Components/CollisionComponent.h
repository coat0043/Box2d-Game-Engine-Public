#pragma once

#include "Component.h"
#include "Physics/PhysicsBody.h"

namespace fw {

class GameObject;
class PhysicsBody;
class vec3;

class CollisionComponent : public Component
{
public:
    CollisionComponent();
    CollisionComponent(CollisionComponent* component);
    CollisionComponent(vec3 offset, ShapeType shapeType, BodyType bodyType, float density = 0.0f);
    virtual ~CollisionComponent() override;

    static const char* GetStaticType() { return "CollisionComponent"; }
    virtual const char* GetType() override { return GetStaticType(); }
    static Component* Create() { return new CollisionComponent(); }

    virtual void Init() override;
    virtual void Update(float deltaTime) override;

    virtual void Save(WriterType& writer) override;
    virtual void Load(rapidjson::Value& component) override;
    virtual void AddToInspector() override;

    void CreatePhysicsBody();

    PhysicsBody* GetPhysicsBody() { return m_pPhysicsBody; }
    void SetPhysicsBody(PhysicsBody* pPhysicsBody) { m_pPhysicsBody = pPhysicsBody; }

    ShapeType GetShapeType() { return m_ShapeType; }
    void SetShapeType(ShapeType shapeType) { m_ShapeType = shapeType; }

    BodyType GetBodyType() { return m_BodyType; }
    void SetBodyType(BodyType bodyType) { m_BodyType = bodyType; }

    vec3 GetOffset() { return m_Offset; }
    void SetOffset(vec3 offset) { m_Offset = offset; }

    float GetDensity() { return m_Density; }
    void SetDensity(float density) { m_Density = density; }

    void SyncPhysicsBodyToTransform();
    void SyncTransformToPhysicsBody();

protected:
    PhysicsBody* m_pPhysicsBody = nullptr;
    ShapeType m_ShapeType = ShapeType::Box;
    BodyType m_BodyType = BodyType::StaticBody;
    vec3 m_Offset = vec3(0,0,0);
    float m_Density = 1.0f;
    bool m_IsSensor = false;
    int m_Category = 1;
    int m_Mask = 0xFFFF; // 1111 1111 1111 1111
};

} // namespace fw
