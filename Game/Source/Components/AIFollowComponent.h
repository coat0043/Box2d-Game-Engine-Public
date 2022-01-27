#pragma once

class AIFollowComponent : public fw::Component
{
public:
    AIFollowComponent();
    virtual ~AIFollowComponent();

    static const char* GetStaticType() { return "AIFollowComponent"; }
    virtual const char* GetType() override { return GetStaticType(); }
    static fw::Component* Create() { return new AIFollowComponent(); }

    virtual void Save(fw::WriterType& writer) override;
    virtual void Load(rapidjson::Value& component) override;
    virtual void AddToInspector() override;

    virtual void Init() override;
    virtual void Update(float deltaTime) override;

    void SetObjectToFollow(fw::GameObject* pGameObject);

protected:
    float m_Speed = 5;
    vec3 m_Direction;
    fw::GameObject* m_pTarget;
};
