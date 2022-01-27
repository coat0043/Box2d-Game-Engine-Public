#pragma once

#include "Math/Vector.h"
#include "Utility/Color.h"
#include "Utility/JSONHelpers.h"

namespace fw {

class CameraComponent;
class Material;
class Mesh;
class ResourceManager;

class ParticleEmitter
{
    struct Particle
    {
        vec3 m_Position;
        vec3 m_Velocity;
        float m_TimeToLive;
    };

public:
    ParticleEmitter(Material* pMaterial);
    virtual ~ParticleEmitter();

    void Update(float deltaTime);
    void Draw(CameraComponent* pCamera, vec3 position);

    void Save(WriterType& writer, ResourceManager* pResources);
    virtual void Load(rapidjson::Value& component, ResourceManager* pResources);

    void AddToInspector(ResourceManager* pResourceManager);

    void SetMaterial(Material* pMaterial) { m_pMaterial = pMaterial; }

    void ToggleOnState();

    vec3 CalculateDirectionInCone(float maxConeWidth);
    
    void ResetEmission();

    static bool SortByZ(Particle& p1, Particle& p2) { return p1.m_Position.z > p2.m_Position.z; }

protected:
    std::vector<Particle> m_Particles;

    float m_NewParticleTimer;

    // How much time passes before a new particle is created
    float m_NewParticleTimerDuration;

    // Speed of the particle
    float m_ParticleSpeed;

    // How long a particle stays in game before being destroyed
    float m_ParticleLifespan;

    // How many particles to spawn at each interval
    int m_ParticlestoSpawn;
  
    float m_EmissionTimer;

    // How long the PE can emit particles for
    float m_EmissionDuration;

    float m_DelayTimer;

    // How long of a delay until the particle emitter can start
    float m_DelayDuration;

    // Is the Emitter On;
    bool m_IsOn;

    // Is there a delay before the emitter can spawn particles?
    bool m_IsDelayed;

    // Is the particle emitter constantly emitting?
    bool m_IsLooping;

    // Are the speeds of each particle random?
    bool m_IsSpeedRandom;

    // Are we able to scale the particles non-uniformly
    bool m_IsScaleNonUniform;

    bool m_IsGravityEnabled;

    // The size of the particle sprites.
    vec2 m_ParticleSize;

    // Min and Max speeds to generate random speeds in
    vec2 m_RandomSpeedRange;

    // The position where the particles are emitting from
    vec3 m_OriginOffset;

    // The width of the cone we're emitting particles from
    float m_EmissionWidth;

    vec3 m_Gravity;

    // Angle that the emitter is pointing towards
    vec3 m_EmitterRotation;

    vec3 m_OwnerPosition;
    Mesh* m_pMesh;
    Material* m_pMaterial;
    Color m_ParticleColor;
};
} // namespace fw
