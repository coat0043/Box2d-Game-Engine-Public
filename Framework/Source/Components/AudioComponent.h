#pragma once

#include "Component.h"

namespace fw {

class Sound;

class AudioComponent : public Component
{
public:
    AudioComponent() {}
    AudioComponent(Sound* pAudio) { m_pSound = pAudio; }

    static const char* GetStaticType() { return "AudioComponent"; }
    virtual const char* GetType() override { return GetStaticType(); }
    static Component* Create() { return new AudioComponent(); }
    virtual void Save(WriterType& writer) override;
    virtual void Load(rapidjson::Value& component) override;
    virtual void AddToInspector() override;

    void PlayAudioComponent();
    void StopAudioComponent();

    // Getters.
    Sound* GetSound() { return m_pSound; }

    // Setters.
    void SetSound(Sound* pSound) { m_pSound = pSound; }


protected:
    Sound* m_pSound = nullptr;
};

} // namespace fw