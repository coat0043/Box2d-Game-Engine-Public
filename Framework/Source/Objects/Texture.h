#pragma once
#include "Math/Vector.h"

namespace fw {

class Texture
{
public:
    Texture();
    Texture(const char* filename);
    Texture(const char* filename[6]);
    Texture(GLuint textureHandle);
    virtual ~Texture();

    GLuint GetHandle();
    vec2 GetSize() { return vec2((float)m_Width, (float)m_Height); }  // Returns the size of texture

    bool IsCubemap() { return m_IsCubemap; }

protected:
    GLuint m_TextureHandle = 0;
    int m_Width = 0;
    int m_Height = 0;

    bool m_IsCubemap = false;
    bool m_FreeTextureWhenDestroyed = false;
};

} // namespace fw
