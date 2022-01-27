#include "FrameworkPCH.h"

#include "ParticleEmitter.h"
#include "Objects/Mesh.h"
#include "Objects/Material.h"
#include "Objects/ResourceManager.h"
#include "UI/ResourcesPanel.h"
#include "Math/MyMatrix.h"

namespace fw {

ParticleEmitter::ParticleEmitter(Material* pMaterial)
{
    m_pMesh = new Mesh();
    m_pMaterial = pMaterial;

    m_IsOn = true;

    m_NewParticleTimerDuration = 0.25f;
    m_NewParticleTimer = 0;
    m_IsLooping = true;
    m_EmissionDuration = 3.0f;
    m_EmissionTimer = m_EmissionDuration;

    m_ParticleSpeed = 5.0f;
    m_IsSpeedRandom = false;
    m_RandomSpeedRange = vec2(1, 10);

    m_ParticleLifespan = 3.0f;
    m_ParticlestoSpawn = 1;

    m_ParticleColor = Color::White();
    m_OriginOffset = vec3(0);

    m_IsScaleNonUniform = false;
    m_ParticleSize = vec2(0.5, 0.5);

    m_EmissionWidth = 45.0f;
    m_EmitterRotation = vec3(90, 0, 0);

    m_DelayDuration = 1.5f;
    m_DelayTimer = m_DelayDuration;
    m_IsDelayed = false;

    m_IsGravityEnabled = false;
    m_Gravity = vec3(0, -9.8f, 0);
}

ParticleEmitter::~ParticleEmitter()
{
    delete m_pMesh;
}

void ParticleEmitter::Update(float deltaTime)
{
    // Move existing particles.
    for (int i = 0; i < m_Particles.size(); i++)
    {
        if (m_IsGravityEnabled)
        {
            m_Particles[i].m_Velocity += m_Gravity * deltaTime;
        }

        m_Particles[i].m_Position += m_Particles[i].m_Velocity * deltaTime;

        m_Particles[i].m_TimeToLive -= deltaTime;
        if (m_Particles[i].m_TimeToLive <= 0)
        {
            m_Particles[i] = m_Particles[m_Particles.size() - 1];
            m_Particles.pop_back();
            i--;
        }
    }

    if (!m_IsLooping && m_DelayTimer <= 0)
    {
        m_EmissionTimer -= deltaTime;
        if (m_EmissionTimer <= 0)
            m_IsOn = false;
    }


    if (m_IsDelayed)
    {
        m_DelayTimer -= deltaTime;
    }

    if (m_IsOn)
    {
        if (m_DelayTimer <= 0 || !m_IsDelayed)
        {
            if (m_EmissionTimer > 0 || m_IsLooping)
            {
                // Create new particles.
                m_NewParticleTimer -= deltaTime;
                if (m_NewParticleTimer <= 0)
                {
                    for (int i = 0; i < m_ParticlestoSpawn; i++)
                    {
                        if (m_IsSpeedRandom)
                            m_ParticleSpeed = (float)rand() / RAND_MAX * m_RandomSpeedRange.y + m_RandomSpeedRange.x;
                        std::sort(m_Particles.begin(), m_Particles.end(), SortByZ);

                        m_Particles.push_back(Particle());
                        m_Particles[m_Particles.size() - 1].m_Position = m_OwnerPosition;
                        m_Particles[m_Particles.size() - 1].m_TimeToLive = m_ParticleLifespan;


                        vec3 dir = CalculateDirectionInCone(m_EmissionWidth);

                        MyMatrix mat;
                        mat.CreateRotation(m_EmitterRotation);

                        m_Particles[m_Particles.size() - 1].m_Velocity = (mat * dir) * m_ParticleSpeed;

                    }
                    m_NewParticleTimer = m_NewParticleTimerDuration;
                }
            }
        }
    }

    // Create the mesh.
    if (!m_Particles.empty())
    {
        m_pMesh->Start(GL_TRIANGLES);
        for (Particle& particle : m_Particles)
        {
            m_pMesh->AddSprite(particle.m_Position, m_ParticleSize);
        }
        m_pMesh->End();
    }
}

void ParticleEmitter::Draw(CameraComponent* pCamera, vec3 position)
{
    if (m_pMaterial == nullptr)
        return;

    m_OwnerPosition = position;

    if (!m_Particles.empty())
    {
        MyMatrix identity;
        identity.CreateTranslation(m_OriginOffset);
        std::sort(m_Particles.begin(), m_Particles.end(), SortByZ);
        m_pMesh->Draw(pCamera, &identity, m_pMaterial);
    }
}

void ParticleEmitter::Save(WriterType& writer, ResourceManager* pResources)
{
    JSONSaveVec3(writer, "Location", m_OriginOffset);
    JSONSaveVec3(writer, "Rotation", m_EmitterRotation);
    JSONSaveVec2(writer, "Scale", m_ParticleSize);
    JSONSaveVec2(writer, "RandomSpeedRange", m_RandomSpeedRange);

    writer.Key("NewParticleTimerDuration");
    writer.Double(m_NewParticleTimerDuration);

    writer.Key("Speed");
    writer.Double(m_ParticleSpeed);

    writer.Key("Lifespan");
    writer.Double(m_ParticleLifespan);

    writer.Key("DelayDuration");
    writer.Double(m_DelayDuration);

    writer.Key("EmissionDuration");
    writer.Double(m_EmissionDuration);

    writer.Key("EmissionWidth");
    writer.Double(m_EmissionWidth);

    writer.Key("Gravity");
    writer.Double(m_Gravity.y);

    writer.Key("NumParticles");
    writer.Int(m_ParticlestoSpawn);

    writer.Key("IsOn");
    writer.Bool(m_IsOn);

    writer.Key("IsDelayed");
    writer.Bool(m_IsDelayed);

    writer.Key("IsScaleNonUniform");
    writer.Bool(m_IsScaleNonUniform);

    writer.Key("IsSpeedRandom");
    writer.Bool(m_IsSpeedRandom);

    writer.Key("IsLooping");
    writer.Bool(m_IsLooping);

    writer.Key("IsGravityEnabled");
    writer.Bool(m_IsGravityEnabled);

    JSONSaveCharArray(writer, "Material", pResources->FindMaterialName(m_pMaterial).c_str());
}

void ParticleEmitter::Load(rapidjson::Value& component, ResourceManager* pResources)
{
    if (component.HasMember("Location"))
    {
        m_OriginOffset.x = (float)component["Location"].GetArray()[0].GetDouble();
        m_OriginOffset.y = (float)component["Location"].GetArray()[1].GetDouble();
        m_OriginOffset.z = (float)component["Location"].GetArray()[2].GetDouble();
    }

    if (component.HasMember("Rotation"))
    {
        m_EmitterRotation.x = (float)component["Rotation"].GetArray()[0].GetDouble();
        m_EmitterRotation.y = (float)component["Rotation"].GetArray()[1].GetDouble();
        m_EmitterRotation.z = (float)component["Rotation"].GetArray()[2].GetDouble();
    }

    if (component.HasMember("Scale"))
    {
        m_ParticleSize.x = (float)component["Scale"].GetArray()[0].GetDouble();
        m_ParticleSize.y = (float)component["Scale"].GetArray()[1].GetDouble();
    }

    if (component.HasMember("RandomSpeedRange"))
    {
        m_RandomSpeedRange.x = (float)component["RandomSpeedRange"].GetArray()[0].GetDouble();
        m_RandomSpeedRange.y = (float)component["RandomSpeedRange"].GetArray()[1].GetDouble();
    }

    if (component.HasMember("NewParticleTimerDuration"))
    {
        m_NewParticleTimerDuration = (float)component["NewParticleTimerDuration"].GetDouble();
    }

    if (component.HasMember("Speed"))
    {
        m_ParticleSpeed = (float)component["Speed"].GetDouble();
    }

    if (component.HasMember("Lifespan"))
    {
        m_ParticleLifespan = (float)component["Lifespan"].GetDouble();
    }

    if (component.HasMember("DelayDuration"))
    {
        m_DelayDuration = (float)component["DelayDuration"].GetDouble();
    }

    if (component.HasMember("EmissionDuration"))
    {
        m_EmissionDuration = (float)component["EmissionDuration"].GetDouble();
    }

    if (component.HasMember("EmissionWidth"))
    {
        m_EmissionWidth = (float)component["EmissionWidth"].GetDouble();
    }

    if (component.HasMember("Gravity"))
    {
        m_Gravity.y = (float)component["Gravity"].GetDouble();
    }

    if (component.HasMember("NumParticles"))
    {
        m_ParticlestoSpawn = component["NumParticles"].GetInt();
    }

    if (component.HasMember("IsOn"))
    {
        m_IsOn = component["IsOn"].GetBool();
    }

    if (component.HasMember("IsScaleNonUniform"))
    {
        m_IsScaleNonUniform = component["IsScaleNonUniform"].GetBool();
    }

    if (component.HasMember("IsDelayed"))
    {
        m_IsDelayed = component["IsDelayed"].GetBool();
    }

    if (component.HasMember("IsSpeedRandom"))
    {
        m_IsSpeedRandom = component["IsSpeedRandom"].GetBool();
    }

    if (component.HasMember("IsLooping"))
    {
        m_IsLooping = component["IsLooping"].GetBool();
    }

    if (component.HasMember("IsGravityEnabled"))
    {
        m_IsGravityEnabled = component["IsGravityEnabled"].GetBool();
    }

    if (component.HasMember("Color"))
    {
        m_ParticleColor.r = (float)component["Color"].GetArray()[0].GetDouble();
        m_ParticleColor.g = (float)component["Color"].GetArray()[1].GetDouble();
        m_ParticleColor.b = (float)component["Color"].GetArray()[2].GetDouble();
        m_ParticleColor.a = (float)component["Color"].GetArray()[3].GetDouble();
    }

    if (component.HasMember("Material"))
    {
        m_pMaterial = pResources->GetMaterial(component["Material"].GetString());
    }
}

void ParticleEmitter::AddToInspector(ResourceManager* pResourceManager)
{
    if (ImGui::Checkbox("Toggle On/Off ('E')", &m_IsOn))
    {
        ResetEmission();
    }

    if (ImGui::ColorEdit4("Color", &m_ParticleColor.r))
    {
        m_pMaterial->SetColor(m_ParticleColor);
    }

    ImGui::DragFloat3("Location", &m_OriginOffset.x, 0.05f);
    ImGui::DragFloat3("Rotation", &m_EmitterRotation.x, 0.5f, 0, 360);

    // Is Uniform Scale is turned off, then the Scale slider turns into a DragFloat2 rather than a DragFloat
    // When Uniform scaling is turned back on, the Y scale is automatically reset back to the X scale
    if (!m_IsScaleNonUniform)
    {
        ImGui::DragFloat("Scale", &m_ParticleSize.x, 0.01f, 0.01f);
        m_ParticleSize.y = m_ParticleSize.x;
    }
    else
    {
        ImGui::DragFloat2("Scale", &m_ParticleSize.x, 0.01f, 0.01f);
    }

    ImGui::DragFloat("Spawn Interval", &m_NewParticleTimerDuration, 0.05f, 0, 10);
    ImGui::DragInt("Part. Per Spawn", &m_ParticlestoSpawn, 1, 1, 50);
    ImGui::DragFloat("Cone Width", &m_EmissionWidth, 0.5, 0, 360);
    ImGui::DragFloat("Lifespan", &m_ParticleLifespan, 0.1f, 0, 100);

    // Speed Range will only show in Inspector when Random Speed is enabled
    if (m_IsSpeedRandom)
    {
        ImGui::DragFloat2("Speed Range", &m_RandomSpeedRange.x, 0.05f, 0.05f, 200);

        // Clamps the Max Range so it cannot be lower or equal to the Min Range
        // It will automatically set it 0.5f higher
        if (m_RandomSpeedRange.y <= m_RandomSpeedRange.x)
            m_RandomSpeedRange.y = m_RandomSpeedRange.x + 0.05f;

        // Clamps the Min Range so it cannot be higher than the Max Range.
        // It will automatically set it 0.5f lower
        if (m_RandomSpeedRange.x >= m_RandomSpeedRange.y)
            m_RandomSpeedRange.x = m_RandomSpeedRange.y - 0.05f;

    }
    // If Random Speed is turned off, show a regular Speed slider as normal
    else
    {
        ImGui::DragFloat("Speed", &m_ParticleSpeed, 0.05f, 0, 50);
    }

    if (m_IsGravityEnabled)
    {
        ImGui::DragFloat("Gravity", &m_Gravity.y, 0.05f);
    }

    // If Looping emission is turned off, the option to set an emission duration appears
    if (!m_IsLooping)
    {
        ImGui::DragFloat("Emission Duration", &m_EmissionDuration, 0.1f, 0, 100);
    }

    // If a Start Delay is enabled
    if (m_IsDelayed)
    {
        // The Delay timer is automatically reset if the duration is modified
        if (ImGui::DragFloat("Delay Duration", &m_DelayDuration, 0.1f, 0, 100))
            m_DelayTimer = m_DelayDuration;
    }

    ImGui::Checkbox("Random Speed?", &m_IsSpeedRandom);
    ImGui::SameLine();
    ImGui::Checkbox("Non-Uniform Scale?", &m_IsScaleNonUniform);

    // The Delay timer is automatically reset when Start Delay is enabled/disabled
    if (ImGui::Checkbox("Start Delay? ", &m_IsDelayed))
    {
        m_DelayTimer = m_DelayDuration;
    }
    ImGui::SameLine();

    // The Emission timer is automatically reset when Looping is enabled/disabled
    if (ImGui::Checkbox("Looping?", &m_IsLooping))
    {
        ResetEmission();
    }

    ImGui::Checkbox("Enable Gravity?", &m_IsGravityEnabled);

    if (m_pMaterial)
    {
        std::string* matNameDropped = ResourcesPanel::DropNode("Material", "", "Materials");
        if (matNameDropped)
        {
            m_pMaterial = pResourceManager->GetMaterial(*matNameDropped);
        }
    }
    else
    {
        m_pMaterial = pResourceManager->GetMaterial("Particle");
    }
}

vec3 ParticleEmitter::CalculateDirectionInCone(float maxConeWidth)
{
    maxConeWidth /= 2.0f;
    float angleWithinCone = (float)rand() / RAND_MAX * (maxConeWidth * PI / 180);
    float angleInCircle = (float)rand() / RAND_MAX * 2 * PI;

    vec2 directionOnCircle = vec2(cos(angleInCircle), sin(angleInCircle));
    float scale = sin(angleWithinCone);
    vec3 dirTowardsPositiveZ = vec3(directionOnCircle * scale, cos(angleWithinCone));

    MyMatrix mat;
    mat.CreateRotation(dirTowardsPositiveZ);
    return mat * dirTowardsPositiveZ;
}

void ParticleEmitter::ToggleOnState()
{
    m_IsOn = !m_IsOn;
    ResetEmission();
}
void ParticleEmitter::ResetEmission()
{
    if (!m_IsLooping)
    {
        m_EmissionTimer = m_EmissionDuration;
    }
}
} // namespace fw
