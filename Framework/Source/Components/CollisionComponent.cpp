#include "FrameworkPCH.h"

#include "CollisionComponent.h"
#include "Components/TransformComponent.h"
#include "Objects/GameObject.h"
#include "Scene/Scene.h"
#include "Math/Vector.h"

namespace fw {

CollisionComponent::CollisionComponent()
{
}

CollisionComponent::CollisionComponent(CollisionComponent* component)
{
    m_pGameObject = component->GetGameObject();
    m_ShapeType = component->GetShapeType();
    m_BodyType = component->GetBodyType();
    m_Density = component->GetDensity();
    m_IsSensor = component->m_IsSensor;
    m_Category = component->m_Category;
    m_Mask = component->m_Mask;
}

CollisionComponent::CollisionComponent(vec3 offset, ShapeType shapeType, BodyType bodyType, float density)
{
    m_ShapeType = shapeType;
    m_BodyType = bodyType;
    m_Density = density;
}

CollisionComponent::~CollisionComponent()
{
    delete m_pPhysicsBody;
}

void CollisionComponent::Init()
{
    if (m_pGameObject != nullptr)
    {
        CreatePhysicsBody();
        SyncPhysicsBodyToTransform();
    }
}

void CollisionComponent::Update(float deltaTime)
{
    SyncTransformToPhysicsBody();
}

void CollisionComponent::Save(WriterType& writer)
{
    Component::Save(writer);

    JSONSaveInt(writer, "ShapeType", m_ShapeType);
    JSONSaveInt(writer, "BodyType", m_BodyType);
    JSONSaveFloat(writer, "Density", m_Density);
    JSONSaveBool(writer, "IsSensor", m_IsSensor);
    JSONSaveInt(writer, "Category", m_Category);
    JSONSaveInt(writer, "Mask", m_Mask);
}

void CollisionComponent::Load(rapidjson::Value& component)
{
    if (component.HasMember("ShapeType"))
    {
        m_ShapeType = (ShapeType)component["ShapeType"].GetInt();
    }

    if (component.HasMember("BodyType"))
    {
        m_BodyType = (BodyType)component["BodyType"].GetInt();
    }

    JSONLoadFloat(component, "Density", &m_Density);
    JSONLoadBool(component, "IsSensor", &m_IsSensor);
    JSONLoadInt(component, "Category", &m_Category);
    JSONLoadInt(component, "Mask", &m_Mask);
}

void CollisionComponent::AddToInspector()
{
    ImGui::Separator();

    ImGui::Text("Collision Component");

    ImGui::Text("Shape Type: %s", ShapeTypeStrings[m_ShapeType]);
    
    bool isStatic = (m_BodyType == BodyType::StaticBody);
    if (ImGui::Checkbox("Static", &isStatic))
    {
        if( isStatic )
        {
            m_BodyType = BodyType::StaticBody;
            m_pPhysicsBody->SetBodyType(BodyType::StaticBody);
        }
        else
        {
            m_BodyType = BodyType::DynamicBody;
            m_pPhysicsBody->SetBodyType(BodyType::DynamicBody);
        }
    }

    if (ImGui::DragFloat("Density", &m_Density, 0.05f, 0, FLT_MAX))
    {
        m_pPhysicsBody->SetDensity(m_Density);
    };

    if (ImGui::Checkbox("Sensor", &m_IsSensor))
    {
        m_pPhysicsBody->SetIsSensor(m_IsSensor);
    }

    if (ImGui::InputInt("Category", &m_Category))
    {
        m_pPhysicsBody->SetFilterOptions(m_Category, m_Mask);
    }

    if (ImGui::InputInt("Mask", &m_Mask))
    {
        m_pPhysicsBody->SetFilterOptions(m_Category, m_Mask);
    }

    if (ImGui::Button("Recreate body"))
    {
        delete m_pPhysicsBody;
        m_pPhysicsBody = nullptr;
        CreatePhysicsBody();
    };

    ImGui::Separator();
}

void CollisionComponent::CreatePhysicsBody()
{
    if (m_pGameObject != nullptr)
    {
        vec3 pos = m_pGameObject->GetTransform()->GetPosition();
        vec3 rot = m_pGameObject->GetTransform()->GetRotation();
        vec3 scale = m_pGameObject->GetTransform()->GetScale();

        if (m_pGameObject->GetScene()->Is2DPhysicsOnXZPlane())
        {
            pos = vec3(pos.x, pos.z, pos.y);
            rot = vec3(rot.x, -rot.z, rot.y);
        }

        assert(m_pPhysicsBody == nullptr);
        m_pPhysicsBody = m_pGameObject->GetScene()->GetWorld()->CreateBody(pos, m_BodyType, true);
        m_pPhysicsBody->SetRotation( rot );

        switch (m_ShapeType)
        {
        case ShapeType::Circle:
            m_pPhysicsBody->CreateCircle(scale.x);
            break;

        case ShapeType::Box:
            m_pPhysicsBody->CreateBox(scale.x, scale.y);
            break;
        }

        m_pPhysicsBody->SetDensity(m_Density);
        m_pPhysicsBody->SetIsSensor(m_IsSensor);
        m_pPhysicsBody->SetFilterOptions(m_Category, m_Mask);
    }
}

void CollisionComponent::SyncPhysicsBodyToTransform()
{
    if (m_pPhysicsBody && m_pGameObject)
    {
        vec3 pos = m_pGameObject->GetTransform()->GetPosition();
        vec3 rot = m_pGameObject->GetTransform()->GetRotation();

        if (m_pGameObject->GetScene()->Is2DPhysicsOnXZPlane())
        {
            m_pPhysicsBody->SetPosition(vec3(pos.x, pos.z, pos.y));
            m_pPhysicsBody->SetRotation(vec3(rot.x, rot.z, -rot.y));
        }
        else
        {
            m_pPhysicsBody->SetPosition(pos);
            m_pPhysicsBody->SetRotation(rot);
        }
    }
}

void CollisionComponent::SyncTransformToPhysicsBody()
{
    if (m_pPhysicsBody && m_pGameObject && m_pGameObject->GetTransform())
    {
        vec3 physicsPosition = m_pPhysicsBody->GetPosition();
        vec3 physicsRotation = m_pPhysicsBody->GetRotation();

        if (m_pGameObject->GetScene()->Is2DPhysicsOnXZPlane())
        {
            vec3 pos = m_pGameObject->GetTransform()->GetPosition();
            pos.x = physicsPosition.x;
            pos.z = physicsPosition.y;

            m_pGameObject->GetTransform()->SetPosition(pos);

            vec3 rot = m_pGameObject->GetTransform()->GetRotation();
            rot.y = -physicsRotation.z;

            m_pGameObject->GetTransform()->SetRotation(vec3(rot.x, rot.y, rot.z));
        }
        else
        {
            vec3 pos = m_pGameObject->GetTransform()->GetPosition();
            pos.x = physicsPosition.x;
            pos.y = physicsPosition.y;

            m_pGameObject->GetTransform()->SetPosition(pos);

            vec3 rot = m_pGameObject->GetTransform()->GetRotation();
            rot.z = physicsRotation.z;

            m_pGameObject->GetTransform()->SetRotation(rot);
        }
    }
}

} // namespace fw
