#include "FrameworkPCH.h"
#include "AudioEngine.h"
#include "Channel.h"
#include "Math/MathHelpers.h"
#include "Objects/Sound.h"

fw::AudioEngine::AudioEngine()
{
    if (FAILED(hr = CoInitializeEx( nullptr, COINIT_MULTITHREADED )))
        assert(hr);

    if ( FAILED(hr = XAudio2Create( &m_XAudio2Engine, 0, XAUDIO2_DEFAULT_PROCESSOR)))
        assert(hr);

    if ( FAILED(hr = m_XAudio2Engine->CreateMasteringVoice( &m_XMasteringVoice)))
        assert(hr);

    m_XAudio2Engine->StartEngine();

    LOG(INFO, "Audio Engine Initialized...");
}

fw::AudioEngine::~AudioEngine()
{
    m_XAudio2Engine->StopEngine();
    for (Channel* pChannel : m_AvailableChannels)
    {
        pChannel->Stop();
        DestroySourceVoice(pChannel->GetSourceVoice());
        delete pChannel;
    }

    for (Channel* pChannel : m_ActiveChannels)
    {
        pChannel->Stop();
        DestroySourceVoice(pChannel->GetSourceVoice());
        delete pChannel;
    }
        //TODO
       

    m_XMasteringVoice->DestroyVoice();
	m_XAudio2Engine->Release();
}

void fw::AudioEngine::PlaySFX(Sound* pSound)
{
    Channel* pChannel = nullptr;
    //Check if any Channel is Available
    for (int i = 0; i < m_AvailableChannels.size(); i++)
    {
        pChannel = m_AvailableChannels[i];
        //Make Sure it is not playing
        if (pChannel->IsPlaying() == false)
        {
            //Setup MemberVariables
            pChannel->SwitchProperties(pSound);
            pChannel->SetBufferContextPointerToThis();

            //Move to ActiveChannel
            SwapToActiveChannel(pChannel);
            EraseFromAvailableChannel(i);

            //PlaySound
            pChannel->Play();
            return;
        }
        else
        {
            LOG(ERROR, "AudioEngine - Playing while located in the Available Channel: %d", m_AvailableChannels[i]->GetName());
        }
    }

    //SaveIndex for Lowest 
    int CurrentLowestImportanceIndex = -1;
    float CurrentLowestImportance = 100.0f;

    for (int i = 0; i < m_ActiveChannels.size(); i++)
    {
        pChannel = m_ActiveChannels[i];

        //Check if Channel Has a lower importants then Current and pSound
        if(pChannel->GetSound()->GetImportance() <= CurrentLowestImportance && pChannel->GetSound()->GetImportance() <= pSound->GetImportance())
        {
            CurrentLowestImportanceIndex = i;
            CurrentLowestImportance = pChannel->GetSound()->GetImportance();
        }
    }

    if (CurrentLowestImportanceIndex != -1)
    {
        pChannel = m_ActiveChannels[CurrentLowestImportanceIndex];

        //Stop the Sound
        pChannel->Stop();

        //Set up member variables
        pChannel->SetSound(pSound);
        pChannel->SetBufferContextPointerToThis();

        //Play Sound
        pChannel->Play();
        return;
    }
    else
    {
        LOG(WARNING, "AudioEngine - Unable to Find a ActiveChannel with a Lower Importance");
    }

}

void fw::AudioEngine::StopSFX(Sound* pSound)
{
    Channel* pChannel = nullptr;

    //Check if it is in ActiveChannels
    for (int i = 0; i < m_ActiveChannels.size(); i++)
    {
        pChannel = m_ActiveChannels[i];

        //Does the WaveDataMatch
        if (pChannel->GetSound() == pSound)
        {
            //Stop playing.
            pChannel->Stop();
            return;
        }
    }

    //Check if it is in AvailableChannels
    for (int i = 0; i < m_AvailableChannels.size(); i++)
    {
        pChannel = m_AvailableChannels[i];

        //Does the WaveDataMatch
        if (pChannel->GetSound() == pSound)
        {
            //Stop playing.
            pChannel->Stop();
            return;
        }
    }

    LOG(WARNING, "Called StopSFX() but cound not find any Channel that refs pSound");
}

void fw::AudioEngine::StopAllSounds()
{
    //Check the ActiveChannels
    for (int i = 0; i < m_ActiveChannels.size(); i++)
    {
        //Stop Playing
        if (m_ActiveChannels[i]->IsPlaying())
        {
            m_ActiveChannels[i]->Stop();
        }
    }

    //SHOULD NOT NEED but just in case also check if AvaibleChannels are running
    for (int i = 0; i < m_AvailableChannels.size(); i++)
    {
        //Stop Playing
        if (m_AvailableChannels[i]->IsPlaying() == false)
        {
            m_AvailableChannels[i]->Stop();
        }
        else
        {
            LOG(WARNING, "Audio Engine - Playing while in AvailableChannels >> %s", m_AvailableChannels[i]->GetName());
        }
    }
}



void fw::AudioEngine::CreateSourceVoice(IXAudio2SourceVoice** source, WAVEFORMATEX* waveFormat, VoiceCallback* callback)
{
    if (FAILED(hr = m_XAudio2Engine->CreateSourceVoice(source, waveFormat, 0, XAUDIO2_DEFAULT_FREQ_RATIO,  callback, nullptr, nullptr)))
       assert(hr);
 
}

void fw::AudioEngine::DestroySourceVoice(IXAudio2SourceVoice* source)
{
    if (source != nullptr)
    {
        source->DestroyVoice();
        source = nullptr;
    }
}

void fw::AudioEngine::SetMasterVolume(float volume)
{
    m_XMasteringVoice->SetVolume(MyClamp_Return(volume, 0.0f, 1.0f));
}

float fw::AudioEngine::GetMasterVolume()
{
    float volume = 0.0f;
	m_XMasteringVoice->GetVolume(&volume);
	return volume;   
}


////////////////////////////////////////////////////////////
///Channel Functions

void fw::AudioEngine::Update(float deltaTime)
{
    //Loop through Active Channels
    for (int i = 0; i < m_ActiveChannels.size(); i++)
    {
        //If any have finished playing
        if (m_ActiveChannels[i]->IsPlaying() == false)
        {
            //Move it to Available Channel
            SwapToAvailableChannel(m_ActiveChannels[i]);

            //Remove it from Active Channel
            EraseFromActiveChannel(i);
        }
    }
}


void fw::AudioEngine::AddChannel(const char* ChannelName, Sound* pSound)
{
    //Setup Channel
    Channel* pNewChannel = new Channel(ChannelName);
    pNewChannel->SetSound(pSound);
    pNewChannel->SetChannelWaveFormat(&pSound->GetWaveData()->waveFormat);
    pNewChannel->SetBufferContextPointerToThis();

    //Added it to AvailableChannels
    m_AvailableChannels.push_back(pNewChannel);

    CreateSourceVoice(&pNewChannel->m_Source, &pNewChannel->m_ChannelWaveFormat, pNewChannel->GetCallBack());

    //Check if SourceVoice failed
    if (pNewChannel->m_Source == nullptr)
    {
        assert( "SouceVoice =" && nullptr);
    }
}

void fw::AudioEngine::RemoveChannel(Channel* pRemoveChannel)
{
    if (pRemoveChannel != nullptr)
        return;

    int ChannelIndex = FindChannel(pRemoveChannel, true);
    //In the ActiveChannelList
    if (ChannelIndex != -1)
    {
        EraseFromActiveChannel(ChannelIndex);
        pRemoveChannel->Stop();
        DestroySourceVoice(pRemoveChannel->GetSourceVoice());
        delete pRemoveChannel;
        return;
    }

    //In the AvaibleChannelList
    ChannelIndex = FindChannel(pRemoveChannel, false);
    if (ChannelIndex != -1)
    {
        EraseFromAvailableChannel(ChannelIndex);
        pRemoveChannel->Stop();
        DestroySourceVoice(pRemoveChannel->GetSourceVoice());
        delete pRemoveChannel;
        return;
    }

     LOG(WARNING, "AudioEngine - Failed to Remove Channel: %d ", pRemoveChannel->GetName());
}

void fw::AudioEngine::MoveChannel(Channel* pSwitchChannel)
{
    if (pSwitchChannel != nullptr)
        return;

    int ChannelIndex = FindChannel(pSwitchChannel, true);
    //In the ActiveChannelList
    if (ChannelIndex != -1)
    {
        SwapToAvailableChannel(pSwitchChannel);
        EraseFromActiveChannel(ChannelIndex);
        return;
    }

    //In the AvaibleChannelList
    ChannelIndex = FindChannel(pSwitchChannel, false);
    if (ChannelIndex != -1)
    {
        SwapToActiveChannel(pSwitchChannel);
        EraseFromAvailableChannel(ChannelIndex);
        return;
    }

     LOG(WARNING, "AudioEngine - Failed to Switch Channe >> %s ", pSwitchChannel->GetName());
}

int fw::AudioEngine::FindChannel(const char* ChannelName, bool CheckActiveChannel)
{
    if (CheckActiveChannel)
    {
        for (int i = 0; i < (int)m_ActiveChannels.size(); i++)
        {
           if (strcmp(m_ActiveChannels[i]->GetName(), ChannelName) == 0)
           {
                return i;
           }
        }
    }
    else
    {
        for (int i = 0; i < (int)m_AvailableChannels.size(); i++)
        {
           if (strcmp(m_AvailableChannels[i]->GetName(), ChannelName) == 0)
           {
                return i;
           }
        }
    }

    LOG(WARNING, "AudioEngine - Could Not FindChannel() by ChannelName: %s ", ChannelName);
    return -1;
}

int fw::AudioEngine::FindChannel(Channel* pFindChannel, bool CheckActiveChannel)
{
    if (pFindChannel != nullptr)
        return -1;

    if (CheckActiveChannel)
    {
        for (int i = 0; i < (int)m_ActiveChannels.size(); i++)
        {
           if (m_ActiveChannels[i] == pFindChannel)
           {
                return i;
           }
        }
    }
    else
    {
        for (int i = 0; i < (int)m_AvailableChannels.size(); i++)
        {
           if (m_AvailableChannels[i] == pFindChannel)
           {
                return i;
           }
        }
    }

    LOG(WARNING, "AudioEngine - Could Not FindChannel() by Channel pointer: %s ", pFindChannel->GetName());
    return -1;
}

fw::Channel* fw::AudioEngine::FindChannel(Sound* pSound, bool CheckActiveChannel)
{
    if (pSound != nullptr)
        return nullptr;

    Channel* pChannel = nullptr;
    if (CheckActiveChannel)
    {
        for (int i = 0; i < (int)m_ActiveChannels.size(); i++)
        {
           pChannel = m_ActiveChannels[i];
           if (pChannel->GetSound() == pSound)
           {
                return pChannel;
           }
        }
    }
    else
    {
        for (int i = 0; i < (int)m_AvailableChannels.size(); i++)
        {
           pChannel = m_AvailableChannels[i];
           if (pChannel->GetSound() == pSound)
           {
                return pChannel;
           }
        }
    }

    //LOG(WARNING, "AudioEngine - Could Not FindChannel() by pSound Pointer");
    return pChannel;
}

std::vector<fw::Channel*> fw::AudioEngine::GetArrayofOwningChannels(Sound* pSound)
{
    Channel* pChannel = nullptr;
    std::vector<Channel*> pVectorArrayChannel;
    for (int i = 0; i < m_ActiveChannels.size(); i++)
    {
        pChannel = m_ActiveChannels[i];

        //Does the WaveDataMatch
        if (pChannel->GetSound() == pSound)
        {
            pVectorArrayChannel.push_back(pChannel);
        }
    }

    //Check if it is in AvailableChannels
    for (int i = 0; i < m_AvailableChannels.size(); i++)
    {
        pChannel = m_AvailableChannels[i];

        //Does the WaveDataMatch
        if (pChannel->GetSound() == pSound)
        {
            pVectorArrayChannel.push_back(pChannel);
        }
    }
    return pVectorArrayChannel;
}

void fw::AudioEngine::SwapToActiveChannel(Channel* pSwapChannel)
{
    m_ActiveChannels.push_back(pSwapChannel);
}

void fw::AudioEngine::SwapToAvailableChannel(Channel* pSwapChannel)
{
    m_AvailableChannels.push_back(pSwapChannel);
}

void fw::AudioEngine::EraseFromActiveChannel(int ChannelIndex)
{
    if (ChannelIndex < m_ActiveChannels.size() && ChannelIndex >= 0)
    {
        m_ActiveChannels.erase(m_ActiveChannels.begin() + ChannelIndex);
        return;
    }
    LOG(WARNING, "AudioEngine - ActiveChannel Out of Range Erase");
}

void fw::AudioEngine::EraseFromAvailableChannel(int ChannelIndex)
{
    if (ChannelIndex < m_ActiveChannels.size() && ChannelIndex >= 0)
    {
        m_AvailableChannels.erase(m_AvailableChannels.begin() + ChannelIndex);
        return;
    }
    LOG(WARNING, "AudioEngine - AvailableChannel Out of Range Erase");
}

///
////////////////////////////////////////////////////////////////