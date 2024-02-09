#pragma once

#include <Animation/animation.h>
#include <ParticleSim/particle_system_data2.h>

namespace jet
{
    //! \brief Abstract base class for 2D particle emitter
    class ParticleEmitter2
    {
    public:
        //! \brief Callback function type for update calls
        //!
        //! This type of callback function will take the emitter pointer, current
        //! time, and timer interval in seconds
        typedef std::function<void(ParticleEmitter2*, double, double)> OnBeginUpdateCallback;

        //! Default constructor.
        ParticleEmitter2();

        //! Destructor
        virtual ~ParticleEmitter2();

        //! Updates the emitter state from \p CurrentTimeInSeconds to the following time-step.
        void Update(double CurrentTimeInSeconds, double TimeIntervalInSeconds);

        //! Returns the target particle system to emit.
        const ParticleSystemData2Ptr& Target() const;

        //! Sets the target particle system to emit.
        void SetTarget(const ParticleSystemData2Ptr& ParticleSys);

        //! \brief Sets the callback funciton to be called when ParticleEmitter2::Update function is called.
        //!
        //! The callback function takes current simulation time in seconds. Use this callback
        //! to track any motion or any state changes related to this emitter.
        //!
        //! \param[in] callback The callback function.
        void SetOnBeginUpdateCallback(const OnBeginUpdateCallback& callback);

    protected:
        //! Called when ParticleEmitter2::SetTarget is executed.
        virtual void OnSetTarget(const ParticleSystemData2Ptr& Particles);

        //! Called when ParticleEmitter3::Update is executed.
        virtual void OnUpdate(double CurrentTimeInSeconds, double TimeIntervalInSeconds) = 0;

    private:
        ParticleSystemData2Ptr _Particles;
        OnBeginUpdateCallback _OnBeginUpdateCallback;
    };

    //! Shared Pointer for the ParticleEmitter2 type.
    typedef std::shared_ptr<ParticleEmitter2> ParticleEmitter2Ptr;
}