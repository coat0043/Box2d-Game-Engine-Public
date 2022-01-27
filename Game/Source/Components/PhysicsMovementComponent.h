#pragma once

class PlayerController;

class PhysicsMovementComponent : public fw::Component
{
public:
    PhysicsMovementComponent();
    virtual ~PhysicsMovementComponent();

    static const char* GetStaticType() { return "PhysicsMovementComponent"; }
    virtual const char* GetType() override { return GetStaticType(); }
    static fw::Component* Create() { return new PhysicsMovementComponent(); }

    virtual void Save(fw::WriterType& writer) override;
    virtual void Load(rapidjson::Value& component) override;
    virtual void AddToInspector() override;

    virtual void Init() override;
    virtual void Update(float deltaTime) override;

    void SetPlayerController(PlayerController* pController);

    virtual void OnEvent(fw::Event* pEvent) override;
    bool CanJump();
    bool m_AllowedToJump = true;

protected:
    float m_Speed = 5;

    vec2 m_PhysicsInput;
    PlayerController* m_pPlayerController = nullptr;
    EventListener* m_pEventListener = nullptr;
};
