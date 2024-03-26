#include <jet.h>
#include <Matrix/matrix2.h>
#include <NeighborhoodSearch/point2_hash_grid_search.h>
#include <Samplers/Samplers.h>
#include <Geometry/Surface/surface_to_implicit2.h>
#include "volume_particle_emitter2.h"
#include <Geometry/PointGenerator/triangle_point_generator.h>

namespace jet
{
    static const size_t kDefaultHashGridResolution = 64;

    VolumeParticleEmitter2::VolumeParticleEmitter2(const ImplicitSurface2Ptr& implicitSurface,
                        const BoundingBox2D& bounds, double spacing,
                        const Vector2D& initialVelocity,
                        size_t maxNumberOfParticles,
                        double jitter, bool isOneShot,
                        bool allowOverlapping,
                        uint32_t seed)
                        : _rng(seed), _ImplicitSurface(implicitSurface),
                        _Bounds(bounds), _Spacing(spacing), _InitialVelocity(initialVelocity),
                        _MaxNumberOfParticles(maxNumberOfParticles), _Jitter(jitter),
                        _IsOneShot(isOneShot), _AllowOverlapping(allowOverlapping)
    {
        _PointsGen = std::make_shared<TrianglePointGenerator>();
    }

    void VolumeParticleEmitter2::OnUpdate(double CurrentTimeInSeconds,
                        double TimeIntervalInSeconds)
    {
        UNUSED_VARAIBLE(CurrentTimeInSeconds);
        UNUSED_VARAIBLE(TimeIntervalInSeconds);

        auto particles = Target();

        if (particles == nullptr)
            return;
        
        if (_NumberOfEmittedParticles > 0 && _IsOneShot)
            return;
        
        Array1<Vector2D> newPositions;
        Array1<Vector2D> newVelocities;

        Emit(particles, &newPositions, &newVelocities);
        particles->AddParticles(newPositions, newVelocities);
    }

    void VolumeParticleEmitter2::Emit(const ParticleSystemData2Ptr& particles,
                        Array1<Vector2D>* newPositions, Array1<Vector2D>* newVelocities)
    {
        // Reserving more space for jittering
        const double j = Jitter();
        const double maxJitterDist = 0.5 * j * _Spacing;

        if (_AllowOverlapping || _IsOneShot)
        {
            _PointsGen->ForEachPoint(_Bounds, _Spacing,
                        [&](const Vector2D& point){
                            double newAngleInRadians = (Random() - 0.5) * kTwoPiD;
                            Matrix2x2D rotationMatrix = Matrix2x2D::MakeRotationMatrix(newAngleInRadians);
                            Vector2D randomDir = rotationMatrix * Vector2D();
                            Vector2D offset = maxJitterDist * randomDir;
                            Vector2D candidate = point + offset;
                            if (_ImplicitSurface->SignedDistance(candidate) <= 0.0)
                            {
                                if (_NumberOfEmittedParticles < _MaxNumberOfParticles)
                                {
                                    newPositions->Append(candidate);
                                    ++_NumberOfEmittedParticles;
                                }
                                else
                                    return false;
                            }
                            return true;

                        });
        }
        else
        {
            // Use serial hash grid search for continuous update.
            PointHashGridSearch2 neighborSearch(Size2(kDefaultHashGridResolution, kDefaultHashGridResolution)
                                , 2.0 * _Spacing);
            if (!_AllowOverlapping)
                neighborSearch.Build(particles->Positions());
            
            _PointsGen->ForEachPoint(_Bounds, _Spacing,
                            [&](const Vector2D& point){
                            double newAngleInRadians = (Random() - 0.5) * kTwoPiD;
                            Matrix2x2D rotationMatrix = Matrix2x2D::MakeRotationMatrix(newAngleInRadians);
                            Vector2D randomDir = rotationMatrix * Vector2D();
                            Vector2D offset = maxJitterDist * randomDir;
                            Vector2D candidate = point + offset;
                            if (_ImplicitSurface->SignedDistance(candidate) <= 0.0 &&
                                (!_AllowOverlapping && !neighborSearch.HasNearbyPoint(candidate, _Spacing)))
                            {
                                if (_NumberOfEmittedParticles < _MaxNumberOfParticles)
                                {
                                    newPositions->Append(candidate);
                                    neighborSearch.Add(candidate);
                                    ++_NumberOfEmittedParticles;
                                }
                                else
                                    return false;
                            }
                            return true;

                        });
        }

        newVelocities->Resize(newPositions->Size());
        newVelocities->Set(_InitialVelocity);
    }

    void VolumeParticleEmitter2::SetPointGenerator(const PointGenerator2Ptr& newPointsGen)
    {
        _PointsGen = newPointsGen;
    }

    double VolumeParticleEmitter2::Jitter() const
    {
        return _Jitter;
    }

    void VolumeParticleEmitter2::SetJitter(double newJitter)
    {
        _Jitter = Clamp(newJitter, 0.0, 1.0);
    }

    bool VolumeParticleEmitter2::IsOneShot() const
    {
        return _IsOneShot;
    }

    void VolumeParticleEmitter2::SetIsOneShot(bool newValue)
    {
        _IsOneShot = newValue;
    }

    bool VolumeParticleEmitter2::AllowOverlapping() const
    {
        return _AllowOverlapping;
    }

    void VolumeParticleEmitter2::SetAllowOverlapping(bool newValue)
    {
        _AllowOverlapping = newValue;
    }

    size_t VolumeParticleEmitter2::MaxNumberOfParticles() const
    {
        return _MaxNumberOfParticles;
    }

    void VolumeParticleEmitter2::SetMaxNumberOfParticles(size_t newMaxNumberOfParticles)
    {
        _MaxNumberOfParticles = newMaxNumberOfParticles;
    }

    double VolumeParticleEmitter2::Spacing() const
    {
        return _Spacing;
    }

    void VolumeParticleEmitter2::SetSpacing(double newSpacing)
    {
        _Spacing = newSpacing;
    }

    Vector2D VolumeParticleEmitter2::InitialVelocity() const
    {
        return _InitialVelocity;
    }

    void VolumeParticleEmitter2::SetInitialVelocity(const Vector2D& newIntialVelocity)
    {
        _InitialVelocity = newIntialVelocity;
    }

    double VolumeParticleEmitter2::Random()
    {
        std::uniform_real_distribution<> d(0.0,1.0);
        return d(_rng);
    }

    VolumeParticleEmitter2::Builder VolumeParticleEmitter2::builder()
    {
        return Builder();
    }

    VolumeParticleEmitter2::Builder& VolumeParticleEmitter2::Builder::WithSurface(const Surface2Ptr& surface)
    {
        _ImplicitSurface = std::make_shared<SurfaceToImplicit2>(surface);
        if (!_IsBoundSet)
        {
            _Bounds = surface->BoundingBox();
        }
        return *this;
    }

    VolumeParticleEmitter2::Builder& VolumeParticleEmitter2::Builder::WithMaxRegion(const BoundingBox2D& bounds)
    {
        _Bounds = bounds;
        _IsBoundSet = true;
        return *this;
    }

    VolumeParticleEmitter2::Builder& VolumeParticleEmitter2::Builder::WithSpacing(double spacing)
    {
        _Spacing = spacing;
        return *this;
    }

    VolumeParticleEmitter2::Builder& VolumeParticleEmitter2::Builder::WithInitialVelocity(const Vector2D& initialVelocity)
    {
        _InitialVelocity = initialVelocity;
        return *this;
    }

    VolumeParticleEmitter2::Builder& VolumeParticleEmitter2::Builder::WithMaxNumberOfParticles(size_t maxParticles)
    {
        _MaxNumberOfParticles = maxParticles;
        return *this;
    }

    VolumeParticleEmitter2::Builder& VolumeParticleEmitter2::Builder::WithJitter(double jitter)
    {
        _Jitter = jitter;
        return *this;
    }

    VolumeParticleEmitter2::Builder& VolumeParticleEmitter2::Builder::WithIsOneShot(bool isOneShot)
    {
        _IsOneShot = isOneShot;
        return *this;
    }

    VolumeParticleEmitter2::Builder& VolumeParticleEmitter2::Builder::WithAllowOverlapping(bool allowOverlapping)
    {
        _AllowOverlapping = allowOverlapping;
        return *this;
    }

    VolumeParticleEmitter2::Builder& VolumeParticleEmitter2::Builder::WithRandomSeed(uint32_t seed)
    {
        _Seed = seed;
        return *this;
    }

    VolumeParticleEmitter2 VolumeParticleEmitter2::Builder::Build() const {
        return VolumeParticleEmitter2(
            _ImplicitSurface,
            _Bounds,
            _Spacing,
            _InitialVelocity,
            _MaxNumberOfParticles,
            _Jitter,
            _IsOneShot,
            _AllowOverlapping,
            _Seed);
    }

    VolumeParticleEmitter2Ptr VolumeParticleEmitter2::Builder::MakeShared() const {
        return std::shared_ptr<VolumeParticleEmitter2>(
            new VolumeParticleEmitter2(
                _ImplicitSurface,
                _Bounds,
                _Spacing,
                _InitialVelocity,
                _MaxNumberOfParticles,
                _Jitter,
                _IsOneShot,
                _AllowOverlapping),
            [] (VolumeParticleEmitter2* obj) {
                delete obj;
            });
    }
}