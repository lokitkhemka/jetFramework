#include <parallel.h>
#include "particle_emitter2.h"

#include <jet.h>

#include <limits>

namespace jet
{
    ParticleEmitter2::ParticleEmitter2() {}

    ParticleEmitter2::~ParticleEmitter2() {}

    const ParticleSystemData2Ptr& ParticleEmitter2::Target() const
    {
        return _Particles;
    }

    void ParticleEmitter2::SetTarget(const ParticleSystemData2Ptr& particles)
    {
        _Particles = particles;
        OnSetTarget(particles);
    }

    void ParticleEmitter2::Update(double CurrentTimeInSeconds, double TimeIntervalInSeconds)
    {
        if (_OnBeginUpdateCallback)
            _OnBeginUpdateCallback(this, CurrentTimeInSeconds, TimeIntervalInSeconds);
        
        OnUpdate(CurrentTimeInSeconds, TimeIntervalInSeconds);
    }

    void ParticleEmitter2::OnSetTarget(const ParticleSystemData2Ptr& particles)
    {
        UNUSED_VARAIBLE(particles);
    }

    void ParticleEmitter2::SetOnBeginUpdateCallback(const OnBeginUpdateCallback& callback)
    {
        _OnBeginUpdateCallback = callback;
    }
}