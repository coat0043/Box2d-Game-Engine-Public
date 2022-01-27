#pragma once

class PlayerController;

class DirectMovementComponent : public fw::Component
{
public:
    DirectMovementComponent();
    virtual ~DirectMovementComponent();

    static const char* GetStaticType() { return "DirectMovementComponent"; }
    virtual const char* GetType() override { return GetStaticType(); }
    static fw::Component* Create() { return new DirectMovementComponent(); }

    virtual void Save(fw::WriterType& writer) override;
    virtual void Load(rapidjson::Value& component) override;
    virtual void AddToInspector() override;

    virtual void Init() override;
    virtual void Update(float deltaTime) override;

    void SetPlayerController(PlayerController* pController);

protected:
    float m_Speed = 10;
    vec2 m_DirectInput;
    PlayerController* m_pPlayerController;
};

