#pragma once

#include<ParticleSim/Collision/collider2.h>
#include <constants.h>
#include <Field/VectorField/vector_field2.h>
#include <ParticleSim/ParticleEmitter/particle_emitter2.h>
#include <ParticleSim/particle_system_data2.h>
#include <Animation/physics_animation.h>

namespace jet
{
    //! \brief Basic 2D Particle System Solver.
    //!
    //! This class implements basic particle system solver. It includes gravity,
    //! air drag and collision. But it does not compute particle-to-particle interaction.
    //! Thus, this solver is suitable for performing simple spray-like simulations
    //! with low computational cost. This class can be extended to add more particle to 
    //! particle interactions
    //! 
    //! \see SPHSolver2
    class ParticleSystemSolver2 : public PhysicsAnimation
    {
    public:
        class Builder;

        //! Constructs an empty solver.
        ParticleSystemSolver2();

        //! Constructs a solver with particle parameters
        ParticleSystemSolver2(double radius, double mass);

        //! Destructor
        virtual ~ParticleSystemSolver2();

        //! Returns the drag coefficient.
        double DragCoefficient() const;

        //! \brief Sets the drag coefficient
        //!
        //! The drag coefficient controls the amount of air-drag. The coefficient
        //! should be a positive numer of 0 means no drag force.
        void SetDragCoefficient(double NewDragCoefficient);

        //! Returns the restitution coefficient
        double RestitutionCoefficient() const;

        //! Sets the restitution coefficient.
        void SetRestitutionCoefficient(double newRestitutionCoeff);

        //! Returns the gravity
        const Vector2D& Gravity() const;

        //! Sets the gravity
        void SetGravity(const Vector2D& NewGravity);;

        //! \brief Returns the particle system data.
        //!
        //! This function returns the particle system data. The data is created when
        //! this solver is constructed and is also owned by the solver.
        //!
        //! \return The Particle System Data.
        const ParticleSystemData2Ptr& ParticleSystemData() const;

        //! Returns the collider
        const Collider2Ptr& Collider() const;

        //! Sets the collider
        void SetCollider(const Collider2Ptr& NewCollider);

        //! Returns the emitter.
        const ParticleEmitter2Ptr& Emitter() const;

        //! Sets the Emitter.
        void SetEmitter(const ParticleEmitter2Ptr& NewEmitter);

        //! Returns the Wind Field
        const VectorField2Ptr& Wind() const;

        //! \brief Sets the wind.
        //!
        //! Wind can be applied to the particle system by setting a vector field to
        //! the solver.
        //!
        //! \param[in] NewWind The Wind Vector
        void SetWind(const VectorField2Ptr& NewWind);

        //! Returns builder for ParticleSystemSolver2
        static Builder builder();
    
    protected:
        //! Initializes the Simulator.
        void OnInitialize() override;

        //! Called to advance a single time-step.
        void OnAdvanceSubTimeStep(double TimeStepInSeconds) override;

        //! Accumulates forces applied to the particles.
        virtual void AccumulateForces(double TimeStepInSeconds);

        //! Called when a time-step is about to begin.
        virtual void OnBeginAdvanceTimeStep(double TimeStepInSeconds);

        //! Called after a time-step is completed.
        virtual void OnEndAdvanceTimeStep(double TimeStepInSeconds);

        //! Resolves any collisions occurred by the particles.
        void ResolveCollision();

        //! Resolves any collisions occurred by the particles where the particle
        //! state is given by the position and velocity arrays.
        void ResolveCollision(ArrayAccessor1<Vector2D> NewPositions, ArrayAccessor1<Vector2D> NewVelocities);

        //! Assign a new particle system data.
        void SetParticleSystemData(const ParticleSystemData2Ptr& NewParticles);
    
    private:
        double _DragCoefficient = 1e-4;
        double _RestitutionCoefficient = 0.0;
        Vector2D _Gravity = Vector2D(0.0, kGravity);

        ParticleSystemData2Ptr _ParticleSystemData;
        ParticleSystemData2::VectorData _NewPositions;
        ParticleSystemData2::VectorData _NewVelocities;
        Collider2Ptr _Collider;
        ParticleEmitter2Ptr _Emitter;
        VectorField2Ptr _Wind;

        void BeginAdvanceTimeStep(double TimeStepInSeconds);

        void EndAdvanceTimeStep(double TimeStepInSeconds);

        void AccumulateExternalForces();

        void TimeIntegration(double TimeStepInSeconds);
        void UpdateCollider(double TimeStepInSeconds);
        void UpdateEmitter(double TimeStepInSeconds);
    };

    typedef std::shared_ptr<ParticleSystemSolver2> ParticleSystemSolver2Ptr;

    //! \brief Base Class for Particle-based Solver Builder
    template<typename DerivedBuilder>
    class ParticleSystemSolverBuilderBase2
    {
    public:
        //! Returns the builder with particle Radius.
        DerivedBuilder& WithRadius(double Radius);

        //! Returns the builder with mass per particle.
        DerivedBuilder& WithMass(double mass);

    protected:
        double _Radius = 1e-3;
        double _Mass = 1e-3;
    };

    template<typename T>
    T& ParticleSystemSolverBuilderBase2<T>::WithRadius(double Radius)
    {
        _Radius = Radius;
        return static_cast<T&>(*this);
    }

    template<typename T>
    T& ParticleSystemSolverBuilderBase2<T>::WithMass(double Mass)
    {
        _Mass = Mass;
        return static_cast<T&>(*this);
    }

    //! \brief Frontend to create ParticleSystemSolver2 instance.
    class ParticleSystemSolver2::Builder final
        : ParticleSystemSolverBuilderBase2<ParticleSystemSolver2::Builder>
    {
    public:
        //! Builds ParticleSystemSolver2
        ParticleSystemSolver2 Build() const;

        //! Builds shared pointer of ParticleSystemSolver2 instance.
        ParticleSystemSolver2Ptr MakeShared() const;
    };
}