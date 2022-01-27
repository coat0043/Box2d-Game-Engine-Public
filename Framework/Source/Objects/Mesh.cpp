#include "FrameworkPCH.h"

#include "Mesh.h"
#include "Components/CameraComponent.h"
#include "Components/TransformComponent.h"
#include "Components/LightComponent.h"
#include "Components/SpotLightComponent.h"

#include "Math/MyMatrix.h"
#include "Objects/Material.h"
#include "Objects/Texture.h"
#include "Utility/Helpers.h"
#include "Utility/ShaderProgram.h"

//temp
#include "Utility/FrameBufferObject.h"

namespace fw {

Mesh::Mesh()
{
}

Mesh::Mesh(int primitiveType, int numVertices, const VertexFormat* pVertices)
{
    CreateShape(primitiveType, numVertices, pVertices);
}

Mesh::Mesh(vec3 aCubeSize, bool facesInward)
{
    CreateCube(aCubeSize, facesInward);
}

Mesh::Mesh(vec2 aWorldSize, ivec2 aVertCount)
{
    CreatePlane(aWorldSize, aVertCount);
}

Mesh::Mesh(const char* filename)
{
    CreateFromOBJ(filename);
}

Mesh::~Mesh()
{
    glDeleteBuffers(1, &m_VBO);
}

void Mesh::Start(int primitiveType)
{
    assert(m_Editable == false);

    m_Verts.clear();

    m_PrimitiveType = primitiveType;

    m_Editable = true;
}

void Mesh::AddVertex(const VertexFormat nVert)
{
    assert(m_Editable);

    m_Verts.push_back(nVert);
}

void Mesh::AddVertex(const vec3 pos, const vec2 uv)
{
    assert(m_Editable);

    m_Verts.push_back(VertexFormat(pos.x, pos.y, pos.z, uv.x, uv.y));
}

void Mesh::AddVertex(const vec3 pos, unsigned char r, unsigned char g, unsigned char b, unsigned char a)
{
    m_Verts.push_back(VertexFormat(pos.x, pos.y, pos.z, r, g, b, a));
}

void Mesh::AddVertex(const vec3 pos, const float u, const float v)
{
    assert(m_Editable);

    m_Verts.push_back(VertexFormat(pos.x, pos.y, pos.z, u, v));
}

void Mesh::AddVertex(const float x, const float y, const float z, const float u, const float v)
{
    assert(m_Editable);

    m_Verts.push_back(VertexFormat(x, y, z, u, v));
}

void Mesh::AddVertex(const float x, const float y, const float z, const vec2 uv)
{
    assert(m_Editable);

    m_Verts.push_back(VertexFormat(x, y, z, uv.x, uv.y));
}

void Mesh::AddSprite(vec3 pos, vec2 size)
{
    assert(m_Editable);
    assert(m_PrimitiveType == GL_TRIANGLES);

    size /= 2;

    m_Verts.push_back(VertexFormat(pos.x - size.x, pos.y - size.y, pos.z, 0, 0)); // bl
    m_Verts.push_back(VertexFormat(pos.x - size.x, pos.y + size.y, pos.z, 0, 1)); // tl
    m_Verts.push_back(VertexFormat(pos.x + size.x, pos.y + size.y, pos.z, 1, 1)); // tr

    m_Verts.push_back(VertexFormat(pos.x - size.x, pos.y - size.y, pos.z, 0, 0)); // bl
    m_Verts.push_back(VertexFormat(pos.x + size.x, pos.y + size.y, pos.z, 1, 1)); // tr
    m_Verts.push_back(VertexFormat(pos.x + size.x, pos.y - size.y, pos.z, 1, 0)); // br
}

void Mesh::End()
{
    assert(m_Editable);

    m_Editable = false;

    if (m_Verts.size() == 0)
        return;

    glDeleteBuffers(1, &m_VBO);
    glGenBuffers(1, &m_VBO);

    m_NumVertices = (int)m_Verts.size();

    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(VertexFormat) * m_NumVertices, &m_Verts[0], GL_STATIC_DRAW);
}

void Mesh::CreateShape(int aPrimitiveType, int aNumVertices, const VertexFormat* pVertices)
{
    // Delete the old buffer if we had one.
    glDeleteBuffers(1, &m_VBO);

    // Generate a buffer for our vertex attributes.
    glGenBuffers(1, &m_VBO); // m_VBO is a GLuint.

    // Set this VBO to be the currently active one.
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

    m_PrimitiveType = aPrimitiveType;
    m_NumVertices = aNumVertices;

    // Copy our attribute data into the VBO.
    glBufferData(GL_ARRAY_BUFFER, sizeof(VertexFormat) * m_NumVertices, pVertices, GL_STATIC_DRAW);
}

void Mesh::CreateShape(int aPrimitiveType, int aNumVertices, const VertexFormat* pVertices, int aNumIndices, const unsigned int* pIndices)
{
    // Delete the old buffer if we had one.
    glDeleteBuffers(1, &m_IBO);

    // Generate a buffer for our indices.
    glGenBuffers(1, &m_IBO);

    // Set this IBO to be currently active
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);

    m_NumIndices = aNumIndices;

    // Copy our indices into the IBO
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * m_NumIndices, pIndices, GL_STATIC_DRAW);

    CreateShape(aPrimitiveType, aNumVertices, pVertices);
}

void Mesh::CreateCube(vec3 aSize, bool facesInward)
{
    const VertexFormat cubeVerts[] =
    {
        // Face One
        VertexFormat(-0.5f * aSize.x, -0.5f * aSize.y, -0.5f * aSize.z,   0.0f, 0.0f),  // lbf
        VertexFormat(-0.5f * aSize.x,  0.5f * aSize.y, -0.5f * aSize.z,   0.0f, 0.33f), // ltf
        VertexFormat(0.5f * aSize.x, -0.5f * aSize.y, -0.5f * aSize.z,   0.5f, 0.0f),  // rbf
        VertexFormat(0.5f * aSize.x,  0.5f * aSize.y, -0.5f * aSize.z,   0.5f, 0.33f), // rtf

        // Face Two
        VertexFormat(0.5f * aSize.x, -0.5f * aSize.y, -0.5f * aSize.z,   0.0f, 0.33f),
        VertexFormat(0.5f * aSize.x,  0.5f * aSize.y, -0.5f * aSize.z,   0.0f, 0.66f),
        VertexFormat(0.5f * aSize.x, -0.5f * aSize.y,  0.5f * aSize.z,   0.5f, 0.33f),
        VertexFormat(0.5f * aSize.x,  0.5f * aSize.y,  0.5f * aSize.z,   0.5f, 0.66f),

        // Face Three
        VertexFormat(0.5f * aSize.x, 0.5f * aSize.y, -0.5f * aSize.z,   0.0f, 0.66f),
        VertexFormat(-0.5f * aSize.x, 0.5f * aSize.y, -0.5f * aSize.z,   0.0f, 1.0f),
        VertexFormat(0.5f * aSize.x, 0.5f * aSize.y,  0.5f * aSize.z,   0.5f, 0.66f),
        VertexFormat(-0.5f * aSize.x, 0.5f * aSize.y,  0.5f * aSize.z,   0.5f, 1.0f),

        // Face Four
        VertexFormat(-0.5f * aSize.x,  0.5f * aSize.y, -0.5f * aSize.z,   0.5f, 0.66f),
        VertexFormat(-0.5f * aSize.x, -0.5f * aSize.y, -0.5f * aSize.z,   0.5f, 1.0f),
        VertexFormat(-0.5f * aSize.x,  0.5f * aSize.y,  0.5f * aSize.z,   1.0f, 0.66f),
        VertexFormat(-0.5f * aSize.x, -0.5f * aSize.y,  0.5f * aSize.z,   1.0f, 1.0f),

        // Face Five
        VertexFormat(-0.5f * aSize.x, -0.5f * aSize.y, -0.5f * aSize.z,   0.5f, 0.33f), // lbf
        VertexFormat(0.5f * aSize.x, -0.5f * aSize.y, -0.5f * aSize.z,   0.5f, 0.66f), // rbf
        VertexFormat(-0.5f * aSize.x, -0.5f * aSize.y,  0.5f * aSize.z,   1.0f, 0.33f), // lbb 
        VertexFormat(0.5f * aSize.x, -0.5f * aSize.y,  0.5f * aSize.z,   1.0f, 0.66f), // rbb

        // Face Six 
        VertexFormat(0.5f * aSize.x, -0.5f * aSize.y,  0.5f * aSize.z,   0.5f, 0.0f),
        VertexFormat(0.5f * aSize.x,  0.5f * aSize.y,  0.5f * aSize.z,   0.5f, 0.33f),
        VertexFormat(-0.5f * aSize.x, -0.5f * aSize.y,  0.5f * aSize.z,   1.0f, 0.0f),
        VertexFormat(-0.5f * aSize.x,  0.5f * aSize.y,  0.5f * aSize.z,   1.0f, 0.33f),
    };

    unsigned int cubeIndices[] =
    {
        // Face One
        0, 1, 2, 2, 1, 3,

        // Face Two
        4, 5, 6, 6, 5, 7,

        // Face Three
        8, 9, 10, 10, 9, 11,

        // Face Four
        12, 13, 14, 14, 13, 15,

        // Face Five
        16, 17, 18, 18, 17, 19,

        // Face Six
        20, 21, 22, 22, 21, 23,
    };

    if (facesInward)
    {
        for (int i = 0; i < 12; i++)
        {
            MySwap(cubeIndices[i * 3 + 0], cubeIndices[i * 3 + 1]);
        }
    }

    const int meshPrimType = GL_TRIANGLES;
    const int meshNumVerts = sizeof(cubeVerts) / sizeof(fw::VertexFormat);
    const int meshNumIndices = sizeof(cubeIndices) / sizeof(unsigned int);

    CreateShape(meshPrimType, meshNumVerts, cubeVerts, meshNumIndices, cubeIndices);
}

void Mesh::CreatePlane(vec2 aWorldSize, ivec2 aVertCount)
{
    std::vector<VertexFormat> verts;
    std::vector<unsigned int> indices;

    verts.reserve(aVertCount.x * aVertCount.y);
    indices.reserve((aVertCount.x - 1) * (aVertCount.y - 1) * 6);

    float stepSizeX = aWorldSize.x / (aVertCount.x - 1);
    float stepSizeY = aWorldSize.y / (aVertCount.y - 1);
    float uvSizeX = 5.0f / (aVertCount.x - 1);
    float uvSizeY = 5.0f / (aVertCount.y - 1);

    for (int y = 0; y < aVertCount.y; y++)
    {
        for (int x = 0; x < aVertCount.x; x++)
        {
            verts.push_back(VertexFormat(stepSizeX * x, 0, stepSizeY * y, uvSizeX * x, uvSizeY * y));
        }
    }

    for (int y = 0; y < aVertCount.y - 1; y++)
    {
        for (int x = 0; x < aVertCount.x - 1; x++)
        {
            unsigned int index = y * aVertCount.x + x;

            indices.push_back(index);
            indices.push_back(index + aVertCount.x);
            indices.push_back(index + 1);
            indices.push_back(index + 1);
            indices.push_back(index + aVertCount.x);
            indices.push_back(index + aVertCount.x + 1);
        }
    }

    CreateShape(GL_TRIANGLES, (int)verts.size(), &verts[0], (int)indices.size(), &indices[0]);
}

void Mesh::CreateFromOBJ(const char* filename)
{
    long length = 0;
    char* buffer = LoadCompleteFile(filename, &length);
    if (buffer == 0 || length == 0)
    {
        delete[] buffer;
        return;
    }

    std::vector<vec3> positions;
    std::vector<vec2> uvs;
    std::vector<vec3> normals;
    std::vector<VertexFormat> verts;

    char* next_token = 0;
    char* line = strtok_s(buffer, "\n", &next_token);
    while (line)
    {
        if (line[0] == 'v')
        {
            if (line[1] == 't')
            {
                vec2 value;
                sscanf_s(line, "vt %f %f", &value.x, &value.y);
                uvs.push_back(value);
            }
            else if (line[1] == 'n')
            {
                vec3 value;
                sscanf_s(line, "vn %f %f %f", &value.x, &value.y, &value.z);
                normals.push_back(value);
            }
            else if (line[1] == ' ')
            {
                vec3 value;
                sscanf_s(line, "v %f %f %f", &value.x, &value.y, &value.z);
                positions.push_back(value);
            }
        }

        // "f 1/1/1 2/2/1 3/3/1"
        if (line[0] == 'f')
        {
            ivec3 v1, v2, v3;
            sscanf_s(line, "f %d/%d/%d %d/%d/%d %d/%d/%d", &v1.x, &v1.y, &v1.z, &v2.x, &v2.y, &v2.z, &v3.x, &v3.y, &v3.z);

            verts.push_back(VertexFormat(positions[v1.x - 1], uvs[v1.y - 1], normals[v1.z - 1]));
            verts.push_back(VertexFormat(positions[v2.x - 1], uvs[v2.y - 1], normals[v2.z - 1]));
            verts.push_back(VertexFormat(positions[v3.x - 1], uvs[v3.y - 1], normals[v3.z - 1]));
        }

        line = strtok_s(0, "\n", &next_token);
    }

    CreateShape(GL_TRIANGLES, (int)verts.size(), &verts[0]);

    delete[] buffer;
}

void Mesh::SetUniform1f(ShaderProgram* pShader, char* name, float value)
{
    int loc = glGetUniformLocation(pShader->GetProgram(), name);
    glUniform1f(loc, value);
}

void Mesh::SetUniform2f(ShaderProgram* pShader, char* name, vec2 value)
{
    int loc = glGetUniformLocation(pShader->GetProgram(), name);
    glUniform2f(loc, value.x, value.y);
}

void Mesh::SetUniform3f(ShaderProgram* pShader, char* name, vec3 value)
{
    int loc = glGetUniformLocation(pShader->GetProgram(), name);
    glUniform3f(loc, value.x, value.y, value.z);
}

void Mesh::SetUniform4f(ShaderProgram* pShader, char* name, vec4 value)
{
    int loc = glGetUniformLocation(pShader->GetProgram(), name);
    glUniform4f(loc, value.x, value.y, value.z, value.w);
}

void Mesh::SetUniform1i(ShaderProgram* pShader, char* name, int value)
{
    int loc = glGetUniformLocation(pShader->GetProgram(), name);
    glUniform1i(loc, value);
}

//void Mesh::Draw(CameraComponent* pCamera, MyMatrix* pWorldMatrix, ShaderProgram* pShader, Texture* pTexture, Color color, vec2 UVScale, vec2 UVOffset)
//{
//    glUseProgram(pShader->GetProgram());
//    Draw(pCamera, pWorldMatrix, pShader, pTexture, nullptr, color,1, UVScale, UVOffset, nullptr);
//}

void Mesh::Draw(CameraComponent* pCamera, MyMatrix* pWorldMatrix, Material* pMaterial, std::vector<Component*>* pLights)
{
    Draw(pCamera, pWorldMatrix, pMaterial->GetShader(), pMaterial->GetTexture(), pMaterial->GetNormalTexture(), pMaterial->GetCubemap(), pMaterial->GetColor(), pMaterial->GetUVScale(), pMaterial->GetUVOffset(), pLights);
}

void Mesh::Draw(CameraComponent* pCamera, TransformComponent* pTransform, Material* pMaterial, std::vector<Component*>* pLights)
{
    Draw(pCamera, pTransform->GetWorldTransform(), pMaterial->GetShader(), pMaterial->GetTexture(), pMaterial->GetNormalTexture(), pMaterial->GetCubemap(), pMaterial->GetColor(), pMaterial->GetUVScale(), pMaterial->GetUVOffset(), pLights);
}

void Mesh::Draw(CameraComponent* pCamera, MyMatrix* pWorldMatrix, ShaderProgram* pShader, Texture* pTexture, Texture* pNormalTexture, Texture* pCubemap, Color color, vec2 UVScale, vec2 UVOffset, std::vector<Component*>* pLights)
{
    glUseProgram(pShader->GetProgram());

    // Set this VBO to be the currently active one.
    glBindBuffer(GL_ARRAY_BUFFER, m_VBO);

    // Set this IBO to be the currently active one.
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);

    // Get the attribute variable's location from the shader.
    GLint locPosition = glGetAttribLocation(pShader->GetProgram(), "a_Position");
    if (locPosition != -1)
    {
        glEnableVertexAttribArray(locPosition);
        glVertexAttribPointer(locPosition, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)offsetof(VertexFormat, pos));
    }

    GLint locColor = glGetAttribLocation(pShader->GetProgram(), "a_Color");
    if (locColor != -1)
    {
        glEnableVertexAttribArray(locColor);
        glVertexAttribPointer(locColor, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(VertexFormat), (void*)offsetof(VertexFormat, color));
    }

    GLint locUVCoord = glGetAttribLocation(pShader->GetProgram(), "a_UVCoord");
    if (locUVCoord != -1)
    {
        glEnableVertexAttribArray(locUVCoord);
        glVertexAttribPointer(locUVCoord, 2, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)offsetof(VertexFormat, uv));
    }

    GLint locNormal = glGetAttribLocation(pShader->GetProgram(), "a_Normal");
    if (locNormal != -1)
    {
        glEnableVertexAttribArray(locNormal);
        glVertexAttribPointer(locNormal, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)offsetof(VertexFormat, normal));
    }

    // Setup our uniforms.
    {
        SetUniform1f(pShader, "u_Time", (float)GetSystemTimeSinceGameStart());
        SetUniform4f(pShader, "u_Color", vec4(color.r, color.g, color.b, color.a));

        SetUniform2f(pShader, "u_UVScale", UVScale);
        SetUniform2f(pShader, "u_UVOffset", UVOffset);

        SetUniform3f(pShader, "u_CameraPosition", pCamera->GetPosition());

        // Transforms.
        int locWorld = glGetUniformLocation(pShader->GetProgram(), "u_WorldTransform");
        glUniformMatrix4fv(locWorld, 1, false, &pWorldMatrix->m11);

        int locRotMatrix = glGetUniformLocation(pShader->GetProgram(), "u_RotationMatrix");
        vec3 rot = pWorldMatrix->GetEulerAngles();
        MyMatrix rotation;
        rotation.CreateRotation(rot);
        glUniformMatrix4fv(locRotMatrix, 1, false, &rotation.m11);

        int locView = glGetUniformLocation(pShader->GetProgram(), "u_ViewTransform");
        glUniformMatrix4fv(locView, 1, false, &pCamera->GetViewMatrix()->m11);

        int locProj = glGetUniformLocation(pShader->GetProgram(), "u_ProjectionMatrix");
        MyMatrix projMat;

        // Check the CameraComponent's ViewType and determine what kind of projection matrix to create.
        glUniformMatrix4fv(locProj, 1, false, &pCamera->GetProjectionMatrix()->m11);

        if (pTexture != nullptr)
        {
            assert(pTexture->IsCubemap() == false);

            int textureUnit = 0;
            glActiveTexture(GL_TEXTURE0 + textureUnit);
            glBindTexture(GL_TEXTURE_2D, pTexture->GetHandle());
            SetUniform1i(pShader, "u_Texture", textureUnit);
        }

        if (pNormalTexture != nullptr)
        {
            assert(pNormalTexture->IsCubemap() == false);

            int textureUnit = 1;

            glActiveTexture(GL_TEXTURE0 + textureUnit);
            glBindTexture(GL_TEXTURE_2D, pNormalTexture->GetHandle());
            SetUniform1i(pShader, "u_NormalMap", textureUnit);
        }

        if (pCubemap != nullptr)
        {
            assert(pCubemap->IsCubemap());

            int textureUnit = 2;
            glActiveTexture(GL_TEXTURE0 + textureUnit);
            glBindTexture(GL_TEXTURE_CUBE_MAP, pCubemap->GetHandle());
            SetUniform1i(pShader, "u_Cubemap", textureUnit);
        }

        if (pLights != nullptr)
        {
            SetUniform1i(pShader, "u_LightsCount", max((int)pLights->size(),6));

            if (pLights->size() > 0)
            {
                for (int i = 0; i < pLights->size(); i++)
                {
                    std::string fragVar = "u_Lights[" + std::to_string(i);

                    LightComponent* light = static_cast<LightComponent*>(pLights->at(i));

                    SetUniform3f(pShader, (fragVar + "].position").data(), light->GetTransform()->GetPosition());
                    SetUniform3f(pShader, (fragVar + "].direction").data(), light->GetTransform()->GetForward());
                    SetUniform3f(pShader, (fragVar + "].color").data(), light->GetColor());
                    SetUniform1f(pShader, (fragVar + "].range").data(), light->GetRange());
                    SetUniform1f(pShader, (fragVar + "].intensity").data(), light->GetIntensity());
                    // SetUniform1f(pShader, (fragVar + "].attenuation").data(), light->GetAttenuation());
                    // SetUniform1f(pShader, (fragVar + "].Ambient").data(), light->GetAmbient());
                    //SetUniform1f(pShader, (fragVar + "].SpecularIntensity").data(), light->GetAmbient());

                    char lightType = (char)light->GetLightType();

                    SetUniform1i(pShader, (fragVar + "].type").data(), lightType);

                    if (lightType == 1) // if it's a spotlight, give it its cone angle and falloff and direction.
                    {
                        //SetUniform1f(pShader, (fragVar + "].SpotConeAngle").data(), static_cast<SpotLightComponent*>(light)->GetSpotAngle());
                        //SetUniform1f(pShader, (fragVar + "].SpotConeFalloff").data(), static_cast<SpotLightComponent*>(light)->GetSpotFalloff());
                        SetUniform3f(pShader, (fragVar + "].direction").data(), light->GetTransform()->GetForward());
                    }

                    if (light->GetTextureCookie() != nullptr)
                    {
                        assert(light->GetTextureCookie()->GetHandle() == false);

                        MyMatrix Matr_light;
                        Matr_light.CreateSRT(1, light->GetTransform()->GetRotation(), light->GetTransform()->GetPosition());

                        MyMatrix Matr_lightProj;
                        MyMatrix Matr_lightMixed;

                        Matr_lightProj.CreatePerspectiveVFoV(light->GetRange(), 1, 0.01f, 1000.0f);

                        int locView = glGetUniformLocation(pShader->GetProgram(), &(fragVar + "ViewMatrix")[0]);
                        glUniformMatrix4fv(locView, 1, false, &Matr_lightMixed.m11);
                        
                        int textureUnit = 3;

                        glActiveTexture(GL_TEXTURE0 + textureUnit);
                        glBindTexture(GL_TEXTURE_2D, light->GetTextureCookie()->GetHandle());
                        SetUniform1i(pShader, "u_TextureCookie", textureUnit);

                    }
                }
            }
        }
    }

    // Draw the primitive.
    if (m_NumIndices == 0)
    {
        glDrawArrays(m_PrimitiveType, 0, m_NumVertices);
    }
    else
    {
        glDrawElements(m_PrimitiveType, m_NumIndices, GL_UNSIGNED_INT, (void*)0);
    }

    if (locPosition != -1)
    {
        glDisableVertexAttribArray(locPosition);
    }

    if (locColor != -1)
    {
        glDisableVertexAttribArray(locColor);
    }

    if (locUVCoord != -1)
    {
        glDisableVertexAttribArray(locUVCoord);
    }

    if (locNormal != -1)
    {
        glDisableVertexAttribArray(locNormal);
    }
}

void Mesh::DeferredRender_GeometryPass(CameraComponent* pCamera, TransformComponent* pTransform, Material* pMaterial)
{
    SetupDeferredRender();
    Draw(pCamera, pTransform->GetWorldTransform(), pMaterial->GetDeferredBufferShader(), pMaterial->GetTexture(), pMaterial->GetNormalTexture(), pMaterial->GetCubemap(), pMaterial->GetColor(), pMaterial->GetUVScale(), pMaterial->GetUVOffset(), nullptr);
}

void Mesh::DeferredRender_LightingPass(CameraComponent* pCamera, std::vector<Component*>* pLights, ShaderProgram* pLightingShader)
{
    glDisable(GL_DEPTH_TEST);
    glUseProgram(pLightingShader->GetProgram());

    for (size_t i = 0; i < pLights->size(); i++)
    {
        std::string baseString = "LightArray[" + std::to_string(i) + "]";
        LightComponent* L = static_cast<LightComponent*>(pLights->at(i));

        // The basics:
        SetUniform3f(pLightingShader, (baseString + ".Position").data(), L->GetTransform()->GetPosition());
        SetUniform3f(pLightingShader, (baseString + ".Color").data(), L->GetColor());
        SetUniform1f(pLightingShader, (baseString + ".Range").data(), L->GetRange());
        SetUniform1f(pLightingShader, (baseString + ".Intensity").data(), L->GetIntensity());
        SetUniform1f(pLightingShader, (baseString + ".Attenuation").data(), L->GetAttenuation());
        SetUniform1f(pLightingShader, (baseString + ".Ambient").data(), L->GetAmbient());
        SetUniform1f(pLightingShader, (baseString + ".SpecularIntensity").data(), L->GetSpecularIntensity());
        
        SetUniform1i(pLightingShader, (baseString + ".LightType").data(), (char)L->GetLightType());

        // For spotlights and directionals:
        SetUniform1f(pLightingShader, (baseString + ".SpotConeAngle").data(), static_cast<SpotLightComponent*>(L)->GetSpotAngle());
        SetUniform1f(pLightingShader, (baseString + ".SpotConeFalloff").data(), static_cast<SpotLightComponent*>(L)->GetSpotFalloff());
        SetUniform3f(pLightingShader, (baseString + ".Direction").data(), L->GetTransform()->GetForward());
        
    }

    SetUniform1i(pLightingShader, "u_NR_LIGHTS", (int)pLights->size());

    {
        int textureUnit = 4;

        glActiveTexture(GL_TEXTURE0 + textureUnit);
        glBindTexture(GL_TEXTURE_2D, m_gPositionBuff);
        SetUniform1i(pLightingShader, "u_PositionTex", textureUnit);
    }

    {
        int textureUnit = 5;

        glActiveTexture(GL_TEXTURE0 + textureUnit);
        glBindTexture(GL_TEXTURE_2D, m_gColorSpec);
        SetUniform1i(pLightingShader, "u_ColorSpecTex", textureUnit);
    }

    {
        int textureUnit = 6;

        glActiveTexture(GL_TEXTURE0 + textureUnit);
        glBindTexture(GL_TEXTURE_2D, m_gNormalBuff);
        SetUniform1i(pLightingShader, "u_NormalTex", textureUnit);
    }

    glActiveTexture(GL_TEXTURE0);

    SetUniform3f(pLightingShader, "u_CameraPosition", pCamera->GetPosition());

    DrawScreenQuad(pLightingShader);

    glEnable(GL_DEPTH_TEST);
}

void Mesh::DrawScreenQuad(ShaderProgram* pShader)
{
    glUseProgram(pShader->GetProgram());

    if (m_ScreenQuadVAO == 0)
    {
        float quadVertices[] = {
            // positions        // texture Coords
            -1.0f,  1.0f, 0.0f, 1.0f,
             1.0f,  1.0f, 1.0f, 1.0f,
             1.0f, -1.0f, 1.0f, 0.0f,

            -1.0f,  1.0f, 0.0f, 1.0f,
             1.0f, -1.0f, 1.0f, 0.0f,
            -1.0f, -1.0f, 0.0f, 0.0f,
        };

        // setup plane VAO
        glGenVertexArrays(1, &m_ScreenQuadVAO);
        glGenBuffers(1, &m_ScreenQuadVBO);
        glBindVertexArray(m_ScreenQuadVAO);
        glBindBuffer(GL_ARRAY_BUFFER, m_ScreenQuadVBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 16, (void*)0);
        glEnableVertexAttribArray(1);                   
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 16, (void*)8);
    }


    glBindVertexArray(m_ScreenQuadVAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);

}

GLuint Mesh::m_gPositionBuff = 0;
GLuint Mesh::m_gNormalBuff = 0;
GLuint Mesh::m_gColorSpec = 0;

void Mesh::SetupDeferredRender()
{
    GLint m_viewport[4];
    glGetIntegerv(GL_VIEWPORT, m_viewport);
    ivec2 WindowSize = ivec2(m_viewport[2], m_viewport[3]);
        
    glViewport(0, 0, (GLsizei)WindowSize.x, (GLsizei)WindowSize.y);

    if (m_gPositionBuff) // if we've already defined these textures, we only need to update their sizes to fit the window.
    {
        glBindTexture(GL_TEXTURE_2D, m_gPositionBuff);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, WindowSize.x, WindowSize.y, 0, GL_RGBA, GL_FLOAT, NULL);

        glBindTexture(GL_TEXTURE_2D, m_gNormalBuff);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8_SNORM, WindowSize.x, WindowSize.y, 0, GL_RGBA, GL_FLOAT, NULL);

        glBindTexture(GL_TEXTURE_2D, m_gColorSpec);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, WindowSize.x, WindowSize.y, 0, GL_RGBA, GL_FLOAT, NULL);

        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, WindowSize.x, WindowSize.y);

        return;
    }

    // - Position data 
    glGenTextures(1, &m_gPositionBuff);
    glBindTexture(GL_TEXTURE_2D, m_gPositionBuff);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, WindowSize.x, WindowSize.y, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_gPositionBuff, 0);
    
    glBindTexture(GL_TEXTURE_2D, 0);

    // - Normal data 
    glGenTextures(1, &m_gNormalBuff);
    glBindTexture(GL_TEXTURE_2D, m_gNormalBuff);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8_SNORM, WindowSize.x, WindowSize.y, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, m_gNormalBuff, 0);

    glBindTexture(GL_TEXTURE_2D, 0);

    // - Color & Spec data 
    glGenTextures(1, &m_gColorSpec);
    glBindTexture(GL_TEXTURE_2D, m_gColorSpec);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, WindowSize.x, WindowSize.y, 0, GL_RGBA, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, m_gColorSpec, 0);

    glBindTexture(GL_TEXTURE_2D, 0);

    // - Set up which color attachments we'll use on this FBO for rendering 
    const GLenum buffers[] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2};
    glDrawBuffers(3, buffers);
    
    unsigned int renderDepthBuffer;
    glGenRenderbuffers(1, &renderDepthBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, renderDepthBuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, WindowSize.x, WindowSize.y);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, renderDepthBuffer);

    glActiveTexture(GL_TEXTURE0);
}

void Mesh::ClearDeferredRenderTextures()
{
    glDeleteTextures(1, &m_gColorSpec);
    m_gColorSpec = 0;

    glDeleteTextures(1, &m_gNormalBuff);
    m_gNormalBuff = 0;

    glDeleteTextures(1, &m_gPositionBuff);
    m_gPositionBuff = 0;
}

} // namespace fw
