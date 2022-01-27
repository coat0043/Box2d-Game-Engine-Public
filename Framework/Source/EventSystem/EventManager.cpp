#include "FrameworkPCH.h"

#include "EventManager.h"
#include "EventListener.h"
#include "Event.h"
#include "Core/GameCore.h"
#include "Objects/GameObject.h"

namespace fw {

EventManager::EventManager()
{
}

EventManager::~EventManager()
{
    while (m_EventQueue.empty() == false)
    {
        delete m_EventQueue.front();
        m_EventQueue.pop();
    }
}

void EventManager::AddEvent(Event* pEvent, float delayBeforeSending)
{
    pEvent->SetDelay(delayBeforeSending);
    m_EventQueue.push(pEvent);
}

void EventManager::DispatchAllEvents(float deltaTime, GameCore* pGameCore)
{
    std::queue<Event*> m_NotSentEventQueue;

    while (m_EventQueue.empty() == false)
    {
        // Remove it from the queue.
        Event* pEvent = m_EventQueue.front();
        m_EventQueue.pop();

        pEvent->m_Delay -= deltaTime;
        if (pEvent->m_Delay <= 0)
        {
            // Send the event to every listener listening for its type.
            for (EventListener* listener : m_EventListeners[pEvent->GetType()])
            {
                listener->OnEvent(pEvent);
            }

            // Delete the event.
            delete pEvent;
        }
        else
        {
            // Temporarily put the event into another queue.
            m_NotSentEventQueue.push(pEvent);
        }
    }

    // Push all untriggered events back into the event queue.
    while (m_NotSentEventQueue.empty() == false)
    {
        m_EventQueue.push(m_NotSentEventQueue.front());
        m_NotSentEventQueue.pop();
    }
}

void EventManager::RegisterEventListener(EventListener* pListener, std::string eventType)
{
    for (int i = 0; i < m_EventListeners[eventType].size(); i++)
    {
        /// listener already registered. Ignore call.
        if (m_EventListeners[eventType][i] == pListener)
            return;
    }
    // Add the requested listener.
    m_EventListeners[eventType].push_back(pListener);
}

void EventManager::RegisterEventListener(EventListener& refListener, std::string eventType)
{
    for (int i = 0; i < m_EventListeners[eventType].size(); i++)
    {
        /// listener already registered. Ignore call.
        if (m_EventListeners[eventType][i] == &refListener)
            return;
    }
    // Add the requested listener.

    m_EventListeners[eventType].push_back(&refListener);
}

void EventManager::UnregisterEventListener(EventListener* pListener, std::string eventType)
{
    for (int i = 0; i < m_EventListeners[eventType].size(); i++)
    {
        if (m_EventListeners[eventType][i] == pListener)
        {
            m_EventListeners[eventType].erase(m_EventListeners[eventType].begin() + i);
            return;
        }
    }

    // listener never existed, nothing happens.
    if (dynamic_cast<GameObject*>(pListener))
    {
        GameObject* pObject = dynamic_cast<GameObject*>(pListener);
        LOG(WARNING, "GameObject \"%s\" tried to unregister for %s but wasn't registered.",
            pObject->GetName().c_str(), eventType.c_str());
    }
    else
    {
        LOG(WARNING, "Something tried to unregister for %s but wasn't registered.", eventType.c_str());
    }
}

void EventManager::UnregisterEventListenerCompletely(EventListener* pListener)
{
    // loop through every element of the map and remove the specified listener.
    for (auto& it : m_EventListeners)
    {
        for (size_t i = 0; i < it.second.size(); i++)
        {
            if (it.second[i] == pListener)
            {
                it.second.erase(it.second.begin() + i);
                break;
            }
        }
    }
}

bool EventManager::AreAnyListenersRegistered()
{
    for (auto& it : m_EventListeners)
    {
        if( it.second.size() > 0 )
            return true;
    }

    return false;
}

} // namespace fw