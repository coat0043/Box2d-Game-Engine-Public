#include "FrameworkPCH.h"

#include "Box2dDebugDraw.h"
#include "Math/MyMatrix.h"
#include "Objects/Material.h"
#include "Objects/Mesh.h"
#include "Utility/ShaderProgram.h"

namespace fw
{

Box2dDebugDraw::Box2dDebugDraw()
{
    m_pMeshTriangles = new Mesh();
    m_pMeshLines = new Mesh();
    m_pMeshPoints = new Mesh();

    m_pShader = new ShaderProgram("Data/Shaders/DebugDraw.vert", "Data/Shaders/DebugDraw.frag");
    m_pMaterial = new Material(m_pShader, nullptr, fw::Color::Cyan(), 1, 0);
}

Box2dDebugDraw::~Box2dDebugDraw()
{
    delete m_pMeshTriangles;
    delete m_pMeshLines;
    delete m_pMeshPoints;

    delete m_pMaterial;
    delete m_pShader;
}

void Box2dDebugDraw::DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{
    unsigned char r = (unsigned char)(color.r * 255);
    unsigned char g = (unsigned char)(color.g * 255);
    unsigned char b = (unsigned char)(color.b * 255);
    unsigned char a = (unsigned char)(color.a * 255);

    for (int i = 0; i < vertexCount - 1; i++)
    {
        m_pMeshLines->AddVertex(vec3(vertices[i].x, vertices[i].y, 0), r, g, b, a);
        m_pMeshLines->AddVertex(vec3(vertices[i + 1].x, vertices[i + 1].y, 0), r, g, b, a);
    }

    m_pMeshLines->AddVertex(vec3(vertices[vertexCount - 1].x, vertices[vertexCount - 1].y, 0), r, g, b, a);
    m_pMeshLines->AddVertex(vec3(vertices[0].x, vertices[0].y, 0), r, g, b, a);
}

void Box2dDebugDraw::DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color)
{
    unsigned char r = (unsigned char)(color.r * 255);
    unsigned char g = (unsigned char)(color.g * 255);
    unsigned char b = (unsigned char)(color.b * 255);
    unsigned char a = (unsigned char)(color.a * 64);

    for (int i = 1; i < vertexCount - 1; i++)
    {
        m_pMeshTriangles->AddVertex(vec3(vertices[0].x, vertices[0].y, 0), r, g, b, a);
        m_pMeshTriangles->AddVertex(vec3(vertices[i + 1].x, vertices[i + 1].y, 0), r, g, b, a);
        m_pMeshTriangles->AddVertex(vec3(vertices[i].x, vertices[i].y, 0), r, g, b, a);
    }
}

void Box2dDebugDraw::DrawCircle(const b2Vec2& center, float radius, const b2Color& color)
{
    assert(m_pMaterial != nullptr);
}

void Box2dDebugDraw::DrawSolidCircle(const b2Vec2& center, float radius, const b2Vec2& axis, const b2Color& color)
{
    assert(m_pMaterial != nullptr);
}

void Box2dDebugDraw::DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color)
{
    unsigned char r = (unsigned char)(color.r * 255);
    unsigned char g = (unsigned char)(color.g * 255);
    unsigned char b = (unsigned char)(color.b * 255);
    unsigned char a = (unsigned char)(color.a * 255);

    m_pMeshLines->AddVertex(vec3(p1.x, p1.y, 0), r, g, b, a);
    m_pMeshLines->AddVertex(vec3(p2.x, p2.y, 0), r, g, b, a);
}

void Box2dDebugDraw::SetCamera(CameraComponent* camera, bool drawOnXZPlane)
{
    m_pCamera = camera;
    m_DrawOnXZPlane = drawOnXZPlane;
}

void Box2dDebugDraw::DrawTransform(const b2Transform& xf)
{
}

void Box2dDebugDraw::DrawPoint(const b2Vec2& p, float size, const b2Color& color)
{
    assert(m_pMaterial != nullptr);

    //m_pMaterial->SetColor(Color(color.r, color.g, color.b, color.a));

    m_pMeshPoints->AddVertex(p.x, p.y, 0, 0, 0);
}

void Box2dDebugDraw::StartMeshes()
{
    glPointSize(3.0f);
    glLineWidth(3.0f);

    m_pMeshTriangles->Start(GL_TRIANGLES);
    m_pMeshLines->Start(GL_LINES);
    m_pMeshPoints->Start(GL_POINTS);
}

void Box2dDebugDraw::EndMeshes()
{
    m_pMeshTriangles->End();
    m_pMeshLines->End();
    m_pMeshPoints->End();

    MyMatrix mat;
    mat.SetIdentity();

    if( m_DrawOnXZPlane )
    {
        mat.m22 = 0;
        mat.m23 = 1;
        mat.m32 = 1;
        mat.m33 = 0;
    }

    m_pMeshTriangles->Draw(m_pCamera, &mat, m_pMaterial);
    m_pMeshLines->Draw(m_pCamera, &mat, m_pMaterial);
    m_pMeshPoints->Draw(m_pCamera, &mat, m_pMaterial);
}

} // namespace fw
