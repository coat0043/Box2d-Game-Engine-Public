#pragma once

namespace fw {

class Event;
class EventListener;
class GameCore;

class EventManager
{
public:
    EventManager();
    ~EventManager();

    void AddEvent(Event* pEvent, float delayBeforeSending = 0.0f);
    void DispatchAllEvents(float deltaTime, GameCore* pGameCore);

    void RegisterEventListener(EventListener* pListener, std::string eventType);
    void RegisterEventListener(EventListener& refListener, std::string eventType);

    void UnregisterEventListener(EventListener* pListener, std::string eventType);
    void UnregisterEventListenerCompletely(EventListener* pListener);

    bool AreAnyListenersRegistered();

protected:
    std::queue<Event*> m_EventQueue;
    std::map<std::string, std::vector<EventListener*>> m_EventListeners;
};

} // namespace fw
