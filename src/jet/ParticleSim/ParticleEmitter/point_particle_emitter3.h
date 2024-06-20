#pragma once

#include <ParticleSim/ParticleEmitter/particle_emitter3.h>

#include <limits>
#include <random>

namespace jet
{
    //! 3D point particle emitter.
    //! This class emits particles from a single point in given direction, speed and spreading angle.

    class PointParticleEmitter3 final : public ParticleEmitter3
    {
    public:
        class Builder;
        
        //! Constructs an emitter that spawns particles from given origin,
        //! direction, speed, spread angle, max number of new particles per second,
        //! max total number of particles to be emitted, and random seed.
        PointParticleEmitter3(const Vector3D& origin, const Vector3D& direction,
                                double speed, double spreadAngleInDegrees,
                                size_t maxNumNewParticlesPerSec = 1,
                                size_t maxNumParticles = std::numeric_limits<size_t>::max(),
                                uint32_t seed = 0);
        
        //! Returns max number of new particles per second
        size_t MaxParticleRate() const;

        //! Sets Max Number of new particles per second.
        void SetMaxParticleRate(size_t rate);

        //! Returns max number of particles to be emitted.
        size_t MaxNumParticles() const;

        //! Sets max number of particles to be emitted.
        void SetMaxNumParticles(size_t maxNumParticles);

        //! Returns builder for PointParticleEmitter3
        static Builder builder();

    private:
        std::mt19937 _rng;
        double _FirstFrameTimeInSeconds = 0.0;
        size_t _NumEmittedParticles = 0;

        size_t _MaxParticleRate = 1;
        size_t _MaxNumParticles = std::numeric_limits<size_t>::max();

        Vector3D _Origin;
        Vector3D _Direction;
        double _Speed;
        double _SpreadAngleInRadians;

        double Random();
        //! \brief Emits particles to the particle system data.
        void Emit(Array1<Vector3D>* newPositions, Array1<Vector3D>* newVelocities,
                        size_t maxNewParticles);
        
        void OnUpdate(double currentTimeInSeconds, double timeIntervalInSeconds) override;
    };

    typedef std::shared_ptr<PointParticleEmitter3> PointParticleEmitter3Ptr;

    //! \brief Frontend to create PointParticleEmitter3

    class PointParticleEmitter3::Builder final
    {
    public:
        //! Returns builder with origin
        Builder& WithOrigin(const Vector3D& origin);

        //! Returns builder with direction
        Builder& WithDirection(const Vector3D& direction);

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

        //! Builds PointParticleEmitter3
        PointParticleEmitter3 Build() const;

        //! Builds Shared pointer of PointParticleEmitter3 instance.
        PointParticleEmitter3Ptr MakeShared() const;
    private:
        size_t _MaxParticleRate = 1;
        size_t _MaxNumberOfParticles = kMaxSize;
        Vector3D _Origin{0,0, 0};
        Vector3D _Direction{0,1, 0};
        double _Speed = 1.0;
        double _SpreadAngleInDegrees = 90.0;
        uint32_t _Seed = 0;
    };
}