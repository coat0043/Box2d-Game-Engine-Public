#pragma once

#include "Math/Vector.h"
#include "Utility/Color.h"

namespace fw {

class ShaderProgram;
class Texture;

struct TexturePack
{
    TexturePack( Texture* pDiffuse, Texture* pNormal = nullptr, Texture* pCubemap = nullptr)
    {
        m_pDiffuseTexture = pDiffuse; m_pNormalTexture = pNormal; m_pCubemapTexture = pCubemap;
    };

    Texture* m_pDiffuseTexture = nullptr;
    Texture* m_pNormalTexture = nullptr;
    Texture* m_pCubemapTexture = nullptr;
};

class Material
{
public:

    Material(ShaderProgram* pShaderProgram, TexturePack texturePack, Color color = Color::White(), vec2 uvScale = 1, vec2 uvOffset = 0);
    Material(ShaderProgram* pShaderProgram, ShaderProgram* pDeferredBufferShader, TexturePack texturePack, Color color = Color::White(),  vec2 uvScale = 1, vec2 uvOffset = 0);

    Material(ShaderProgram* pShaderProgram, Color color);
    ~Material();

    ShaderProgram* GetShader() { return m_pShaderProgram; }
    ShaderProgram* GetDeferredBufferShader() { return m_pDeferredBufferShaderProgram; }

    Texture* GetTexture() { return m_pTexture; }
    Texture* GetNormalTexture() { return m_pNormalTexture; }
    Texture* GetCubemap() { return m_pCubemap; }
    float GetShine() { return m_Shineness; }
    Color GetColor() { return m_Color; }
    vec2 GetUVScale() { return m_UVScale; }
    vec2 GetUVOffset() { return m_UVOffset; }

    void SetShader(ShaderProgram* pShader) { m_pShaderProgram = pShader; }
    void SetDeferredBufferShader(ShaderProgram* pShader) { m_pDeferredBufferShaderProgram = pShader; }
    void SetTexture(Texture* pTexture) { m_pTexture = pTexture; }
    void SetNormalTexture(Texture* pNormalTexture) { m_pNormalTexture = pNormalTexture; }
    void SetCubemap(Texture* pCubemap) { m_pCubemap = pCubemap; }
    void SetColor(Color color) { m_Color = color; }
    void SetUVScale(vec2 uvScale) { m_UVScale = uvScale; }
    void SetUVOffset(vec2 uvOffset) { m_UVOffset = uvOffset; }
    void SetShineness(float shineness) { m_Shineness = shineness; }

protected:

    ShaderProgram* m_pShaderProgram = nullptr;
    ShaderProgram* m_pDeferredBufferShaderProgram = nullptr;

    Texture* m_pTexture = nullptr;
    Texture* m_pCubemap = nullptr;
    Texture* m_pNormalTexture = nullptr;
    Color m_Color = Color(1, 1, 1, 1);
    vec2 m_UVScale = vec2(1, 1);
    vec2 m_UVOffset = vec2(0, 0);

public:
    float m_Shineness = 0;
};

} // namespace fw
