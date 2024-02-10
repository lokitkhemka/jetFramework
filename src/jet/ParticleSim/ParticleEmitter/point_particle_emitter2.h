#pragma once

#include <ParticleSim/ParticleEmitter/particle_emitter2.h>
#include <limits>
#include <random>

namespace jet
{
    //! \brief 2D Point Particle Emitter
    //!
    //! This class emits particles from a single point in given direction,speed
    //! and spreading angle.
    class PointParticleEmitter2 final : public ParticleEmitter2
    {
    public:
        class Builder;

        //! Constructs an emitter that spawns particles from given origin,
        //! direction, speed, spread angle, max number of new particles per second (max Rate),
        //! max total number of particles to be emitted, and random seed.
        PointParticleEmitter2(const Vector2D& origin, const Vector2D& direction,
                        double speed, double spreadAngleInDegrees,
                        size_t maxRate = 1,
                        size_t maxNumParticles = std::numeric_limits<size_t>::max(),
                        uint32_t seed = 0);
        
        //! Returns max number of new particles per second.
        size_t MaxParticleRate() const;

        //! Sets the Max Number of New Particles Per Second.
        void SetMaxParticleRate(size_t rate);

        //! Returns Max Number of Particles to be emitted.
        size_t MaxNumberOfParticles() const;

        //! Sets the max number of particles to be emitted.
        void SetMaxNumberOfParticles(size_t MaxNumberOfParticles);

        //! Returns builder for PointParticleEmitter2
        static Builder builder();
    
    private:
        std::mt19937 _rng;

        double _FirstFrameTime = 0.0;
        size_t _NumberOfEmittedParticles = 0;

        size_t _MaxParticleRate;
        size_t _MaxNumberOfParticles;

        Vector2D _Origin;
        Vector2D _Direction;
        double _Speed;
        double _SpreadAngleInRadians;

        //! \brief Emits Particles to the Particle System Data
        //!
        //! \param[in] CurrentTimeInSeconds  Current Simulation Time.
        //! \param[in] TimeIntervalInSeconds The Time-Step Interval
        void OnUpdate(double CurrentTimeInSeconds, double TimeIntervalInSeconds) override;

        void Emit(Array1<Vector2D>* NewPositions, Array1<Vector2D>* NewVelocities, size_t MaxNewNumParticles);

        double Random();
    };

    //! Shared pointer for the PointParticleEmitter2 type.
    typedef std::shared_ptr<PointParticleEmitter2> PointParticleEmitter2Ptr;

    //! \brief Front-end to create PointParticleEmitter2 object instance.
    class PointParticleEmitter2::Builder final
    {
    public:
        //! Returns builder with origin
        Builder& WithOrigin(const Vector2D& origin);

        //! Returns builder with direction
        Builder& WithDirection(const Vector2D& direction);

        //! Returns the builder with speed.
        Builder& WithSpeed(double speed);

        //! Returns the builder with spread angle in degrees.
        Builder& WithSpreadAngleInDegrees(double spreadAngleInDegrees);

        //! Returns the builder with Max Number of New particles per second
        Builder& WithMaxParticleRate(size_t MaxParticleRate);

        //! Returns builder with Max Number of Particles.
        Builder& WithMaxNumParticles(size_t MaxNumberParticles);

        //! Returns builder with random seed.
        Builder& WithRandomSeed(uint32_t seed);

        //! Builds PointParticleEmitter2
        PointParticleEmitter2 Build() const;

        //! Builds Shared pointer of PointParticleEmitter2 instance.
        PointParticleEmitter2Ptr MakeShared() const;
    private:
        size_t _MaxParticleRate = 1;
        size_t _MaxNumberOfParticles = kMaxSize;
        Vector2D _Origin{0,0};
        Vector2D _Direction{0,1};
        double _Speed = 1.0;
        double _SpreadAngleInDegrees = 90.0;
        uint32_t _Seed = 0;
    };
}