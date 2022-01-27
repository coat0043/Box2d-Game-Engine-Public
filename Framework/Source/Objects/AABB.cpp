#include "FrameworkPCH.h"

#include "AABB.h"
#include "GameObject.h"

namespace fw {

AABB::AABB()
{
    m_Center.Set( 0, 0, 0 );
    m_HalfSize.Set( 0, 0, 0 );
}

AABB::AABB(vec3 center, vec3 halfSize)
{
    m_Center = center;
    m_HalfSize = halfSize;
}

AABB::~AABB()
{
}

bool AABB::CheckCollision(const AABB& otherAABB) const
{
    //check the X axis
    if (fabs(m_Center.x - otherAABB.m_Center.x) < m_HalfSize.x + otherAABB.m_HalfSize.x)
    {
        //check the Y axis
        if (fabs(m_Center.y - otherAABB.m_Center.y) < m_HalfSize.y + otherAABB.m_HalfSize.y)
        {
            //check the Z axis
            if (fabs(m_Center.z - otherAABB.m_Center.z) < m_HalfSize.z + otherAABB.m_HalfSize.z)
            {
                return true;
            }
        }
    }
    return false;
}

vec3 AABB::GetCenter()
{
    return m_Center;
}

vec3 AABB::GetHalfSize()
{
    return m_HalfSize;
}

void AABB::Set(vec3 center, vec3 halfSize)
{
    m_Center = center;
    m_HalfSize = halfSize;
}

} // namespace fw
