#pragma once

#include "Component.h"
#include "Math/Vector.h"
#include "Math/MyMatrix.h"
#include "Utility/JSONHelpers.h"

namespace fw
{

class CameraComponent : public Component
{
public:
    enum class ViewType
    {
        ORTHOGRAPHIC,
        PERSPECTIVE,
    };

    ViewType GetCameraType() { return m_CameraView; }
    void SetCameraType(ViewType view) { m_CameraView = view; }

public:
    CameraComponent();
    CameraComponent(vec2 projScale, ViewType type);
    ~CameraComponent();

    static const char* GetStaticType() { return "CameraComponent"; }
    virtual const char* GetType() override { return GetStaticType(); }
    static Component* Create() { return new CameraComponent(); }

    virtual void Init();
    virtual void Update(float deltaTime);
    virtual void Save(WriterType& writer) override;
    virtual void Load(rapidjson::Value& component) override;
    virtual void AddToInspector() override;

    void AdjustForAspect();

    // Getters
    MyMatrix* GetViewMatrix();
    MyMatrix* GetProjectionMatrix();

    vec2 GetProjectionScale() { return m_ProjectionScale; }
    vec3 GetRotation() { return m_Rotation; }
    vec3 GetPosition() { return m_Position; }

    vec2 GetNormalizedDeviceCoordinates();

    ivec2 GetWindowSize();

    // Setters
    void SetRotation(vec3 rotation) { m_Rotation = rotation; }
    void SetRotation(float x, float y, float z) { m_Rotation.x = x; m_Rotation.y = y; m_Rotation.z = z; }

    void SetPosition(vec3 position) { m_Position = position; }
    void SetPosition(float x, float y, float z) { m_Position.x = x; m_Position.y = y; m_Position.z = z; }

    void FirstPersonCamControls(float deltaTime);
    void OnEvent(Event* pEvent);

protected:
    ViewType m_CameraView = ViewType::ORTHOGRAPHIC;

    vec3 m_Position = vec3(0, 0, 0);
    vec3 m_Rotation = vec3(0, 0, 0);
    vec2 m_ProjectionScale = vec2(5, 5);
    vec2 m_RequestedProjectionScale = vec2(5, 5);

    MyMatrix m_ViewMatrix;
    MyMatrix m_ProjMatrix;

    vec3 m_MovementAxes = vec3(0); // x: left/right, y: up/down, z: forward/back
    vec2 m_RotationAxes = vec2(0); // x: Yaw, y: Pitch
};

} // namespace fw