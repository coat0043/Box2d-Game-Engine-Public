#pragma once

class PlayerController;

class OrbitComponent : public fw::Component
{
public:
    OrbitComponent();
    virtual ~OrbitComponent();

    static const char* GetStaticType() { return "OrbitComponent"; }
    virtual const char* GetType() override { return GetStaticType(); }
    static fw::Component* Create() { return new OrbitComponent(); }

    virtual void Save(fw::WriterType& writer) override;
    virtual void Load(rapidjson::Value& component) override;
    virtual void FinalizeLoad(rapidjson::Value& component) override;
    virtual void AddToInspector() override;

    virtual void Init() override;
    virtual void Update(float deltaTime) override;

    void SetPlayerController(PlayerController* pController);

protected:
    fw::GameObject* m_pObjectFollowing = nullptr;
    float m_Distance = 10;
    vec3 m_Offset = vec3(0);

    PlayerController* m_pPlayerController;
};

