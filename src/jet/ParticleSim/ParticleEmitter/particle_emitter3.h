#pragma once

#include <Animation/animation.h>
#include <ParticleSim/particle_system_data3.h>

namespace jet
{
    //! \brief Abstract base class for 3D particle emitter.
    class ParticleEmitter3
    {
    public:
        //! \brief Callback function type for update calls.
        typedef std::function<void(ParticleEmitter3*, double, double)> OnBeginUpdateCallback;

        //! Default constructor.
        ParticleEmitter3();

        //! Destructor
        virtual ~ParticleEmitter3();

        //! Updates the emitter state from \p currentTimeInSeconds to the following timestep.
        void Update(double currentTimeInSeconds, double timeIntervalInSeconds);
        
        //! Returns the target particle system to emit
        const ParticleSystemData3Ptr& Target() const;

        //! Sets the target particle system to emit
        void SetTarget(const ParticleSystemData3Ptr& particles);

        //! Sets the callback function to be called when ParticleEmitter3::Update function is invoked.
        void SetOnBeginUpdateCallback(const OnBeginUpdateCallback& callback);

    protected:
        //! Called when ParticleEmitter3::SetTarget is executed.
        virtual void OnSetTarget(const ParticleSystemData3Ptr& particles);

        //! Called when ParticleEmitter3::Update is executed.
        virtual void OnUpdate(double currentTimeInSeconds, double timeIntervalInSeconds) = 0;

    private:
        ParticleSystemData3Ptr _Particles;
        OnBeginUpdateCallback _OnBeginUpdateCallback;
    };

    typedef std::shared_ptr<ParticleEmitter3> ParticleEmitter3Ptr;
}