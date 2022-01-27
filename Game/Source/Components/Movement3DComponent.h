#pragma once

class PlayerController;

class Movement3DComponent : public fw::Component
{
public:
    Movement3DComponent();
    virtual ~Movement3DComponent();

    static const char* GetStaticType() { return "Movement3DComponent"; }
    virtual const char* GetType() override { return GetStaticType(); }
    static fw::Component* Create() { return new Movement3DComponent(); }

    virtual void Save(fw::WriterType& writer) override;
    virtual void Load(rapidjson::Value& component) override;
    virtual void AddToInspector() override;

    virtual void Init() override;
    virtual void Update(float deltaTime) override;

    void SetPlayerController(PlayerController* pController);

protected:
    float m_Speed = 10;
    float m_TurnSpeed = 180.0f;

    vec2 m_DirectInput;
    PlayerController* m_pPlayerController;
};

