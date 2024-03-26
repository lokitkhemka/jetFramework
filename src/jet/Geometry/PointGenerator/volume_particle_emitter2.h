#pragma once

#include <constants.h>
#include <Geometry/BoundingBox/bounding_box2.h>
#include <Geometry/ImplicitSurface/implicit_surface2.h>
#include <ParticleSim/ParticleEmitter/particle_emitter2.h>
#include <Geometry/PointGenerator/point2_generator.h>

#include <limits>
#include <memory>
#include <random>

namespace jet
{
    //! \brief 2D volumetric particle emitter.
    class VolumeParticleEmitter2 final : public ParticleEmitter2
    {
    public:
        class Builder;

        //! Constructs an emitter that spawns particles from given implicit surface
        //! which defines the volumetric geometry. Provided bounding box limits the
        //! particle generation region.
        //! 
        //! \param[in] implicitSurface The implicitSurface
        //! \param[in]  bounds The max region
        //! \param[in] spacing The spacing between particles.
        //! \param[in] initalVelocity The initial Velocity
        //! \param[in] maxNumberOfParticles Max Number of Particles to be emitted
        //!
        //! \param[in] jitter The jitter amount between 0 and 1
        //! \param[in] isOneShot Set to true if the particles are emitted just once
        //! \param[in] allowOverlapping  true if particles can overlap
        //! \param[in] seed The random seed.
        VolumeParticleEmitter2(const ImplicitSurface2Ptr& implicitSurface,
                        const BoundingBox2D& bounds, double spacing,
                        const Vector2D& initialVelocity = Vector2D(),
                        size_t maxNumberOfParticles = kMaxSize,
                        double jitter = 0.0, bool isOneShot = true,
                        bool allowOverlapping = false,
                        uint32_t seed = 0);
        
        //! \brief Sets the point generator
        //!
        //! This functionsets the point generator that defines the pattern of the point
        //! distribution within the volume
        void SetPointGenerator(const PointGenerator2Ptr& newPoint2Gen);

        //! Returns jitter amount
        double Jitter() const;

        //! Sets the jitter amount
        void SetJitter(double newJitter);

        //! Returns true if particles should be emitted just once.
        bool IsOneShot() const;

        //! \brief Sets the flag to true if particles are emitted just once. 
        //! 
        //! If set to true, the emitter will generate particles only ondce even after
        //! multiple emit calls. If false, it will keep generating particles. Default
        //! value is true.
        void SetIsOneShot(bool newValue);

        //! Returns true if particles can be overlapped.
        bool AllowOverlapping() const;

        //! Sets the flag whether or not the particles can overlap with each other.
        void SetAllowOverlapping(bool newValue);

        //! Returns the max number of particles to be emitted.
        size_t MaxNumberOfParticles() const;

        //! Sets the max number of particles to be emitted.
        void SetMaxNumberOfParticles (size_t newMaxNumberOfParticles);

        //! Returns the spacing between particles.
        double Spacing() const;

        //! Sets the spacing between particles.
        void SetSpacing(double newSpacing);

        //! Returns the initial velocity of the particles.
        Vector2D InitialVelocity() const;

        //! Sets the initial velocity of the particles
        void SetInitialVelocity(const Vector2D& newInitialVelocity);

        //! Returns builder for VolumeParticleEmitter2
        static Builder builder();

    private:
        std::mt19937 _rng;

        ImplicitSurface2Ptr _ImplicitSurface;
        BoundingBox2D _Bounds;
        double _Spacing;
        Vector2D _InitialVelocity;
        PointGenerator2Ptr _PointsGen;

        size_t _MaxNumberOfParticles = kMaxSize;
        size_t _NumberOfEmittedParticles = 0;

        double _Jitter = 0.0;
        bool _IsOneShot = true;
        bool _AllowOverlapping = false;

        //! Emits particles to the particle system data.
        void OnUpdate(double CurrentTimeInSeconds, double TimeIntervalInSeconds) override;

        void Emit(const ParticleSystemData2Ptr& particles,
                    Array1<Vector2D>* newPositions,
                    Array1<Vector2D>* newVelocities);
        double Random();
    };

    typedef std::shared_ptr<VolumeParticleEmitter2> VolumeParticleEmitter2Ptr;

    //! Frontend to create VolumeParticleEmitter2 objects
    class VolumeParticleEmitter2::Builder final
    {
    public:
        //! Returns builder with implicit surface defining volume shape
        Builder& WithImplicitSurface(const ImplicitSurface2Ptr& implicitSurface);

        //! Returns builder with surface defining volume shape.
        Builder& WithSurface(const Surface2Ptr& surface);

        //! Returns builder with max region.
        Builder& WithMaxRegion(const BoundingBox2D& bounds);

        //! Returns builder with spacing.
        Builder& WithSpacing(double spacing);

        //! Returns builder with initial velocity
        Builder& WithInitialVelocity(const Vector2D& intialVelocity);

        //! Returns builder with max number of particles.
        Builder& WithMaxNumberOfParticles(size_t maxNumberOfParticles);

        //! Returns builder with jitter amount
        Builder& WithJitter(double jitter);

        //! Returns builder with one-shot flag
        Builder& WithIsOneShot(bool IsOneShot);

        //! Returns builder with overlapping flag.
        Builder& WithAllowOverlapping(bool allowOverlapping);

        //! Returns builder with random seed.
        Builder& WithRandomSeed(uint32_t seed);

        //! Builds VolumeParticleEmitter2.
        VolumeParticleEmitter2 Build() const;

        //! Builds shared pointer of VolumeParticleEmitter2 instance.
        VolumeParticleEmitter2Ptr MakeShared() const;

    private:
        ImplicitSurface2Ptr _ImplicitSurface;
        bool _IsBoundSet = false;
        BoundingBox2D _Bounds;
        double _Spacing = 0.1;
        Vector2D _InitialVelocity{0, 0};

        size_t _MaxNumberOfParticles = kMaxSize;

        double _Jitter = 0.0;
        bool _IsOneShot = true;
        bool _AllowOverlapping = false;
        uint32_t _Seed = 0;
    };
}