#pragma once

class PlayerController : public fw::EventListener
{
public:
    enum Mask
    {
        Up = 1,
        Down = 2,
        Left = 4,
        Right = 8,
    };

    PlayerController();
    virtual ~PlayerController();

    void StartFrame();
    virtual void OnEvent(fw::Event* pEvent) override;

    bool IsHeld(Mask mask);
    bool WasNewlyPressed(Mask mask);
    bool WasNewlyReleased(Mask mask);

    vec2 GetLeftStick() { return m_LeftStick; }

protected:
    unsigned int m_Flags = 0;
    unsigned int m_OldFlags = 0;

    vec2 m_LeftStick = vec2(0);
};
