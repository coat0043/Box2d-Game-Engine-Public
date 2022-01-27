#pragma once

struct IXAudio2;
struct IXAudio2MasteringVoice;

namespace fw
{
    class Channel;
    class VoiceCallback;
    class Sound;

    class AudioEngine
    {
    public:
        AudioEngine();
        ~AudioEngine();

        //Plays the Sound Passed in. Finds a Channel that wants it. 
        void PlaySFX(Sound* pSound);

        //Stops the Sound Passed in. Finds the Channel that owns.
        void StopSFX(Sound* pSound);

        //Stops All Sounds. Checks Also the Sound that should not be playing.
        void StopAllSounds();

        //Update 3d sound and location
        void Update(float deltaTime);
        // Calls New Channel() and adds it to AvailableChannel
        void AddChannel(const char* ChannelName, Sound* pSound);
        //Finds the location of the channel and deletes it.
        void RemoveChannel(Channel* pRemoveChannel);
        //Finds the location of the channel and switches it.
        void MoveChannel(Channel* pSwitchChannel);
        //Finds location by name and returns the index. CheckActiveChannel Tells you where it is located. If you pass in true then it will check ActiveChannel List, false to check AvailableChannel List.
        int FindChannel(const char* ChannelName, bool CheckActiveChannel);
        //Finds location by Pointer and returns the index. CheckActiveChannel Tells you where it is located. If you pass in true then it will check ActiveChannel List, false to check AvailableChannel List.
        int FindChannel(Channel* pFindChannel, bool CheckActiveChannel);
        //Finds Channel by Sound and returns pointer. If Fails returns nullptr. CheckActiveChannel Tells you where it is located. If you pass in true then it will check ActiveChannel List, false to check AvailableChannel List. 
        Channel* FindChannel(Sound* pSound, bool CheckActiveChannel);
        //Returns vector of channels that contains this sound
        std::vector<Channel*> GetArrayofOwningChannels(Sound* pSound);
        //Does a bounds Check and then calls Erase on ActiveChannel at the given index.
        void EraseFromActiveChannel(int ChannelIndex);
        //Does a bounds Check and then calls Erase on AvailableChannel at the given index.
        void EraseFromAvailableChannel(int ChannelIndex);
        //push_back into ActiveChannel.
        void SwapToActiveChannel(Channel* pSwapChannel);
        //push_back into ActiveChannel.
        void SwapToAvailableChannel(Channel* pSwapChannel);

        //Set the volume for all the audio files being played, range 0.0f to 1.0f
		void SetMasterVolume(float volume);

        //Returns the volume for all the audio files being played
		float GetMasterVolume();

        //Get XAudio2Engine
        IXAudio2* GetAudioEngine() { return m_XAudio2Engine; }


    private:

        //AudioEngine Functions
        void CreateSourceVoice(IXAudio2SourceVoice** source, WAVEFORMATEX* waveFormat, VoiceCallback* callback);
        void DestroySourceVoice(IXAudio2SourceVoice* source);

       
        //Core
        IXAudio2* m_XAudio2Engine;
        IXAudio2MasteringVoice* m_XMasteringVoice;
        HRESULT hr;

        //Channels
        std::vector<Channel*> m_ActiveChannels;
        std::vector<Channel*> m_AvailableChannels;

        
    }; 
} //namespace fw