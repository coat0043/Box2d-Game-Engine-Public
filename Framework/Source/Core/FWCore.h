#pragma once

#include "EventSystem/Event.h"

namespace fw {

class MyGLContext;
class GameCore;

class FWCore
{
protected:
    bool m_EscapeKeyWillQuit;

    int m_WindowWidth;
    int m_WindowHeight;

    HWND m_hWnd;
    HGLRC m_hRenderingContext;
    HDC m_hDeviceContext;
    HINSTANCE m_hInstance;
    MyGLContext* m_pMyGLContext;

    GameCore* m_pGame;

    bool m_KeyStates[256];
    bool m_MouseButtonStates[3];
    bool m_OldKeyStates[256];
    bool m_OldMouseButtonStates[3];
    XINPUT_STATE m_OldXInputState;
    XINPUT_STATE m_XInputState;

    bool m_WindowIsActive;
    bool m_FullscreenMode;
    unsigned int m_KeyTyped[256];
    float m_MouseWheel;

protected:
    void ResizeWindow(int width, int height);
    bool CreateGLWindow(char* title, int width, int height, unsigned char colorBits, unsigned char alphaBits, unsigned char zBits, unsigned char stencilBits, unsigned char multisampleSize, bool fullscreenflag);
    bool FailAndCleanup(const char* pMessage);
    void KillGLWindow(bool destroyInstance);

    static LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

public:
    FWCore();
    virtual ~FWCore();

    bool Init(int width, int height);
    int Run(GameCore* pGame);
    void Shutdown();

    void SetWindowSize(int width, int height);
    HWND GetHWND();

    bool IsKeyDown(int value);
    bool IsMouseButtonDown(int id);
    bool WasMouseButtonDown(int id);
    void GetMouseCoordinates(int* mx, int* my);
    bool IsKeyTyped(int value);
    float GetMouseWheel();

    void XInput_SendEvents();
    bool XInput_WasButtonNewlyPressed(int mask);
    bool XInput_WasButtonNewlyReleased(int mask);
    void XInput_SendSingleButtonEvent(int xinputMask, InputEvent::ControllerButton buttonID);
    void XInput_SendLeftJoystickEvent();

    unsigned int GetWindowWidth() { return m_WindowWidth; }
    unsigned int GetWindowHeight() { return m_WindowHeight; }

    void SetEscapeKeyWillQuit(bool value) { m_EscapeKeyWillQuit = value; }

    void SwapBuffers();
};

} // namespace fw
