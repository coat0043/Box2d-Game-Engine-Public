#pragma once

#include "Component.h"


namespace fw {

enum class AudioCueMode
{
    Shuffle,
    Random,
    Weighted

};

class Sound;

class AudioCueComponent : public Component
{
public:
    AudioCueComponent() { m_RNG = std::default_random_engine {}; m_AudioCueMode = AudioCueMode::Random; m_ActiveSound = nullptr; }
    AudioCueComponent(AudioCueMode AudioCueMode) { m_AudioCueMode = AudioCueMode; m_ActiveSound = nullptr; }
    static const char* GetStaticType() { return "AudioCueComponent"; }
    virtual const char* GetType() override { return GetStaticType(); }
    static Component* Create() { return new AudioCueComponent(); }
    virtual void Update(float deltaTime) override;
    virtual void Save(WriterType& writer) override;
    virtual void Load(rapidjson::Value& component) override;
    virtual void AddToInspector() override;

    void PlayAudioCueComponent();
    void StopAudioCueComponent();

    void AddSound(Sound* pSound) { m_pSoundCue.push_back(pSound); }
    void RemoveSound() { m_pSoundCue.pop_back(); }

    int RandomInt(int min, int max) { return min + rand()%(max - min + 1); }

    // Getters.
    std::vector<Sound*> GetSoundCue() { return m_pSoundCue; }

    // Setters.
    void SetSound(int Index, Sound* pSound) { m_pSoundCue[Index] = pSound; }


protected:
    AudioCueMode m_AudioCueMode;
    std::vector<Sound*> m_pSoundCue;
    std::vector<Sound*> m_OpenSounds;
    Sound* m_ActiveSound;

    std::default_random_engine m_RNG;
    int m_CurrentIndex;
};

} // namespace fw