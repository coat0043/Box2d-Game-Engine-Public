//
// Copyright (c) 2012-2019 Jimmy Lord http://www.flatheadgames.com
//
// This software is provided 'as-is', without any express or implied warranty.  In no event will the authors be held liable for any damages arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose, including commercial applications, and to alter it and redistribute it freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.

#ifndef __VECTOR_H__
#define __VECTOR_H__

#include "MathHelpers.h"

namespace fw
{

class vec2
{
public:
    float x;
    float y;

public:
    vec2() {}
    vec2(float nxy) { x = nxy; y = nxy; }
    vec2(float nx, float ny) { x = nx; y = ny; }
    //virtual ~vec2() {}

    static const vec2 Right() { return vec2(1.0f, 0.0f); }
    static const vec2 Up() { return vec2(0.0f, 1.0f); }

    inline void Set(float nx, float ny) { x = nx; y = ny; }
    inline float LengthSquared() const { return x * x + y * y; }
    inline float Length() const { return sqrtf(x * x + y * y); }
    inline float DistanceFrom(const vec2 o) const { return sqrtf((x - o.x) * (x - o.x) + (y - o.y) * (y - o.y)); }

    inline vec2 GetNormalized() const { float len = Length(); if (fequal(len, 0)) return vec2(x, y); len = 1.0f / len; return vec2(x * len, y * len); }
    inline vec2 Normalize() { float len = Length(); if (!fequal(len, 0)) { x /= len; y /= len; } return *this; }
    inline void Absolute() { if (x < 0) x *= -1; if (y < 0) y *= -1; }
    inline vec2 GetAbsolute() const { return vec2(x < 0 ? -x : x, y < 0 ? -y : y); }
    inline float Dot(const vec2& o) const { return x * o.x + y * o.y; }
    inline vec2 Add(const vec2& o) const { return vec2(this->x + o.x, this->y + o.y); }
    inline vec2 Sub(const vec2& o) const { return vec2(this->x - o.x, this->y - o.y); }
    inline vec2 Scale(const float o) const { return vec2(this->x * o, this->y * o); }

    inline vec2 WithX(float x) const { return vec2(x, this->y); }
    inline vec2 WithY(float y) const { return vec2(this->x, y); }

    inline bool operator ==(const vec2& o) const { return fequal(this->x, o.x) && fequal(this->y, o.y); }
    inline bool operator !=(const vec2& o) const { return !fequal(this->x, o.x) || !fequal(this->y, o.y); }

    inline vec2 operator -() const { return vec2(-this->x, -this->y); }
    inline vec2 operator *(const float o) const { return vec2(this->x * o, this->y * o); }
    inline vec2 operator /(const float o) const { return vec2(this->x / o, this->y / o); }
    inline vec2 operator +(const float o) const { return vec2(this->x + o, this->y + o); }
    inline vec2 operator -(const float o) const { return vec2(this->x - o, this->y - o); }
    inline vec2 operator *(const vec2& o) const { return vec2(this->x * o.x, this->y * o.y); }
    inline vec2 operator /(const vec2& o) const { return vec2(this->x / o.x, this->y / o.y); }
    inline vec2 operator +(const vec2& o) const { return vec2(this->x + o.x, this->y + o.y); }
    inline vec2 operator -(const vec2& o) const { return vec2(this->x - o.x, this->y - o.y); }

    inline vec2 operator *=(const float o) { this->x *= o; this->y *= o; return *this; }
    inline vec2 operator /=(const float o) { this->x /= o; this->y /= o; return *this; }
    inline vec2 operator +=(const float o) { this->x += o; this->y += o; return *this; }
    inline vec2 operator -=(const float o) { this->x -= o; this->y -= o; return *this; }
    inline vec2 operator *=(const vec2& o) { this->x *= o.x; this->y *= o.y; return *this; }
    inline vec2 operator /=(const vec2& o) { this->x /= o.x; this->y /= o.y; return *this; }
    inline vec2 operator +=(const vec2& o) { this->x += o.x; this->y += o.y; return *this; }
    inline vec2 operator -=(const vec2& o) { this->x -= o.x; this->y -= o.y; return *this; }

    float& operator[] (int i) { assert(i >= 0 && i < 2); return *(&x + i); }
};

inline vec2 operator *(float scalar, const vec2& vec) { return vec2(scalar * vec.x, scalar * vec.y); }
inline vec2 operator /(float scalar, const vec2& vec) { return vec2(scalar / vec.x, scalar / vec.y); }
inline vec2 operator +(float scalar, const vec2& vec) { return vec2(scalar + vec.x, scalar + vec.y); }
inline vec2 operator -(float scalar, const vec2& vec) { return vec2(scalar - vec.x, scalar - vec.y); }

class vec3
{
public:
    float x;
    float y;
    float z;

public:
    vec3() {}
    vec3(float nxyz) { x = nxyz; y = nxyz; z = nxyz; }
    vec3(float nx, float ny) { x = nx; y = ny; z = 0; }
    vec3(float nx, float ny, float nz) { x = nx; y = ny; z = nz; }
    vec3(vec2 v2) { x = v2.x; y = v2.y; z = 0; }
    vec3(vec2 v2, float nz) { x = v2.x; y = v2.y; z = nz; }
    //virtual ~vec3() {}

    static const vec3 Right() { return vec3(1.0f, 0.0f, 0.0f); }
    static const vec3 Up() { return vec3(0.0f, 1.0f, 0.0f); }
    static const vec3 In() { return vec3(0.0f, 0.0f, 1.0f); }

    inline vec2 XY() const { return vec2(x, y); }
    inline vec2 XZ() const { return vec2(x, z); }

    inline void Set(float nx, float ny, float nz) { x = nx; y = ny; z = nz; }
    inline float LengthSquared() const { return x * x + y * y + z * z; }
    inline float Length() const { return sqrtf(x * x + y * y + z * z); }
    inline float DistanceFrom(const vec3 o) const { return sqrtf((x - o.x) * (x - o.x) + (y - o.y) * (y - o.y) + (z - o.z) * (z - o.z)); }

    inline vec3 GetNormalized() const { float len = Length(); if (fequal(len, 0)) return vec3(x, y, z); len = 1.0f / len; return vec3(x * len, y * len, z * len); }
    inline vec3 Normalize() { float len = Length(); if (!fequal(len, 0)) { x /= len; y /= len; z /= len; } return *this; }
    inline vec3 Cross(const vec3& o) const { return vec3((y * o.z - z * o.y), (z * o.x - x * o.z), (x * o.y - y * o.x)); }
    inline float Dot(const vec3& o) const { return x * o.x + y * o.y + z * o.z; }
    inline vec3 Add(const vec3& o) const { return vec3(this->x + o.x, this->y + o.y, this->z + o.z); }
    inline vec3 Sub(const vec3& o) const { return vec3(this->x - o.x, this->y - o.y, this->z - o.z); }
    inline vec3 Scale(const float o) const { return vec3(this->x * o, this->y * o, this->z * o); }
    inline vec3 MultiplyComponents(const vec3& o) const { return vec3(this->x * o.x, this->y * o.y, this->z * o.z); }
    inline vec3 DivideComponents(const vec3& o) const { return vec3(this->x / o.x, this->y / o.y, this->z / o.z); }
    //inline vec3 MultiplyComponents(const ivec3& o) const { return vec3(this->x * o.x, this->y * o.y, this->z * o.z); }

    inline vec3 WithX(float x) const { return vec3(x, this->y, this->z); }
    inline vec3 WithY(float y) const { return vec3(this->x, y, this->z); }
    inline vec3 WithZ(float z) const { return vec3(this->x, this->y, z); }

    inline bool operator ==(const vec3& o) const { return fequal(this->x, o.x) && fequal(this->y, o.y) && fequal(this->z, o.z); }
    inline bool operator !=(const vec3& o) const { return !fequal(this->x, o.x) || !fequal(this->y, o.y) || !fequal(this->z, o.z); }

    inline vec3 operator -() const { return vec3(-this->x, -this->y, -this->z); }
    inline vec3 operator *(const float o) const { return vec3(this->x * o, this->y * o, this->z * o); }
    inline vec3 operator /(const float o) const { return vec3(this->x / o, this->y / o, this->z / o); }
    inline vec3 operator +(const float o) const { return vec3(this->x + o, this->y + o, this->z + o); }
    inline vec3 operator -(const float o) const { return vec3(this->x - o, this->y - o, this->z - o); }
    inline vec3 operator *(const vec3& o) const { return vec3(this->x * o.x, this->y * o.y, this->z * o.z); }
    inline vec3 operator /(const vec3& o) const { return vec3(this->x / o.x, this->y / o.y, this->z / o.z); }
    inline vec3 operator +(const vec3& o) const { return vec3(this->x + o.x, this->y + o.y, this->z + o.z); }
    inline vec3 operator -(const vec3& o) const { return vec3(this->x - o.x, this->y - o.y, this->z - o.z); }

    inline vec3 operator *=(const float o) { this->x *= o; this->y *= o; this->z *= o; return *this; }
    inline vec3 operator /=(const float o) { this->x /= o; this->y /= o; this->z /= o; return *this; }
    inline vec3 operator +=(const float o) { this->x += o; this->y += o; this->z += o; return *this; }
    inline vec3 operator -=(const float o) { this->x -= o; this->y -= o; this->z -= o; return *this; }
    inline vec3 operator *=(const vec3& o) { this->x *= o.x; this->y *= o.y; this->z *= o.z; return *this; }
    inline vec3 operator /=(const vec3& o) { this->x /= o.x; this->y /= o.y; this->z /= o.z; return *this; }
    inline vec3 operator +=(const vec3& o) { this->x += o.x; this->y += o.y; this->z += o.z; return *this; }
    inline vec3 operator -=(const vec3& o) { this->x -= o.x; this->y -= o.y; this->z -= o.z; return *this; }

    float& operator[] (int i) { assert(i >= 0 && i < 3); return *(&x + i); }

    //inline void operator =(const vec2& o) { x = o.x; y = o.y; z = 0; } // couldn't make this work, used a constructor instead.
};

inline vec3 operator *(float scalar, const vec3& vec) { return vec3(scalar * vec.x, scalar * vec.y, scalar * vec.z); }
inline vec3 operator /(float scalar, const vec3& vec) { return vec3(scalar / vec.x, scalar / vec.y, scalar / vec.z); }
inline vec3 operator +(float scalar, const vec3& vec) { return vec3(scalar + vec.x, scalar + vec.y, scalar + vec.z); }
inline vec3 operator -(float scalar, const vec3& vec) { return vec3(scalar - vec.x, scalar - vec.y, scalar - vec.z); }

class vec4
{
public:
    float x;
    float y;
    float z;
    float w;

public:
    vec4() {}
    vec4(float nx, float ny, float nz, float nw) { x = nx; y = ny; z = nz; w = nw; }
    vec4(vec2 vec, float nz, float nw) { x = vec.x; y = vec.y; z = nz; w = nw; }
    vec4(vec3 vec, float nw) { x = vec.x; y = vec.y; z = vec.z; w = nw; }
    //virtual ~vec4() {}

    inline vec3 XYZ() { return vec3(x, y, z); }

    inline void Set(float nx, float ny, float nz, float nw) { x = nx; y = ny; z = nz; w = nw; }
    inline float LengthSquared() const { return x * x + y * y + z * z + w * w; }
    inline float Length() const { return sqrtf(x * x + y * y + z * z + w * w); }
    inline float DistanceFrom(const vec4 o) const { return sqrtf((x - o.x) * (x - o.x) + (y - o.y) * (y - o.y) + (z - o.z) * (z - o.z) + (w - o.w) * (w - o.w)); }

    inline vec4 GetNormalized() const { float len = Length(); if (fequal(len, 0)) return vec4(x, y, z, w); len = 1.0f / len; return vec4(x * len, y * len, z * len, w * len); }
    inline vec4 Normalize() { float len = Length(); if (!fequal(len, 0)) { x /= len; y /= len; z /= len; w /= len; } return *this; }
    //vec4 CrossProduct(const vec4& b, const vec4& c)
    // from http://www.gamedev.net/topic/269241-4d-vec-class/
    //  not sure if right and have no use for it.
    //{
    //    //Precompute some 2x2 matrix determinants for speed
    //    float Pxy = b.x*c.y - c.x*b.y;
    //    float Pxz = b.x*c.z - c.x*b.z;
    //    float Pxw = b.x*c.w - c.x*b.w;
    //    float Pyz = b.y*c.z - c.y*b.z;
    //    float Pyw = b.y*c.w - c.y*b.w;
    //    float Pzw = b.z*c.w - c.z*b.w;
    //    return vec4(
    //        y*Pzw - z*Pyw + w*Pyz,    //Note the lack of 'x' in this line
    //        z*Pxw - x*Pzw - w*Pxz,    //y, Etc.
    //        x*Pyw - y*Pxw + w*Pxy,
    //        y*Pxz - x*Pyz - z*Pxy
    //        );
    //}
    inline float Dot(const vec4& o) const { return x * o.x + y * o.y + z * o.z + w * o.w; }
    inline vec4 Add(const vec4& o) const { return vec4(this->x + o.x, this->y + o.y, this->z + o.z, this->w + o.w); }
    inline vec4 Sub(const vec4& o) const { return vec4(this->x - o.x, this->y - o.y, this->z - o.z, this->w - o.w); }
    inline vec4 Scale(const float o) const { return vec4(this->x * o, this->y * o, this->z * o, this->w * o); }
    inline vec4 MultiplyComponents(const vec4& o) const { return vec4(this->x * o.x, this->y * o.y, this->z * o.z, this->w * o.w); }
    inline vec4 DivideComponents(const vec4& o) const { return vec4(this->x / o.x, this->y / o.y, this->z / o.z, this->w / o.w); }
    //inline vec4 MultiplyComponents(const ivec4& o) const { return vec4(this->x * o.x, this->y * o.y, this->z * o.z, this->w * o.w); }

    inline vec4 WithX(float x) const { return vec4(x, this->y, this->z, this->w); }
    inline vec4 WithY(float y) const { return vec4(this->x, y, this->z, this->w); }
    inline vec4 WithZ(float z) const { return vec4(this->x, this->y, z, this->w); }
    inline vec4 WithW(float w) const { return vec4(this->x, this->y, this->z, w); }

    inline bool operator ==(const vec4& o) const { return fequal(this->x, o.x) && fequal(this->y, o.y) && fequal(this->z, o.z) && fequal(this->w, o.w); }
    inline bool operator !=(const vec4& o) const { return !fequal(this->x, o.x) || !fequal(this->y, o.y) || !fequal(this->z, o.z) || !fequal(this->w, o.w); }

    inline vec4 operator -() const { return vec4(-this->x, -this->y, -this->z, -this->w); }
    inline vec4 operator *(const float o) const { return vec4(this->x * o, this->y * o, this->z * o, this->w * o); }
    inline vec4 operator /(const float o) const { return vec4(this->x / o, this->y / o, this->z / o, this->w / o); }
    inline vec4 operator +(const float o) const { return vec4(this->x + o, this->y + o, this->z + o, this->w + o); }
    inline vec4 operator -(const float o) const { return vec4(this->x - o, this->y - o, this->z - o, this->w - o); }
    inline vec4 operator *(const vec4& o) const { return vec4(this->x * o.x, this->y * o.y, this->z * o.z, this->w * o.w); }
    inline vec4 operator /(const vec4& o) const { return vec4(this->x / o.x, this->y / o.y, this->z / o.z, this->w / o.w); }
    inline vec4 operator +(const vec4& o) const { return vec4(this->x + o.x, this->y + o.y, this->z + o.z, this->w + o.w); }
    inline vec4 operator -(const vec4& o) const { return vec4(this->x - o.x, this->y - o.y, this->z - o.z, this->w - o.w); }

    float& operator[] (int i) { assert(i >= 0 && i < 4); return *(&x + i); }
};

inline vec4 operator *(float scalar, const vec4& vec) { return vec4(scalar * vec.x, scalar * vec.y, scalar * vec.z, scalar * vec.w); }
inline vec4 operator /(float scalar, const vec4& vec) { return vec4(scalar / vec.x, scalar / vec.y, scalar / vec.z, scalar / vec.w); }
inline vec4 operator +(float scalar, const vec4& vec) { return vec4(scalar + vec.x, scalar + vec.y, scalar + vec.z, scalar + vec.w); }
inline vec4 operator -(float scalar, const vec4& vec) { return vec4(scalar - vec.x, scalar - vec.y, scalar - vec.z, scalar - vec.w); }

class ivec2
{
public:
    int x;
    int y;

public:
    ivec2() {}
    ivec2(int nx, int ny) { x = nx; y = ny; }
    ivec2(vec2 o) { x = (int)o.x; y = (int)o.y; }
    //virtual ~ivec2() {}

    inline void Set(int nx, int ny) { x = nx; y = ny; }
    inline float LengthSquared() const { return (float)x * x + y * y; }
    inline float Length() const { return sqrtf((float)x * x + y * y); }
    inline float DistanceFrom(const ivec2 o) const { return sqrtf(((float)x - o.x) * (x - o.x) + (y - o.y) * (y - o.y)); }

    //inline ivec2 Normalize() const {float len = Length(); if( fequal(len,0) ) return ivec2(x,y); len = 1.0f/len; return ivec2(x*len, y*len);}

    inline ivec2 WithX(int x) const { return ivec2(x, this->y); }
    inline ivec2 WithY(int y) const { return ivec2(this->x, y); }

    inline bool operator ==(const ivec2& o) const { return this->x == o.x && this->y == o.y; }
    inline bool operator !=(const ivec2& o) const { return this->x != o.x || this->y != o.y; }

    inline ivec2 operator -() const { return ivec2(-this->x, -this->y); }
    inline vec2 operator *(const float o) const { return vec2(this->x * o, this->y * o); }
    inline vec2 operator /(const float o) const { return vec2(this->x / o, this->y / o); }
    inline vec2 operator +(const float o) const { return vec2(this->x + o, this->y + o); }
    inline vec2 operator -(const float o) const { return vec2(this->x - o, this->y - o); }
    inline ivec2 operator *(const int o) const { return ivec2(this->x * o, this->y * o); }
    inline ivec2 operator /(const int o) const { return ivec2(this->x / o, this->y / o); }
    inline ivec2 operator +(const int o) const { return ivec2(this->x + o, this->y + o); }
    inline ivec2 operator -(const int o) const { return ivec2(this->x - o, this->y - o); }
    inline vec2 operator *(const vec2& o) const { return vec2(this->x * o.x, this->y * o.y); }
    inline vec2 operator /(const vec2& o) const { return vec2(this->x / o.x, this->y / o.y); }
    inline vec2 operator +(const vec2& o) const { return vec2(this->x + o.x, this->y + o.y); }
    inline vec2 operator -(const vec2& o) const { return vec2(this->x - o.x, this->y - o.y); }
    inline ivec2 operator *(const ivec2& o) const { return ivec2(this->x * o.x, this->y * o.y); }
    inline ivec2 operator /(const ivec2& o) const { return ivec2(this->x / o.x, this->y / o.y); }
    inline ivec2 operator +(const ivec2& o) const { return ivec2(this->x + o.x, this->y + o.y); }
    inline ivec2 operator -(const ivec2& o) const { return ivec2(this->x - o.x, this->y - o.y); }

    inline ivec2 operator *=(const int o) { this->x *= o; this->y *= o; return *this; }
    inline ivec2 operator /=(const int o) { this->x /= o; this->y /= o; return *this; }
    inline ivec2 operator +=(const int o) { this->x += o; this->y += o; return *this; }
    inline ivec2 operator -=(const int o) { this->x -= o; this->y -= o; return *this; }
    inline ivec2 operator *=(const ivec2& o) { this->x *= o.x; this->y *= o.y; return *this; }
    inline ivec2 operator /=(const ivec2& o) { this->x /= o.x; this->y /= o.y; return *this; }
    inline ivec2 operator +=(const ivec2& o) { this->x += o.x; this->y += o.y; return *this; }
    inline ivec2 operator -=(const ivec2& o) { this->x -= o.x; this->y -= o.y; return *this; }

    int& operator[] (int i) { assert(i >= 0 && i < 2); return *(&x + i); }
};

inline ivec2 operator *(int scalar, const ivec2& vec) { return ivec2(scalar * vec.x, scalar * vec.y); }
inline ivec2 operator /(int scalar, const ivec2& vec) { return ivec2(scalar / vec.x, scalar / vec.y); }
inline ivec2 operator +(int scalar, const ivec2& vec) { return ivec2(scalar + vec.x, scalar + vec.y); }
inline ivec2 operator -(int scalar, const ivec2& vec) { return ivec2(scalar - vec.x, scalar - vec.y); }

class ivec3
{
public:
    int x;
    int y;
    int z;

public:
    ivec3() {}
    ivec3(int nx, int ny, int nz) { x = nx; y = ny; z = nz; }
    //virtual ~ivec3() {}

    inline void Set(int nx, int ny, int nz) { x = nx; y = ny; z = nz; }
    inline void Set(ivec3 vec) { x = vec.x; y = vec.y; z = vec.z; }
    inline int LengthSquared() const { return x * x + y * y + z * z; }
    inline float Length() const { return sqrtf((float)x * x + y * y + z * z); }
    inline float DistanceFrom(const ivec3 o) const { return sqrtf(((float)x - o.x) * (x - o.x) + (y - o.y) * (y - o.y) + (z - o.z) * (z - o.z)); }

    //inline ivec3 Normalize() const {float len = Length(); if( fequal(len,0) ) return ivec3(x,y,z); len = 1.0f/len; return ivec3(x*len, y*len, z*len);}
    //inline ivec3 Cross(const ivec3& o) const {return ivec3( (y*o.z - z*o.y), (z*o.x - x*o.z), (x*o.y - y*o.x) );}

    inline vec3 MultiplyComponents(const vec3& o) const { return vec3(this->x * o.x, this->y * o.y, this->z * o.z); }
    inline ivec3 MultiplyComponents(const ivec3& o) const { return ivec3(this->x * o.x, this->y * o.y, this->z * o.z); }

    inline ivec3 WithX(int x) const { return ivec3(x, this->y, this->z); }
    inline ivec3 WithY(int y) const { return ivec3(this->x, y, this->z); }
    inline ivec3 WithZ(int z) const { return ivec3(this->x, this->y, z); }

    inline bool operator ==(const ivec3& o) const { return this->x == o.x && this->y == o.y && this->z == o.z; }
    inline bool operator !=(const ivec3& o) const { return this->x != o.x || this->y != o.y || this->z != o.z; }

    inline ivec3 operator -() const { return ivec3(-this->x, -this->y, -this->z); }
    inline vec3 operator *(const float o) const { return vec3(this->x * o, this->y * o, this->z * o); }
    inline vec3 operator /(const float o) const { return vec3(this->x / o, this->y / o, this->z / o); }
    inline vec3 operator +(const float o) const { return vec3(this->x + o, this->y + o, this->z + o); }
    inline vec3 operator -(const float o) const { return vec3(this->x - o, this->y - o, this->z - o); }
    inline ivec3 operator *(const int o) const { return ivec3(this->x * o, this->y * o, this->z * o); }
    inline ivec3 operator /(const int o) const { return ivec3(this->x / o, this->y / o, this->z / o); }
    inline ivec3 operator +(const int o) const { return ivec3(this->x + o, this->y + o, this->z + o); }
    inline ivec3 operator -(const int o) const { return ivec3(this->x - o, this->y - o, this->z - o); }
    inline vec3 operator *(const vec3& o) const { return vec3(this->x * o.x, this->y * o.y, this->z * o.z); }
    inline vec3 operator /(const vec3& o) const { return vec3(this->x / o.x, this->y / o.y, this->z / o.z); }
    inline vec3 operator +(const vec3& o) const { return vec3(this->x + o.x, this->y + o.y, this->z + o.z); }
    inline vec3 operator -(const vec3& o) const { return vec3(this->x - o.x, this->y - o.y, this->z - o.z); }
    inline ivec3 operator *(const ivec3& o) const { return ivec3(this->x * o.x, this->y * o.y, this->z * o.z); }
    inline ivec3 operator /(const ivec3& o) const { return ivec3(this->x / o.x, this->y / o.y, this->z / o.z); }
    inline ivec3 operator +(const ivec3& o) const { return ivec3(this->x + o.x, this->y + o.y, this->z + o.z); }
    inline ivec3 operator -(const ivec3& o) const { return ivec3(this->x - o.x, this->y - o.y, this->z - o.z); }

    //inline ivec3 operator *=(const float o) { this->x *= o; this->y *= o; this->z *= o; return *this; }
    //inline ivec3 operator /=(const float o) { this->x /= o; this->y /= o; this->z /= o; return *this; }
    //inline ivec3 operator +=(const float o) { this->x += o; this->y += o; this->z += o; return *this; }
    //inline ivec3 operator -=(const float o) { this->x -= o; this->y -= o; this->z -= o; return *this; }
    //inline ivec3 operator +=(const vec3& o) { this->x += o.x; this->y += o.y; this->z += o.z; return *this; }
    //inline ivec3 operator -=(const vec3& o) { this->x -= o.x; this->y -= o.y; this->z -= o.z; return *this; }
    inline ivec3 operator +=(const ivec3& o) { this->x += o.x; this->y += o.y; this->z += o.z; return *this; }
    inline ivec3 operator -=(const ivec3& o) { this->x -= o.x; this->y -= o.y; this->z -= o.z; return *this; }

    int& operator[] (int i) { assert(i >= 0 && i < 3); return *(&x + i); }
};

inline vec3 operator *(float scalar, const ivec3& vec) { return vec3(scalar * vec.x, scalar * vec.y, scalar * vec.z); }
inline vec3 operator /(float scalar, const ivec3& vec) { return vec3(scalar / vec.x, scalar / vec.y, scalar / vec.z); }
inline vec3 operator +(float scalar, const ivec3& vec) { return vec3(scalar + vec.x, scalar + vec.y, scalar + vec.z); }
inline vec3 operator -(float scalar, const ivec3& vec) { return vec3(scalar - vec.x, scalar - vec.y, scalar - vec.z); }
inline ivec3 operator *(int scalar, const ivec3& vec) { return ivec3(scalar * vec.x, scalar * vec.y, scalar * vec.z); }
inline ivec3 operator /(int scalar, const ivec3& vec) { return ivec3(scalar / vec.x, scalar / vec.y, scalar / vec.z); }
inline ivec3 operator +(int scalar, const ivec3& vec) { return ivec3(scalar + vec.x, scalar + vec.y, scalar + vec.z); }
inline ivec3 operator -(int scalar, const ivec3& vec) { return ivec3(scalar - vec.x, scalar - vec.y, scalar - vec.z); }

class ivec4
{
public:
    int x;
    int y;
    int z;
    int w;

public:
    ivec4() {}
    ivec4(int nx, int ny, int nz, int nw) { x = nx; y = ny; z = nz; w = nw; }
    //virtual ~ivec4() {}

    inline void Set(int nx, int ny, int nz, int nw) { x = nx; y = ny; z = nz; w = nw; }
    inline float LengthSquared() const { return (float)x * x + y * y + z * z + w * w; }
    inline float Length() const { return sqrtf((float)x * x + y * y + z * z + w * w); }
    inline float DistanceFrom(const ivec4 o) const { return sqrtf(((float)x - o.x) * (x - o.x) + (y - o.y) * (y - o.y) + (z - o.z) * (z - o.z) + (w - o.w) * (w - o.w)); }

    //inline ivec4 Normalize() const {float len = Length(); if( fequal(len,0) ) return ivec4(x,y,z); len = 1.0f/len; return ivec4(x*len, y*len, z*len);}
    //inline ivec4 Cross(const ivec4& o) const {return ivec4( (y*o.z - z*o.y), (z*o.x - x*o.z), (x*o.y - y*o.x) );}

    inline ivec4 WithX(int x) const { return ivec4(x, this->y, this->z, this->w); }
    inline ivec4 WithY(int y) const { return ivec4(this->x, y, this->z, this->w); }
    inline ivec4 WithZ(int z) const { return ivec4(this->x, this->y, z, this->w); }
    inline ivec4 WithW(int w) const { return ivec4(this->x, this->y, this->z, w); }

    inline bool operator ==(const ivec4& o) const { return this->x == o.x && this->y == o.y && this->z == o.z && this->w == o.w; }
    inline bool operator !=(const ivec4& o) const { return this->x != o.x || this->y != o.y || this->z != o.z || this->w != o.w; }

    inline ivec4 operator -() const { return ivec4(-this->x, -this->y, -this->z, -this->w); }
    inline ivec4 operator +(const ivec4& o) const { return ivec4(this->x + o.x, this->y + o.y, this->z + o.z, this->w + o.w); }
    inline ivec4 operator -(const ivec4& o) const { return ivec4(this->x - o.x, this->y - o.y, this->z - o.z, this->w - o.w); }

    int& operator[] (int i) { assert(i >= 0 && i < 4); return *(&x + i); }
};

inline ivec4 operator *(int scalar, const ivec4& vec) { return ivec4(scalar * vec.x, scalar * vec.y, scalar * vec.z, scalar * vec.w); }
inline ivec4 operator /(int scalar, const ivec4& vec) { return ivec4(scalar / vec.x, scalar / vec.y, scalar / vec.z, scalar / vec.w); }
inline ivec4 operator +(int scalar, const ivec4& vec) { return ivec4(scalar + vec.x, scalar + vec.y, scalar + vec.z, scalar + vec.w); }
inline ivec4 operator -(int scalar, const ivec4& vec) { return ivec4(scalar - vec.x, scalar - vec.y, scalar - vec.z, scalar - vec.w); }

class MyRect
{
public:
    int x;
    int y;
    int w;
    int h;

public:
    MyRect() {}
    MyRect(int nx, int ny, int nw, int nh) { x = nx; y = ny; w = nw; h = nh; }

    void Set(int nx, int ny, int nw, int nh) { x = nx; y = ny; w = nw; h = nh; }
};

template <class MyType> class vec2T
{
public:
    MyType x;
    MyType y;

public:
    vec2T() {}
    vec2T(MyType nx, MyType ny) { x = nx; y = ny; }
    //vec2T(MyType nx, MyType ny) { x = (float)nx; y = (float)ny; }
    //virtual ~ivec2() {}

    inline void Set(MyType nx, MyType ny) { x = nx; y = ny; }
    inline float LengthSquared() const { return (float)x * x + y * y; }
    inline float Length() const { return sqrtf((float)x * x + y * y); }
    inline float DistanceFrom(const vec2T<MyType>& o) const { return sqrtf(((float)x - o.x) * (x - o.x) + (y - o.y) * (y - o.y)); }

    inline vec2T<MyType> GetNormalized() const { float len = Length(); if (fequal(len, 0)) return vec2T<MyType>(x, y); len = 1.0f / len; return vec2T<MyType>(x * len, y * len); }
    //inline vec2T<MyType> Normalize() const {float len = Length(); if( fequal(len,0) ) return vec2T<MyType>(x,y); len = 1.0f/len; return vec2T<MyType>(x*len, y*len);}

    inline vec2T<MyType> WithX(MyType x) const { return vec2T<MyType>(x, this->y); }
    inline vec2T<MyType> WithY(MyType y) const { return vec2T<MyType>(this->x, y); }

    inline bool operator ==(const vec2T<MyType>& o) const { return this->x == o.x && this->y == o.y; }
    inline bool operator !=(const vec2T<MyType>& o) const { return this->x != o.x || this->y != o.y; }

    inline vec2T<MyType> operator -() const { return vec2T<MyType>(-this->x, -this->y); }

    inline vec2T<MyType> operator *(const float o) const { return vec2T<MyType>(this->x * o, this->y * o); }
    inline vec2T<MyType> operator /(const float o) const { return vec2T<MyType>(this->x / o, this->y / o); }
    inline vec2T<MyType> operator +(const vec2& o) const { return vec2T<MyType>(this->x + o.x, this->y + o.y); }
    inline vec2T<MyType> operator -(const vec2& o) const { return vec2T<MyType>(this->x - o.x, this->y - o.y); }

    inline vec2T<MyType> operator *=(const float o) { this->x *= o; this->y *= o; return *this; }
    inline vec2T<MyType> operator /=(const float o) { this->x /= o; this->y /= o; return *this; }
    inline vec2T<MyType> operator +=(const vec2T<MyType>& o) { this->x += o.x; this->y += o.y; return *this; }
    inline vec2T<MyType> operator -=(const vec2T<MyType>& o) { this->x -= o.x; this->y -= o.y; return *this; }
};

}

#endif //__VECTOR_H__
