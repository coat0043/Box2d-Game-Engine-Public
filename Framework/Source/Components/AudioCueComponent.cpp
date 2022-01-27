#include "FrameworkPCH.h"

#include "AudioCueComponent.h"
#include "Audio/AudioEngine.h"
#include "Audio/Channel.h"
#include "Core/GameCore.h"
#include "Scene/Scene.h"
#include "Objects/GameObject.h"
#include "UI/ResourcesPanel.h"
#include "Objects/ResourceManager.h"

void fw::AudioCueComponent::Update(float deltaTime)
{
    if (m_ActiveSound)
    {
        AudioEngine* pAudioEngine = m_pGameObject->GetScene()->GetGameCore()->GetAudioEngine();
        std::vector<Channel*> pOwningChannels = pAudioEngine->GetArrayofOwningChannels(m_ActiveSound);
        for (auto& it : pOwningChannels) 
        {
            if (it->IsPlaying()) 
            {
                return;  
            }
        }

        if (m_AudioCueMode == AudioCueMode::Random)
        {
            Sound* RandomSound = nullptr;
            do {  RandomSound = m_OpenSounds[RandomInt(0, (int)m_OpenSounds.size() - 1)];  }
            while (RandomSound != m_ActiveSound);
            m_ActiveSound = RandomSound;
            pAudioEngine->PlaySFX(m_ActiveSound);
        }
        else if (m_AudioCueMode == AudioCueMode::Shuffle)
        {
            if (m_CurrentIndex < m_OpenSounds.size())
            {
                 m_ActiveSound = m_OpenSounds[m_CurrentIndex];
                 m_CurrentIndex++;
                 pAudioEngine->PlaySFX(m_ActiveSound);
            }
            else
            {
                std::shuffle(std::begin(m_OpenSounds), std::end(m_OpenSounds), m_RNG);
                m_CurrentIndex = 0;      
            }  
        }
        else if (m_AudioCueMode == AudioCueMode::Weighted)
        {
            m_ActiveSound->SetAmountNotPicked(0);
            m_ActiveSound->SetWeight(0.0f);
            for (int i = 0; i < m_OpenSounds.size(); i++)
            {
                if (m_OpenSounds[i]->GetWeight() >= m_ActiveSound->GetWeight()) 
                {
                    m_ActiveSound = m_OpenSounds[i];
                    pAudioEngine->PlaySFX(m_ActiveSound);
                }
                
            }

            for (int i = 0; i < m_OpenSounds.size(); i++)
            {
                if (m_ActiveSound != m_OpenSounds[i])
                {
                    m_OpenSounds[i]->SetAmountNotPicked(m_OpenSounds[i]->GetAmountNotPicked() + 1);
                    m_OpenSounds[i]->SetWeight(m_OpenSounds[i]->GetWeight() + 2.5f * m_OpenSounds[i]->GetAmountNotPicked());
                }
            }
        }
        
        
    }
}

void fw::AudioCueComponent::PlayAudioCueComponent()
{
    m_OpenSounds.clear();
    for (int i = 0; i < m_pSoundCue.size(); i++)
    {
        m_OpenSounds.push_back(m_pSoundCue[i]);
        m_pSoundCue[i]->SetWeight(0.0f);
        m_pSoundCue[i]->SetAmountNotPicked(0);
    }

    if (m_ActiveSound) 
    {
          AudioEngine* pAudioEngine = m_pGameObject->GetScene()->GetGameCore()->GetAudioEngine();
          std::vector<Channel*> pOwningChannels = pAudioEngine->GetArrayofOwningChannels(m_ActiveSound);
          for (auto& it : pOwningChannels) 
          {
              it->Stop();
          }
    }

    if (m_AudioCueMode == AudioCueMode::Random)
    {
        std::shuffle(std::begin(m_OpenSounds), std::end(m_OpenSounds), m_RNG);
        m_ActiveSound = m_OpenSounds[RandomInt(0, (int)m_OpenSounds.size() - 1)];
    }
    else if (m_AudioCueMode == AudioCueMode::Shuffle)
    {
        std::shuffle(std::begin(m_OpenSounds), std::end(m_OpenSounds), m_RNG);
        m_ActiveSound = m_OpenSounds[0];
    }
    else if (m_AudioCueMode == AudioCueMode::Weighted)
    {
        std::shuffle(std::begin(m_OpenSounds), std::end(m_OpenSounds), m_RNG);
        m_ActiveSound = m_OpenSounds[RandomInt(0, (int)m_OpenSounds.size() - 1)];

        for (int i = 0; i < m_OpenSounds.size(); i++)
        {
            if (m_ActiveSound != m_OpenSounds[i])
            {
                m_OpenSounds[i]->SetAmountNotPicked(m_OpenSounds[i]->GetAmountNotPicked() + 1);
                m_OpenSounds[i]->SetWeight(m_OpenSounds[i]->GetWeight() + 2.5f * i);
            }
        }
    }
}

void fw::AudioCueComponent::StopAudioCueComponent()
{
    if (m_ActiveSound)
    {
        AudioEngine* pAudioEngine = m_pGameObject->GetScene()->GetGameCore()->GetAudioEngine();
        std::vector<Channel*> pOwningChannels = pAudioEngine->GetArrayofOwningChannels(m_ActiveSound);
        for (auto& it : pOwningChannels) 
        {
          it->Stop();
        }
        m_ActiveSound = nullptr;
    }
    m_OpenSounds.clear();
}

void fw::AudioCueComponent::Save(WriterType& writer)
{
}

void fw::AudioCueComponent::Load(rapidjson::Value& component)
{
}

void fw::AudioCueComponent::AddToInspector()
{
    ImGui::PushID(this);

    if (m_pSoundCue.size() > 0) 
    {
        if (ImGui::Button("Play", ImVec2(2,2)))
        {
             PlayAudioCueComponent();
        }
        ImGui::SameLine();
        if (ImGui::Button("Stop", ImVec2(2,2)))
        {
             StopAudioCueComponent();
        }
    }
    for (int i = 0; i < m_pSoundCue.size(); i++) 
    {
        ResourcesPanel::DropNodeSound(&m_pSoundCue[i], m_pGameObject);
    }
    if (ImGui::Button("Add", ImVec2(2,2)))
    {
        m_pSoundCue.push_back(m_pGameObject->GetScene()->GetGameCore()->GetResourceManager()->GetSound("MarcusAngry1"));
    }
    ImGui::SameLine();
    if (ImGui::Button("Remove", ImVec2(2,2)))
    {
        m_pSoundCue.pop_back();
    }
  

    ImGui::PopID();
}


