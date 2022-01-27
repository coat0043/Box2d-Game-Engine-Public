#pragma once

class ExampleComponent : public fw::Component
{
public:
    ExampleComponent() {}

    static const char* GetStaticType() { return "ExampleComponent"; }
    virtual const char* GetType() override { return GetStaticType(); }
    static fw::Component* Create() { return new ExampleComponent(); }

    virtual void Save(fw::WriterType& writer) override;
    virtual void Load(rapidjson::Value& component) override;
    virtual void AddToInspector() override;

    virtual void Update(float deltaTime) override;

    // Getters.

    // Setters.

protected:
};
