#pragma once

#include "Math/Vector.h"

namespace fw {

class AABB
{
public:
    AABB();
    AABB(vec3 center, vec3 halfSize);
    virtual ~AABB();

    bool CheckCollision(const AABB& otherAABB) const;

    // Getters.
    vec3 GetCenter();
    vec3 GetHalfSize();

    // Setters.
    void Set(vec3 center, vec3 halfSize);

protected:
    vec3 m_Center;
    vec3 m_HalfSize;
};

} // namespace fw
