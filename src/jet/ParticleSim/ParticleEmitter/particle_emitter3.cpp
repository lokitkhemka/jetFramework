#include <jet.h>

#include <parallel.h>
#include "particle_emitter3.h"

#include <limits>

namespace jet
{
    ParticleEmitter3::ParticleEmitter3()
    {}

    ParticleEmitter3::~ParticleEmitter3()
    {}

    const ParticleSystemData3Ptr& ParticleEmitter3::Target() const
    {
        return _Particles;
    }

    void ParticleEmitter3::SetTarget(const ParticleSystemData3Ptr& particles)
    {
        _Particles = particles;

        OnSetTarget(particles);
    }

    void ParticleEmitter3::Update(double currentTimeInSeconds, double timeIntervalInSeconds)
    {
        if(_OnBeginUpdateCallback)
        {
            _OnBeginUpdateCallback(this, currentTimeInSeconds, timeIntervalInSeconds);
        }

        OnUpdate(currentTimeInSeconds, timeIntervalInSeconds);
    }

    void ParticleEmitter3::OnSetTarget(const ParticleSystemData3Ptr& particles)
    {
        UNUSED_VARAIBLE(particles);
    }

    void ParticleEmitter3::SetOnBeginUpdateCallback(const OnBeginUpdateCallback& callback)
    {
        _OnBeginUpdateCallback = callback;
    }
}