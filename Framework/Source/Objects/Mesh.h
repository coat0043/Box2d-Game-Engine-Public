#pragma once

#include "Math/Vector.h"
#include "Utility/Color.h"

namespace fw {

class CameraComponent;
class Material;
class MyMatrix;
class ShaderProgram;
class Texture;
class TransformComponent;
class Component;

struct VertexFormat
{
    vec3 pos;
    unsigned char color[4];
    vec2 uv;
    vec3 normal;

    VertexFormat(float nx, float ny, float nz, float nu, float nv)
    {
        pos.x = nx;
        pos.y = ny;
        pos.z = nz;
        color[0] = 1;
        color[1] = 1;
        color[2] = 1;
        color[3] = 1;
        uv.x = nu;
        uv.y = nv;
        normal.Set( 0, 1, 0 );
    }

    VertexFormat(float nx, float ny, float nz, unsigned char nr, unsigned char ng, unsigned char nb, unsigned char na)
    {
        pos.x = nx;
        pos.y = ny;
        pos.z = nz;
        color[0] = nr;
        color[1] = ng;
        color[2] = nb;
        color[3] = na;
        uv.x = 0;
        uv.y = 0;
        normal.Set( 0, 1, 0 );
    }

    VertexFormat(vec3 npos, vec2 nuv)
    {
        pos = npos;
        color[0] = 1;
        color[1] = 1;
        color[2] = 1;
        color[3] = 1;
        uv = nuv;
        normal.Set( 0, 1, 0 );
    }

    VertexFormat(vec3 npos, vec2 nuv, vec3 nnormal)
    {
        pos = npos;
        color[0] = 1;
        color[1] = 1;
        color[2] = 1;
        color[3] = 1;
        uv = nuv;
        normal = nnormal;
    }
};

class Mesh
{
public:
    Mesh();
    // Creates a mesh from a vertex array.
    Mesh(int primitiveType, int numVertices, const VertexFormat* pVertices);
    // Creates a cube.
    Mesh(vec3 cubeSize, bool facesInward);
    // Creates a plane.
    Mesh(vec2 worldSize, ivec2 vertCount);
    Mesh(const char* filename);
    virtual ~Mesh();

    void Start(int primitiveType);
    void AddVertex(const VertexFormat nVert);
    void AddVertex(const vec3 pos, const vec2 uv);
    void AddVertex(const vec3 pos, unsigned char r, unsigned char g, unsigned char b, unsigned char a);
    void AddVertex(const vec3 pos, const float u, const float v);
    void AddVertex(const float x, const float y, const float z, const float u, const float v);
    void AddVertex(const float x, const float y, const float z, const vec2 uv);
    void AddSprite(vec3 pos, vec2 size);
    void End();

    void CreateShape(int primitiveType, int numVertices, const VertexFormat* pVertices);
    void CreateShape(int primitiveType, int numVertices, const VertexFormat* pVertices, int numIndices, const unsigned int* pIndices);
    void CreateCube(vec3 size, bool facesInward);
    void CreatePlane(vec2 worldSize, ivec2 vertCount);
    void CreateFromOBJ(const char* filename);

    void SetUniform1f(ShaderProgram* pShader, char* name, float value);
    void SetUniform2f(ShaderProgram* pShader, char* name, vec2 value);
    void SetUniform3f(ShaderProgram* pShader, char* name, vec3 value);
    void SetUniform4f(ShaderProgram* pShader, char* name, vec4 value);
    void SetUniform1i(ShaderProgram* pShader, char* name, int value);

    void Draw(CameraComponent* pCamera, MyMatrix* pWorldMatrix, Material* pMaterial, std::vector<Component*>* pLights = nullptr);
    void Draw(CameraComponent* pCamera, TransformComponent* pTransform, Material* pMaterial, std::vector<Component*>* pLights = nullptr);
    void Draw(CameraComponent* pCamera, MyMatrix* pWorldMatrix, ShaderProgram* pShader, Texture* pTexture, Texture* pNormalTexture, Texture* pCubemap, Color color, vec2 UVScale, vec2 UVOffset, std::vector<Component*>* pLights);

    void DeferredRender_GeometryPass(CameraComponent* pCamera, TransformComponent* pTransform, Material* pMaterial);
    void DeferredRender_LightingPass(CameraComponent* pCamera, std::vector<Component*>* pLights, ShaderProgram* pLightingShader);

    void DrawScreenQuad(ShaderProgram* pShader);

    void SetupDeferredRender();

    static void ClearDeferredRenderTextures();

protected:

    static GLuint m_gPositionBuff, m_gNormalBuff, m_gColorSpec;
    
    GLuint m_IBO = 0;
    GLuint m_VBO = 0;

    unsigned int m_ScreenQuadVAO = 0;
    unsigned int m_ScreenQuadVBO = 0;

    std::vector<VertexFormat> m_Verts;

    int m_NumVertices = 0;
    int m_NumIndices = 0;
    int m_PrimitiveType = GL_POINTS;

    bool m_Editable = false;
};

} // namespace fw
