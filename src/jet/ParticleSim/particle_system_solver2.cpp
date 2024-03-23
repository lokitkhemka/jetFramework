#include <jet.h>

#include <Arrays/array-utils.h>
#include <Field/VectorField/constant_vector_field2.h>
#include <parallel.h>
#include "particle_system_solver2.h"
#include <timer.h>

#include <algorithm>

namespace jet
{
    ParticleSystemSolver2::ParticleSystemSolver2()
        : ParticleSystemSolver2(1e-3, 1e-3)
    {}

    ParticleSystemSolver2::ParticleSystemSolver2(double radius, double mass)
    {
        _ParticleSystemData = std::make_shared<ParticleSystemData2>();
        _ParticleSystemData->SetRadius(radius);
        _ParticleSystemData->SetMass(mass);
        _Wind = std::make_shared<ConstantVectorField2>(Vector2D());
    }

    ParticleSystemSolver2::~ParticleSystemSolver2()
    {}

    double ParticleSystemSolver2::DragCoefficient() const
    {
        return _DragCoefficient;
    }

    void ParticleSystemSolver2::SetDragCoefficient(double newDragCoefficient)
    {
        _DragCoefficient = std::max(newDragCoefficient, 0.0);
    }

    double ParticleSystemSolver2::RestitutionCoefficient() const
    {
        return _RestitutionCoefficient;
    }

    void ParticleSystemSolver2::SetRestitutionCoefficient(double newRestitutionCoeff)
    {
        _RestitutionCoefficient = Clamp(newRestitutionCoeff, 0.0, 1.0);
    }

    const Vector2D& ParticleSystemSolver2::Gravity() const
    {
        return _Gravity;
    }

    void ParticleSystemSolver2::SetGravity(const Vector2D& newGravity)
    {
        _Gravity = newGravity;
    }

    const ParticleSystemData2Ptr&
    ParticleSystemSolver2::ParticleSystemData() const
    {
        return _ParticleSystemData;
    }

    const Collider2Ptr& ParticleSystemSolver2::Collider() const
    {
        return _Collider;
    }

    void ParticleSystemSolver2::SetCollider(const Collider2Ptr& newCollider)
    {
        _Collider = newCollider;
    }

    const ParticleEmitter2Ptr& ParticleSystemSolver2::Emitter() const
    {
        return _Emitter;
    }

    void ParticleSystemSolver2::SetEmitter(const ParticleEmitter2Ptr& newEmitter)
    {
        _Emitter = newEmitter;
        newEmitter->SetTarget(_ParticleSystemData);
    }

    const VectorField2Ptr& ParticleSystemSolver2::Wind() const
    {
        return _Wind;
    }

    void ParticleSystemSolver2::SetWind(const VectorField2Ptr& newWind)
    {
        _Wind = newWind;
    }

    void ParticleSystemSolver2::OnInitialize()
    {
        // When initializing the solver, update the collider and emitter state as
        // well since they also affects the initial condition of the simulation.
        Timer timer;
        UpdateCollider(0.0);
        JET_INFO << "Update Collider took "
                << timer.DurationInSeconds() << " seconds";
        
        timer.Reset();
        UpdateEmitter(0.0);
        JET_INFO << "Update Emitter took "
                << timer.DurationInSeconds() << " seconds";
    }

    void ParticleSystemSolver2::OnAdvanceSubTimeStep(double timeStepInSeconds)
    {
        BeginAdvanceTimeStep(timeStepInSeconds);

        Timer timer;
        AccumulateForces(timeStepInSeconds);
        JET_INFO << "Accumulating Forces took "
                << timer.DurationInSeconds() << " seconds";
        
        timer.Reset();
        TimeIntegration(timeStepInSeconds);
        JET_INFO << "Time Integration took "
                <<timer.DurationInSeconds() << " seconds";

        timer.Reset();
        ResolveCollision();
        JET_INFO << "Resolving Collision took "
                << timer.DurationInSeconds() << " seconds";
        
        EndAdvanceTimeStep(timeStepInSeconds);
    }

    void ParticleSystemSolver2::AccumulateForces(double timeStepInSeconds)
    {
        UNUSED_VARAIBLE(timeStepInSeconds);

        // Add External Forces
        AccumulateExternalForces();
    }

    void ParticleSystemSolver2::BeginAdvanceTimeStep(double timeStepInSeconds)
    {
        // Clear Forces
        auto forces = _ParticleSystemData->Forces();
        SetRange1(forces.Size(), Vector2D(), &forces);

        // Update Collider and Emitter
        Timer timer;
        UpdateCollider(timeStepInSeconds);
        JET_INFO << "Update Collider took "
                << timer.DurationInSeconds() << " seconds";
        
        timer.Reset();
        UpdateEmitter(timeStepInSeconds);
        JET_INFO << "Update Emitter took "
                << timer.DurationInSeconds() << " seconds";
        
        // Allocate Buffers
        size_t n = _ParticleSystemData->NumberOfParticles();
        _NewPositions.Resize(n);
        _NewVelocities.Resize(n);

        OnBeginAdvanceTimeStep(timeStepInSeconds);
    }

    void ParticleSystemSolver2::EndAdvanceTimeStep(double timeStepInSeconds)
    {
        // Update Data
        size_t n = _ParticleSystemData->NumberOfParticles();
        auto positions = _ParticleSystemData->Positions();
        auto velocities = _ParticleSystemData->Velocities();
        ParallelFor(kZeroSize, n, [&](size_t i)
                    {
                        positions[i] = _NewPositions[i];
                        velocities[i] = _NewVelocities[i];
        });

        OnEndAdvanceTimeStep(timeStepInSeconds);
    }

    void ParticleSystemSolver2::OnBeginAdvanceTimeStep(double timeStepInSeconds)
    {
        UNUSED_VARAIBLE(timeStepInSeconds);
    }

    void ParticleSystemSolver2::OnEndAdvanceTimeStep(double timeStepInSeconds)
    {
        UNUSED_VARAIBLE(timeStepInSeconds);
    }

    void ParticleSystemSolver2::ResolveCollision()
    {
        ResolveCollision(_NewPositions.Accessor(), _NewVelocities.Accessor());
    }

    void ParticleSystemSolver2::ResolveCollision(ArrayAccessor1<Vector2D> newPositions,
                ArrayAccessor1<Vector2D> newVelocities)
    {
        if (_Collider != nullptr)
        {
            size_t numberOfParticles = _ParticleSystemData->NumberOfParticles();
            const double radius = _ParticleSystemData->Radius();

            ParallelFor(kZeroSize, numberOfParticles,
                        [&](size_t i)
                        {
                            _Collider->ResolveCollision(radius, _RestitutionCoefficient,
                                                &newPositions[i], &newVelocities[i]);
            });
        }
    }

    void ParticleSystemSolver2::SetParticleSystemData(const ParticleSystemData2Ptr& newParticleData)
    {
        _ParticleSystemData = newParticleData;
    }

    void ParticleSystemSolver2::AccumulateExternalForces()
    {
        size_t n = _ParticleSystemData->NumberOfParticles();
        auto forces = _ParticleSystemData->Forces();
        auto velocities = _ParticleSystemData->Velocities();
        auto positions = _ParticleSystemData->Positions();
        const double mass = _ParticleSystemData->Mass();

        ParallelFor(kZeroSize, n,[&](size_t i)
            {
                // Gravity
                Vector2D force = mass * _Gravity;

                // Wind Forces
                Vector2D relVel = velocities[i] - _Wind->Sample(positions[i]);
                force += - _DragCoefficient * relVel;

                forces[i] += force;
        });
    }

    void ParticleSystemSolver2::TimeIntegration(double timeStepInSeconds)
    {
        size_t n = _ParticleSystemData->NumberOfParticles();
        auto forces = _ParticleSystemData->Forces();
        auto velocities = _ParticleSystemData->Velocities();
        auto positions = _ParticleSystemData->Positions();
        const double mass = _ParticleSystemData->Mass();

        ParallelFor(kZeroSize, n, [&](size_t i)
                {
                    // Integrate velocity first
                    Vector2D& newVelocity = _NewVelocities[i];
                    newVelocity = velocities[i] + timeStepInSeconds * forces[i] / mass;

                    // Integrate position
                    Vector2D& newPosition = _NewPositions[i];
                    newPosition = positions[i] + timeStepInSeconds * newVelocity;
        });
    }

    void ParticleSystemSolver2::UpdateCollider(double timeStepInSeconds)
    {
        if (_Collider != nullptr)
        {
            _Collider->Update(CurrentTimeInSeconds(), timeStepInSeconds);
        }
    }

    void ParticleSystemSolver2::UpdateEmitter(double timeStepInSeconds)
    {
        if (_Emitter != nullptr)
        {
            _Emitter->Update(CurrentTimeInSeconds(), timeStepInSeconds);
        }
    }

    ParticleSystemSolver2::Builder ParticleSystemSolver2::builder()
    {
        return Builder();
    }

    ParticleSystemSolver2 ParticleSystemSolver2::Builder::Build() const
    {
        return ParticleSystemSolver2(_Radius, _Mass);
    }

    ParticleSystemSolver2Ptr ParticleSystemSolver2::Builder::MakeShared() const
    {
        return std::shared_ptr<ParticleSystemSolver2>(
            new ParticleSystemSolver2(_Radius, _Mass),
            [] (ParticleSystemSolver2* obj)
            {
                delete obj;
        });
    }
}