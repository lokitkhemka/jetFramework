
#include <Matrix/matrix2.h>
#include "point_particle_emitter3.h"
#include <Samplers/Samplers.h>

namespace jet
{
    PointParticleEmitter3::PointParticleEmitter3(const Vector3D& origin,
                const Vector3D& direction, double speed, double spreadAngleInDegrees,
                size_t MaxParticleRate, size_t MaxNumParticles, uint32_t seed) :
                _rng(seed), _MaxParticleRate(MaxParticleRate), _MaxNumParticles(MaxNumParticles),
                _Origin(origin), _Direction(direction), _Speed(speed),
                _SpreadAngleInRadians(DegreesToRadians(spreadAngleInDegrees))
    {}

    size_t PointParticleEmitter3::MaxParticleRate() const
    {
        return _MaxParticleRate;
    }

    void PointParticleEmitter3::SetMaxParticleRate(size_t rate)
    {
        _MaxParticleRate = rate;
    }

    size_t PointParticleEmitter3::MaxNumParticles() const
    {
        return _MaxNumParticles;
    }

    void PointParticleEmitter3::SetMaxNumParticles(size_t MaxNumberOfParticles)
    {
        _MaxNumParticles = MaxNumberOfParticles;
    }

    void PointParticleEmitter3::OnUpdate(double CurrentTimeInSeconds, double TimeIntervalInSeconds)
    {
        auto particles = Target();

        if (particles == nullptr)
            return;
        
        if (_NumEmittedParticles == 0)
            _FirstFrameTimeInSeconds = CurrentTimeInSeconds;
        
        double ElapsedTime = CurrentTimeInSeconds - _FirstFrameTimeInSeconds;

        size_t newMaxTotalNumberOfEmittedParticles = static_cast<size_t>(std::ceil(
            (ElapsedTime + TimeIntervalInSeconds) * _MaxParticleRate));
        newMaxTotalNumberOfEmittedParticles = std::min(newMaxTotalNumberOfEmittedParticles, _MaxNumParticles);
        size_t MaxNumberofNewParticles = newMaxTotalNumberOfEmittedParticles - _NumEmittedParticles;

        if (MaxNumberofNewParticles > 0)
        {
            Array1<Vector3D> CandidatePositions;
            Array1<Vector3D> CandidateVelocities;
            Array1<Vector3D> NewPositions;
            Array1<Vector3D> NewVelocities;

            Emit(&CandidatePositions, &CandidateVelocities, MaxNumberofNewParticles);

            NewPositions.Append(CandidatePositions);
            NewVelocities.Append(CandidateVelocities);

            particles->AddParticles(NewPositions, NewVelocities);

            _NumEmittedParticles += NewPositions.Size();
        }

    }

    void PointParticleEmitter3::Emit(Array1<Vector3D>* NewPositions, 
            Array1<Vector3D>* NewVelocities, size_t MaxNewNumParticles)
    {
        for (size_t i = 0; i < MaxNewNumParticles; ++i)
        {
            Vector3D newDirection = UniformSampleCone(Random(), Random(),
                                        _Direction, _SpreadAngleInRadians);
            
            NewPositions->Append(_Origin);
            NewVelocities->Append(_Speed * newDirection);
        }
    }

    double PointParticleEmitter3::Random()
    {
        std::uniform_real_distribution<> d(0.0, 1.0);
        return d(_rng);
    }

    PointParticleEmitter3::Builder PointParticleEmitter3::builder()
    {
        return Builder();
    }

    PointParticleEmitter3::Builder& PointParticleEmitter3::Builder::WithOrigin(const Vector3D& origin)
    {
        _Origin = origin;
        return *this;
    }

    PointParticleEmitter3::Builder& PointParticleEmitter3::Builder::WithDirection(const Vector3D& direction)
    {
        _Direction = direction;
        return *this;
    }

    PointParticleEmitter3::Builder& PointParticleEmitter3::Builder::WithSpeed(double speed)
    {
        _Speed = speed;
        return *this;
    }

    PointParticleEmitter3::Builder& PointParticleEmitter3::Builder::WithSpreadAngleInDegrees(double spreadAngleInDegrees)
    {
        _SpreadAngleInDegrees = spreadAngleInDegrees;
        return *this;
    }

    PointParticleEmitter3::Builder& PointParticleEmitter3::Builder::WithMaxParticleRate(size_t maxParticleRate)
    {
        _MaxParticleRate = maxParticleRate;
        return *this;
    }

    PointParticleEmitter3::Builder& PointParticleEmitter3::Builder::WithMaxNumParticles(size_t maxNumParticles)
    {
        _MaxNumberOfParticles = maxNumParticles;
        return *this;
    }

    PointParticleEmitter3::Builder& PointParticleEmitter3::Builder::WithRandomSeed(uint32_t seed)
    {
        _Seed = seed;
        return *this;
    }

    PointParticleEmitter3 PointParticleEmitter3::Builder::Build() const
    {
        return PointParticleEmitter3(_Origin, _Direction, _Speed, _SpreadAngleInDegrees, _MaxParticleRate,
                        _MaxNumberOfParticles, _Seed);
    }

    PointParticleEmitter3Ptr PointParticleEmitter3::Builder::MakeShared() const
    {
        return std::shared_ptr<PointParticleEmitter3>(
            new PointParticleEmitter3(_Origin, _Direction, _Speed,
            _MaxParticleRate, _MaxNumberOfParticles, _Seed), [](PointParticleEmitter3* obj){
                delete obj;});
    }
}