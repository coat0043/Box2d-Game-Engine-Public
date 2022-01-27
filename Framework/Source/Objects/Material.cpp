#include "FrameworkPCH.h"

#include "Material.h"
#include "Texture.h"

namespace fw {


Material::Material(ShaderProgram* pShaderProgram, TexturePack texturePack, Color color, vec2 uvScale, vec2 uvOffset)
{
    m_pShaderProgram = pShaderProgram;

    m_pTexture = texturePack.m_pDiffuseTexture;
    m_pNormalTexture = texturePack.m_pNormalTexture;
    m_pCubemap = texturePack.m_pCubemapTexture;

    assert(m_pTexture == nullptr || m_pTexture->IsCubemap() == false);
    assert(m_pCubemap == nullptr || m_pCubemap->IsCubemap() == true);

    m_Color = color;
    m_UVScale = uvScale;
    m_UVOffset = uvOffset;

    m_pDeferredBufferShaderProgram = nullptr;
}

Material::Material(ShaderProgram* pShaderProgram, ShaderProgram* pDeferredBufferShader, TexturePack texturePack, Color color, vec2 uvScale, vec2 uvOffset)
{
    m_pShaderProgram = pShaderProgram;

    m_pTexture = texturePack.m_pDiffuseTexture;
    m_pNormalTexture = texturePack.m_pNormalTexture;
    m_pCubemap = texturePack.m_pCubemapTexture;

    assert(m_pTexture == nullptr || m_pTexture->IsCubemap() == false);
    assert(m_pCubemap == nullptr || m_pCubemap->IsCubemap() == true);

    m_Color = color;
    m_UVScale = uvScale;
    m_UVOffset = uvOffset;

    m_pDeferredBufferShaderProgram = pDeferredBufferShader;
}

Material::Material(ShaderProgram* pShaderProgram, Color color)
{
    m_pShaderProgram = pShaderProgram;
    m_Color = color;
}

Material::~Material()
{
}

} // namespace fw
