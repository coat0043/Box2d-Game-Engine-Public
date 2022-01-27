#pragma once
#include "Objects/Sound.h"

namespace fw
{


    class VoiceCallback : public IXAudio2VoiceCallback
    {
    public:
        HANDLE m_hBufferEndEvent;
        VoiceCallback(): m_hBufferEndEvent( CreateEvent( NULL, FALSE, FALSE, NULL ) ){}
        ~VoiceCallback(){ CloseHandle( m_hBufferEndEvent ); }

       //Called when the voice has just finished playing a contiguous audio stream.
       void OnStreamEnd() override;
        //{ SetEvent( m_hBufferEndEvent ); }

       //Unused methods are stubs
       void OnVoiceProcessingPassEnd() override;
       void OnVoiceProcessingPassStart(UINT32 SamplesRequired) override; 
       void OnBufferEnd(void * pBufferContext) override;
       void OnBufferStart(void * pBufferContext) override; 
       void OnLoopEnd(void * pBufferContext) override; 
       void OnVoiceError(void * pBufferContext, HRESULT Error) override;
    };

    struct WaveData
	{
		WaveData()
		{
			ZeroMemory(&waveFormat, sizeof(waveFormat));
			ZeroMemory(&buffer, sizeof(buffer));
			data = nullptr;
		}

		~WaveData()
		{
			delete[] data;
		}

		WAVEFORMATEX waveFormat;
		XAUDIO2_BUFFER buffer;
		void* data;
	};

    class Channel
    {
    public:
        Channel(const char* ChannelName);
        ~Channel();

        void Play();
        void Stop();
        void SwitchProperties(Sound* pSound);
        const char* GetName() { return m_Name; }
        WaveData* GetWavaData() { return m_Sound->GetWaveData(); }
        IXAudio2SourceVoice* GetSourceVoice() { return m_Source; }
        VoiceCallback* GetCallBack() { return m_CallBack; }
        Sound* GetSound() { return m_Sound; }
        void SetSound(Sound* pSound) { m_Sound = pSound; }
        void SetBufferContextPointerToThis() { m_Sound->GetWaveData()->buffer.pContext = this;}
        void SetImportance(float Importance) { m_Importance = Importance; }
        bool IsPlaying();
        void SetChannelWaveFormat(WAVEFORMATEX* pChannelWaveFormat);

        IXAudio2SourceVoice* m_Source;
        WAVEFORMATEX m_ChannelWaveFormat;


    private:
        const char* m_Name;
        float m_Importance;
        Sound* m_Sound;
        VoiceCallback* m_CallBack;
		bool m_IsPlaying;
		unsigned long long m_SampleOffset;
    };

} //namespace fw