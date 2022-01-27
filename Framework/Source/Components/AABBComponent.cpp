#include "FrameworkPCH.h"
#include "AABBComponent.h"

void fw::AABBComponent::Save(WriterType& writer)
{
    Component::Save(writer);

    JSONSaveVec3(writer, "Center", m_AABB.GetCenter());
    JSONSaveVec3(writer, "HalfSize", m_AABB.GetHalfSize());
}

void fw::AABBComponent::Load(rapidjson::Value& component)
{
    // I can't find any places where we are using this yet so not sure how loading format should work
}
