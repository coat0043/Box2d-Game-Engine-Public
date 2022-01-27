#include "FrameworkPCH.h"

#include "Channel.h"


namespace fw
{
    Channel::Channel(const char* ChannelName)
    {
        m_Name = ChannelName;
        m_Source = nullptr;
        m_Importance = 0.0f;
        m_Sound = nullptr;
        m_CallBack = new VoiceCallback();
		m_IsPlaying = false;
		m_SampleOffset = 0;

        ZeroMemory(&m_ChannelWaveFormat, sizeof(m_ChannelWaveFormat));
        m_ChannelWaveFormat = WAVEFORMATEX();
    }

    Channel::~Channel()
    {
        delete m_CallBack;
    }

    void Channel::Play()
    {
        //We can only have one
		if (IsPlaying() == false)
		{
            if (m_Source != nullptr && m_Sound->GetWaveData() != nullptr)
            {
                //Submit the source buffer and start playing the sound
			    m_Source->SubmitSourceBuffer(&m_Sound->GetWaveData()->buffer);
			    m_Source->Start();

			    //Query the state of the buffer to calculate the sample offset, 
			    //this is needed for accurate playback information
			    XAUDIO2_VOICE_STATE state;
			    m_Source->GetState(&state);
			    m_SampleOffset = state.SamplesPlayed;

			    //Reset the buffer
			    m_Sound->GetWaveData()->buffer.PlayBegin = 0;

			    //Set the is playing flag to true
			    m_IsPlaying = true;
            }
			
		}
    }

    void Channel::Stop()
    {
        //Stop playing the sound and flush the source buffer
		m_Source->Stop();
		m_Source->FlushSourceBuffers();

		//Set the is playing flag to false
		m_IsPlaying = false;
    }

    void Channel::SwitchProperties(Sound* pSound)
    {
        SetSound(pSound);
        SetChannelWaveFormat(&pSound->GetWaveData()->waveFormat);
    }

    bool Channel::IsPlaying()
    {
        return m_IsPlaying;
    }

    void Channel::SetChannelWaveFormat(WAVEFORMATEX* pChannelWaveFormat)
    {
        m_ChannelWaveFormat.cbSize = pChannelWaveFormat->cbSize;
        m_ChannelWaveFormat.nAvgBytesPerSec = pChannelWaveFormat->nAvgBytesPerSec;
        m_ChannelWaveFormat.nBlockAlign = pChannelWaveFormat->nBlockAlign;
        m_ChannelWaveFormat.nChannels = pChannelWaveFormat->nChannels;
        m_ChannelWaveFormat.nSamplesPerSec = pChannelWaveFormat->nSamplesPerSec;
        m_ChannelWaveFormat.wBitsPerSample = pChannelWaveFormat->wBitsPerSample;
        m_ChannelWaveFormat.wFormatTag = pChannelWaveFormat->wFormatTag;
    }

    //////////////////////////////////////////////////////
    ///VoiceCallBack Interface
    void VoiceCallback::OnStreamEnd()
    {
    }

    void VoiceCallback::OnVoiceProcessingPassEnd()
    {
    }

    void VoiceCallback::OnVoiceProcessingPassStart(UINT32 SamplesRequired)
    {
    }

    void VoiceCallback::OnBufferEnd(void* pBufferContext)
    {
        if (pBufferContext != nullptr) 
        {
          Channel* pChannel = static_cast<Channel*>(pBufferContext);
          if (pChannel)
          {
              pChannel->Stop();
          }
        }
    }

    void VoiceCallback::OnBufferStart(void* pBufferContext)
    {
    }

    void VoiceCallback::OnLoopEnd(void* pBufferContext)
    {
    }

    void VoiceCallback::OnVoiceError(void* pBufferContext, HRESULT Error)
    {
    }

} // namespace fw