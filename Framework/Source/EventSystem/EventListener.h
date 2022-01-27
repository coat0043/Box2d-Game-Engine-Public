#pragma once

namespace fw {

class Event;

class EventListener
{
public:
    EventListener() {};
    ~EventListener() {};

    virtual void OnEvent(Event* pEvent) {};
};

} // namespace fw
