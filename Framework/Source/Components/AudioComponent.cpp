#include "FrameworkPCH.h"

#include "AudioComponent.h"
#include "Audio/AudioEngine.h"
#include "Audio/Channel.h"
#include "Core/GameCore.h"
#include "Objects/GameObject.h"
#include "Objects/Sound.h"
#include "Objects/ResourceManager.h"
#include "UI/ResourcesPanel.h"
#include "Scene/Scene.h"

namespace fw {


void AudioComponent::Save(WriterType& writer)
{
    Component::Save(writer);

    ResourceManager* pResources = m_pGameObject->GetScene()->GetGameCore()->GetResourceManager();
}

void AudioComponent::Load(rapidjson::Value& component)
{
    ResourceManager* pResources = m_pGameObject->GetScene()->GetGameCore()->GetResourceManager();
}

void AudioComponent::AddToInspector()
{
    ImGui::PushID(this);
    AudioEngine* pAudioEngine = m_pGameObject->GetScene()->GetGameCore()->GetAudioEngine();

    Channel* pChannel = pAudioEngine->FindChannel(m_pSound, true);
    //Channel is Located in ActiveChannel
    if (pChannel != nullptr)
    {
        ImGui::Text("%s State: ", pChannel->GetName());
        ImGui::SameLine();
        if (pChannel->IsPlaying() == true)
            ImGui::TextColored(ImVec4(0.0f, 1.0f, 0.0f, 1.0f), "Playing" );
        else
            ImGui::TextColored(ImVec4( 1.0f, 1.0f, 0.0f, 1.0f), "Error" );

        ResourcesPanel::DropNodeSound(&m_pSound, m_pGameObject);

        ImGui::PopID();
        return;
    }

    pChannel = pAudioEngine->FindChannel(m_pSound, false);
    //Channel is Located in AvailableChannel
    if (pChannel != nullptr)
    {
        ImGui::Text("%s State: ", pChannel->GetName());
        ImGui::SameLine();
        if (pChannel->IsPlaying() == false)
            ImGui::TextColored(ImVec4( 1.0f, 0.0f, 0.0f, 1.0f), "Stopped" );
        else
            ImGui::TextColored(ImVec4( 1.0f, 1.0f, 0.0f, 1.0f), "Error" );

        ResourcesPanel::DropNodeSound(&m_pSound, m_pGameObject);

        ImGui::PopID();
        return;
    }

    //Could Not Find Channel that Owns Sound
    ImGui::Text("N/A State: NONE");
    ResourcesPanel::DropNodeSound(&m_pSound, m_pGameObject);
    if (ImGui::Button("Play"))
    {
        PlayAudioComponent();
    }
    ImGui::SameLine();
    if (ImGui::Button("Stop"))
    {
        StopAudioComponent();
    }

    ImGui::PopID();
}
void AudioComponent::PlayAudioComponent()
{
    AudioEngine* pAudioEngine = m_pGameObject->GetScene()->GetGameCore()->GetAudioEngine();
    pAudioEngine->PlaySFX(m_pSound);
}

void AudioComponent::StopAudioComponent()
{
    AudioEngine* pAudioEngine = m_pGameObject->GetScene()->GetGameCore()->GetAudioEngine();
    pAudioEngine->StopSFX(m_pSound);
}


} // namespace fw