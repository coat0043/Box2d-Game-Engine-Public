#pragma once

class fw::PhysicsWorld;
class fw::PhysicsBody;
class Game;

class GameScene : public fw::Scene
{
public:
    GameScene(fw::GameCore* pGameCore);
    virtual ~GameScene();

    virtual void Init() override;
    virtual void StartFrame(float deltaTime) override;
    virtual void OnEvent(fw::Event* pEvent) override;
    virtual void Update(float deltaTime) override;
    virtual void Draw() override;

    void UpdateExampleComponents(float deltaTime);
    void UpdateDirectMovementComponents(float deltaTime);
    void UpdateMovement3DComponents(float deltaTime);
    void UpdateOrbitComponents(float deltaTime);
    void UpdatePhysicsMovementComponents(float deltaTime);
    void UpdateAIFollowComponents(float deltaTime);
    
    Game* GetGame();

    void SetName(std::string NewName);

protected:
    virtual void DrawNewObjectButtons() override;
};
