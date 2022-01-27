#include "FrameworkPCH.h"

#if FW_USING_LUA
#include "LuaGameState.h"
#include "Objects/GameObject.h"
#include "Math/Vector.h"
#include "Math/MyMatrix.h"
#include "Physics/PhysicsBody.h"
#include "Physics/PhysicsWorld.h"
#include "EventSystem/EventManager.h"
#include "EventSystem/EventListener.h"
#include "Scene/Scene.h"
#include "Core/GameCore.h"
#include "EventSystem/Event.h"

using namespace luabridge;

namespace fw
{

void LogInfo(const char* log)
{
    LOG(INFO, "%s", log);
}

LuaGameState::LuaGameState()
{
    m_pLuaState = luaL_newstate();
    luaL_openlibs(m_pLuaState);

    RegisterClasses();
}

LuaGameState::~LuaGameState()
{
    if (m_pLuaState != 0)
    {
        lua_close(m_pLuaState);
    }
}

void LuaGameState::InitGameObjectFromLua(GameObject* go, const char* funcName)
{
    LuaRef InitFunction = getGlobal(m_pLuaState, funcName);
    if(InitFunction)
    {
        InitFunction(go);
    }
}

void LuaGameState::UpdateGameObjectFromLua(GameObject* go, float delta, const char* funcName)
{
    LuaRef UpdateFunction = getGlobal(m_pLuaState, funcName);
    if (UpdateFunction)
    {
        UpdateFunction(go, delta);
    }
}

void LuaGameState::OnInputEvent(Event* pEvent, GameObject* go,  int deviceType, int deviceState, int key, bool canJump, const char* funcName)
{
    LuaRef InputEventFunc = getGlobal(m_pLuaState, funcName);
    if (InputEventFunc)
    {
        InputEventFunc(go, deviceType, deviceState, key, canJump);
    }
}

void LuaGameState::RegisterClasses()
{
    // Global Functions
    getGlobalNamespace(m_pLuaState)
        .addFunction("Log", LogInfo);

    luabridge::getGlobalNamespace(m_pLuaState)
        .beginClass<MyMatrix>("MyMatrix")
        .addConstructor<void(*) ()>() // MyMatrix()
        .addData("m11", &MyMatrix::m11) // float
        .addData("m12", &MyMatrix::m12) // float
        .addData("m13", &MyMatrix::m13) // float
        .addData("m14", &MyMatrix::m24) // float
        .addData("m21", &MyMatrix::m21) // float
        .addData("m22", &MyMatrix::m22) // float
        .addData("m23", &MyMatrix::m23) // float
        .addData("m24", &MyMatrix::m34) // float
        .addData("m31", &MyMatrix::m31) // float
        .addData("m32", &MyMatrix::m32) // float
        .addData("m33", &MyMatrix::m33) // float
        .addData("m34", &MyMatrix::m44) // float
        .addData("m41", &MyMatrix::m41) // float
        .addData("m42", &MyMatrix::m42) // float
        .addData("m43", &MyMatrix::m43) // float
        .addData("m44", &MyMatrix::m44) // float
        .addFunction("CreateLookAtView", &MyMatrix::CreateLookAtView) // void MyMatrix::CreateLookAtView(const Vector3 &eye, const Vector3 &up, const Vector3 &at)
        .addFunction("CreateLookAtWorld", &MyMatrix::CreateLookAtWorld) // void MyMatrix::CreateLookAtWorld(const Vector3& objpos, const Vector3& up, const Vector3& at)
        .addFunction("Scale", (void (MyMatrix::*)(float scale)) & MyMatrix::Scale) // void MyMatrix::Scale(float scale);
        .addFunction("Rotate", (void (MyMatrix::*)(float angle, float x, float y, float z)) & MyMatrix::Rotate) // void MyMatrix::Rotate(float angle, float x, float y, float z);
        .addFunction("Translate", (void (MyMatrix::*)(vec3 pos)) & MyMatrix::Translate) // void MyMatrix::Translate(Vector3 pos);
        .addFunction("SetIdentity", &MyMatrix::SetIdentity) // void MyMatrix::SetIdentity();
        .addFunction("CreateSRT", (void (MyMatrix::*)(vec3 scale, vec3 rot, vec3 pos)) & MyMatrix::CreateSRT) // void MyMatrix::CreateSRT(float scale, Vector3 rot, Vector3 pos);
        .addFunction("Multiply", (MyMatrix(MyMatrix::*)(const MyMatrix o) const) & MyMatrix::operator*) // MyMatrix MyMatrix::operator *(const MyMatrix o) const
        .addFunction("CopyFrom", (MyMatrix & (MyMatrix::*)(const MyMatrix & o)) & MyMatrix::operator=) // MyMatrix& MyMatrix::operator =(const MyMatrix& o)
        .endClass();

    getGlobalNamespace(m_pLuaState)
        .beginClass<vec3>("vec3")
        .addConstructor<void(*) (float x, float y, float z)>() // Vector3(float nx, float ny, float nz)
        .addData("x", &vec3::x) // float
        .addData("y", &vec3::y) // float
        .addData("z", &vec3::z) // float
        .addFunction("Set", &vec3::Set) // void Vector3::Set(float nx, float ny, float nz)
        .addFunction("Add", &vec3::Add) // Vector3 Vector3::Add(const Vector3& o) const
        .addFunction("Sub", &vec3::Sub) // Vector3 Vector3::Sub(const Vector3& o) const
        .addFunction("Scale", &vec3::Scale) // Vector3 Vector3::Scale(const float o) const
        .addFunction("Dot", &vec3::Dot) // float Vector3::Dot(const Vector3 &o) const
        .addFunction("Cross", &vec3::Cross) // Vector3 Vector3::Cross(const Vector3& o) const
        .addFunction("Length", &vec3::Length) // float Vector3::Length() const
        .addFunction("LengthSquared", &vec3::LengthSquared) // float Vector3::LengthSquared() const
        .addFunction("Normalize", &vec3::Normalize) // Vector3 Vector3::Normalize()
        .endClass();

    getGlobalNamespace(m_pLuaState)
        .beginClass<vec2>("vec2")
        .addConstructor<void(*) (float x, float y)>() // Vector2(float nx, float ny)
        .addData("x", &vec2::x) // float
        .addData("y", &vec2::y) // float
        .addFunction("Set", &vec2::Set) // void Vector2::Set(float nx, float ny)
        .addFunction("Add", &vec2::Add) // Vector2 Vector2::Add(const Vector2& o) const
        .addFunction("Sub", &vec2::Sub) // Vector2 Vector2::Sub(const Vector2& o) const
        .addFunction("Scale", &vec2::Scale) // Vector2 Vector2::Scale(const float o) const
        .addFunction("Dot", &vec2::Dot) // float Vector2::Dot(const Vector2 &o) const
        .addFunction("Length", &vec2::Length) // float Vector2::Length()
        .addFunction("LengthSquared", &vec2::LengthSquared) // float Vector2::LengthSquared()
        .addFunction("Normalize", &vec2::Normalize) // Vector2 Vector2::Normalize()
        .endClass();

    getGlobalNamespace(m_pLuaState)
        .beginClass<Scene>("Scene")
        .addFunction("GetGameCore", &Scene::GetGameCore)
        .endClass();

    GameObject::LuaRegister(m_pLuaState);

    getGlobalNamespace(m_pLuaState)
        .beginClass<GameObject>("GameObject")
        //.addFunction("GetPosition", &GameObject::GetPosition)
        //.addFunction("SetPosition", (void (GameObject::*)(vec2 position)) &GameObject::SetPosition)
        //.addFunction("SetPositionFloat", (void (GameObject::*)(float x, float y)) &GameObject::SetPosition)
        //.addFunction("GetRotation",  &GameObject::GetRotation)
        .addFunction("GetScene", &GameObject::GetScene)
        //.addFunction("GetPhysicsBody", &GameObject::GetPhysicsBody)
        //.addFunction("GetMaterial", &GameObject::GetMaterial)
        .endClass();

    getGlobalNamespace(m_pLuaState)
        .beginClass<PhysicsBody>("PhysicsBody")
        .addFunction("SetPosition", &PhysicsBody::SetPosition)
        .addFunction("GetPosition", &PhysicsBody::GetPosition)
        .addFunction("ApplyForceToCenter", &PhysicsBody::ApplyForceToCenter)
        .addFunction("ApplyImpulseToCenter", &PhysicsBody::ApplyImpulseToCenter)
        .addFunction("CreateCircle", &PhysicsBody::CreateCircle)
        .addFunction("CreateBox", &PhysicsBody::CreateBox)
        .endClass();

    getGlobalNamespace(m_pLuaState)
        .beginClass<GameCore>("GameCore")
        .addFunction("GetEventManager", &GameCore::GetEventManager)
        .endClass();

    getGlobalNamespace(m_pLuaState)
        .beginClass<EventListener>("EventListener")
        .endClass();

    getGlobalNamespace(m_pLuaState)
        .beginClass<PhysicsWorld>("PhysicsWorld")
        .endClass();

    // TODO: Move this to LuaScript
    luaL_dofile(m_pLuaState, "Data/Scripts/Player.lua");
}

}

#endif