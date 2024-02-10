
#include <Matrix/matrix2.h>
#include "point_particle_emitter2.h"
#include <Samplers/Samplers.h>

namespace jet
{
    PointParticleEmitter2::PointParticleEmitter2(const Vector2D& origin,
                const Vector2D& direction, double speed, double spreadAngleInDegrees,
                size_t MaxParticleRate, size_t MaxNumParticles, uint32_t seed) :
                _rng(seed), _MaxParticleRate(MaxParticleRate), _MaxNumberOfParticles(MaxNumParticles),
                _Origin(origin), _Direction(direction), _Speed(speed),
                _SpreadAngleInRadians(DegreesToRadians(spreadAngleInDegrees))
    {}

    size_t PointParticleEmitter2::MaxParticleRate() const
    {
        return _MaxParticleRate;
    }

    void PointParticleEmitter2::SetMaxParticleRate(size_t rate)
    {
        _MaxParticleRate = rate;
    }

    size_t PointParticleEmitter2::MaxNumberOfParticles() const
    {
        return _MaxNumberOfParticles;
    }

    void PointParticleEmitter2::SetMaxNumberOfParticles(size_t MaxNumberOfParticles)
    {
        _MaxNumberOfParticles = MaxNumberOfParticles;
    }

    void PointParticleEmitter2::OnUpdate(double CurrentTimeInSeconds, double TimeIntervalInSeconds)
    {
        auto particles = Target();

        if (particles == nullptr)
            return;
        
        if (_NumberOfEmittedParticles == 0)
            _FirstFrameTime = CurrentTimeInSeconds;
        
        double ElapsedTime = CurrentTimeInSeconds - _FirstFrameTime;

        size_t newMaxTotalNumberOfEmittedParticles = static_cast<size_t>(std::ceil(
            (ElapsedTime + TimeIntervalInSeconds) * _MaxParticleRate));
        newMaxTotalNumberOfEmittedParticles = std::min(newMaxTotalNumberOfEmittedParticles, _MaxNumberOfParticles);
        size_t MaxNumberofNewParticles = newMaxTotalNumberOfEmittedParticles - _NumberOfEmittedParticles;

        if (MaxNumberofNewParticles > 0)
        {
            Array1<Vector2D> CandidatePositions;
            Array1<Vector2D> CandidateVelocities;
            Array1<Vector2D> NewPositions;
            Array1<Vector2D> NewVelocities;

            Emit(&CandidatePositions, &CandidateVelocities, MaxNumberofNewParticles);

            NewPositions.Append(CandidatePositions);
            NewVelocities.Append(CandidateVelocities);

            particles->AddParticles(NewPositions, NewVelocities);

            _NumberOfEmittedParticles += NewPositions.Size();
        }

    }

    void PointParticleEmitter2::Emit(Array1<Vector2D>* NewPositions, 
            Array1<Vector2D>* NewVelocities, size_t MaxNewNumParticles)
    {
        for (size_t i = 0; i < MaxNewNumParticles; ++i)
        {
            double NewAngleInRadian = (Random() - 0.5) * _SpreadAngleInRadians;
            Matrix2x2D RotationMatrix = Matrix2x2D::MakeRotationMatrix(NewAngleInRadian);

            NewPositions->Append(_Origin);
            NewVelocities->Append(_Speed * (RotationMatrix * _Direction));
        }
    }

    double PointParticleEmitter2::Random()
    {
        std::uniform_real_distribution<> d(0.0, 1.0);
        return d(_rng);
    }

    PointParticleEmitter2::Builder PointParticleEmitter2::builder()
    {
        return Builder();
    }

    PointParticleEmitter2::Builder& PointParticleEmitter2::Builder::WithOrigin(const Vector2D& origin)
    {
        _Origin = origin;
        return *this;
    }

    PointParticleEmitter2::Builder& PointParticleEmitter2::Builder::WithDirection(const Vector2D& direction)
    {
        _Direction = direction;
        return *this;
    }

    PointParticleEmitter2::Builder& PointParticleEmitter2::Builder::WithSpeed(double speed)
    {
        _Speed = speed;
        return *this;
    }

    PointParticleEmitter2::Builder& PointParticleEmitter2::Builder::WithSpreadAngleInDegrees(double spreadAngleInDegrees)
    {
        _SpreadAngleInDegrees = spreadAngleInDegrees;
        return *this;
    }

    PointParticleEmitter2::Builder& PointParticleEmitter2::Builder::WithMaxParticleRate(size_t maxParticleRate)
    {
        _MaxParticleRate = maxParticleRate;
        return *this;
    }

    PointParticleEmitter2::Builder& PointParticleEmitter2::Builder::WithMaxNumParticles(size_t maxNumParticles)
    {
        _MaxNumberOfParticles = maxNumParticles;
        return *this;
    }

    PointParticleEmitter2::Builder& PointParticleEmitter2::Builder::WithRandomSeed(uint32_t seed)
    {
        _Seed = seed;
        return *this;
    }

    PointParticleEmitter2 PointParticleEmitter2::Builder::Build() const
    {
        return PointParticleEmitter2(_Origin, _Direction, _Speed, _SpreadAngleInDegrees, _MaxParticleRate,
                        _MaxNumberOfParticles, _Seed);
    }

    PointParticleEmitter2Ptr PointParticleEmitter2::Builder::MakeShared() const
    {
        return std::shared_ptr<PointParticleEmitter2>(
            new PointParticleEmitter2(_Origin, _Direction, _Speed,
            _MaxParticleRate, _MaxNumberOfParticles, _Seed), [](PointParticleEmitter2* obj){
                delete obj;});
    }
}