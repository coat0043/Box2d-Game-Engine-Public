#include "FrameworkPCH.h"

#include "ResourceManager.h"
#include "Utility/ShaderProgram.h"
#include "Mesh.h"
#include "Texture.h"
#include "Material.h"
#include "Sound.h"

namespace fw {

ResourceManager::ResourceManager()
{

}

ResourceManager::~ResourceManager()
{
    for (std::pair<std::string, ShaderProgram*> object : m_pShaders)
    {
        delete object.second;
    }

    for (std::pair<std::string, Mesh*> object : m_pMeshes)
    {
        delete object.second;
    }

    for (std::pair<std::string, Texture*> object : m_pTextures)
    {
        delete object.second;
    }

    for (std::pair<std::string, Material*> object : m_pMaterials)
    {
        delete object.second;
    }

    for (std::pair<std::string, Sound*> object : m_pSound)
    {
        delete object.second;
    }
}

void ResourceManager::AddShader(const char* name, ShaderProgram* pShader)
{
    m_pShaders[name] = pShader;

}

void ResourceManager::AddShader(const char* name, const char* vertFilename, const char* fragFilename)
{
    ShaderProgram* temp = new ShaderProgram(vertFilename, fragFilename);
    AddShader(name, temp);
}

void ResourceManager::AddMesh(const char* name, Mesh* pMesh)
{
    m_pMeshes[name] = pMesh;
}

void ResourceManager::AddMesh(const char* name, int primitiveType, int numVertices, const VertexFormat* pVertices)
{
    Mesh* temp = new Mesh(primitiveType, numVertices, pVertices);
    AddMesh(name, temp);
}

void ResourceManager::AddTexture(const char* name, Texture* pTexture)
{
    m_pTextures[name] = pTexture;
}

void ResourceManager::AddTexture(const char* name, const char* filename)
{
    Texture* temp = new Texture(filename);
    AddTexture(name, temp);
}

void ResourceManager::AddSound(const char* name, Sound* pSound)
{
    m_pSound[name] = pSound;
}

void ResourceManager::AddSound(const char* name, float Importance, const char* filename)
{
    Sound* temp = new Sound(Importance, filename);
    AddSound(name, temp);
}

void ResourceManager::AddMaterial(const char* name, Material* pMaterial)
{
    m_pMaterials[name] = pMaterial;
}

void ResourceManager::AddMaterial(const char* name, ShaderProgram* pShaderProgram, Color pColor)
{
    Material* temp = new Material(pShaderProgram, pColor);
    AddMaterial(name, temp);
}

void ResourceManager::AddMaterial(const char* name, ShaderProgram* pShaderProgram, Texture* pTexture, Color pColor, vec2 m_UVScale, vec2 pUVOffset)
{
    Material* temp = new Material(pShaderProgram, pTexture, pColor, m_UVScale, pUVOffset);
    AddMaterial(name, temp);
}

std::string ResourceManager::FindMeshName(Mesh* pMesh)
{
    for (auto& it = m_pMeshes.begin(); it != m_pMeshes.end(); it++)
    {
        if (it->second == pMesh)
        {
            return it->first;
        }
    }

    return "Mesh not found";
}

std::string ResourceManager::FindShaderName(ShaderProgram* pShader)
{
    for (auto& it = m_pShaders.begin(); it != m_pShaders.end(); it++)
    {
        if (it->second == pShader)
        {
            return it->first;
        }
    }

    return "Shader not found";
}

std::string ResourceManager::FindTextureName(Texture* pTexture)
{
    for(auto& it = m_pTextures.begin(); it != m_pTextures.end(); it++)
    {
        if(it->second == pTexture)
        {
            return it->first;
        }
    }

    return "Texture not found";
}

std::string ResourceManager::FindMaterialName(Material* pMaterial)
{
    for (auto& it = m_pMaterials.begin(); it != m_pMaterials.end(); it++)
    {
        if (it->second == pMaterial)
        {
            return it->first;
        }
    }

    return "Material not found";
}

std::string ResourceManager::FindSoundName(Sound* pSound)
{
    for (auto& it = m_pSound.begin(); it != m_pSound.end(); it++)
    {
        if (it->second == pSound)
        {
            return it->first;
        }
    }

    return "Sound not found";
}

} // namespace fw
