#pragma once

#include <constants.h>
#include <ParticleSim/particle_system_solver2.h>
#include <ParticleSim/SPH/sph_system_data2.h>

namespace jet
{
    //! 2D SPH solver class
    class SPHSolver2 : public ParticleSystemSolver2
    {
    public:
        class Builder;

        //! Constructs a solver with empty particle set.
        SPHSolver2();

        virtual ~SPHSolver2();

        //! Constructs a solver with target density, spacing and relative radius.
        SPHSolver2(double TargetDensity, double TargetSpacing, double RelativeKernelRadius);

        //! Returns the exponent part of the equation of state.
        double EOSExponent() const;

        //! \brief Sets the exponent part of the equation of state.
        //!
        //! This function sets the exponent part of the equation of state. The
        //! value must be greater than 1.0, and smaller inputs will be clamped.
        //! Default is 7.
        void SetEOSExponent ( double newEOSExponent);

        //! Returns the negative pressure scale.
        double NegativePressureScale() const;

        //! \brief Sets the negative pressure scale.
        //!
        //! This function sets the negative pressure scale. By setting the number
        //! between 0 and 1, the solver will scale the effect of negative pressure
        //! which can prevent the clumping of the particles near the surface. Input
        //! value outside 0 and 1 will be clamped within the range. Default is
        //! 0.
        void SetNegativePressureScale(double newNegativePressureScale);

        //! Returns the viscosity coefficient.
        double ViscosityCoefficient() const;

        //! Sets the viscosity coefficient.
        void SetViscosityCoefficient(double newViscosityCoeff);

        //! Returns the pseudo viscosity coefficient.
        double PseudoViscosityCoefficient() const;

        //! \brief Sets the pseudo Viscosity coefficient.
        //!
        //! This function sets the pseudo viscosity coefficient which applies
        //! addition pseudo-physical damping to the system. Default is 10.
        void SetPseudoViscosityCoefficient(double newPseudoViscosityCoefficient);

        //! Returns the speed of sound
        double SpeedOfSound() const;

        //! \brief Sets the speed of sound.
        //!
        //! This function sets the speed of sound which affects the stiffness of the EOS
        //! and the time-step size. Higher value will make EOS stiffer and the
        //! time-step smaller. The input value must be higher than 0.0.
        void SetSpeedOfSound(double newSpeedOfSound);

        //! \brief Multiplier that scales the max allowed time-step.
        //!
        //! This function returns the multiplier that scales the max allowed
        //! time-step. When the scale is 1.0, the time-step is bounded by the speed
        //! of sound and max acceleration.
        double TimeStepLimitScale() const;

        //! \brief Sets the multiplier that scales the max allowed time-step.
        //!
        //! This function sets the multiplier that scales the max allowed time-step.
        //! When the scale is 1.0, the time-step is bounded by the speed of sound
        //! and max acceleration.
        void SetTimeStepLimitScale(double newScale);

        //! Returns the SPH system data.
        SPHSystemData2Ptr SPHSystemData() const;

        //! Returns builder for SPHSolver2
        static Builder builder();

    protected:
        //! Returns the number of sub-timesteps.
        unsigned int NumberOfSubTimeSteps(double TimeIntervalInSeconds) const override;

        //! Accumulate the force to the forces array in the particle system.
        void AccumulateForces(double TimeStepInSeconds) override;

        //! Performs pre-processing step before the simulation.
        void OnBeginAdvanceTimeStep(double TimeStepInSeconds) override;

        //! Performce Post-processing step before the simulation.
        void OnEndAdvanceTimeStep(double TimeStepInSeconds) override;

        //! Accumulates the non-pressure forces to the forces array in the particle system.
        virtual void AccumulateNonPressureForces(double TimeStepInSeconds);
        
        //! Accumulates the pressure force to the forces array in the particle system.
        virtual void AccumulatePressureForce(double TimeStepInSeconds);

        //! Computes the pressure.
        void ComputePressure();

        //! Accumulates the pressure force to the given \p pressureForces array.
        void AccumulatePressureForce(const ConstArrayAccessor1<Vector2D>& positions,
                                const ConstArrayAccessor1<double>& densities,
                                const ConstArrayAccessor1<double>& pressures,
                                ArrayAccessor1<Vector2D> pressureForces);

        //! Accumulates the viscosity force to the forces array in the particle system.
        void AccumulateViscosityForce();

        //! Computes PseudoViscosity.
        void ComputePseudoViscosity(double TimeStepInSeconds);

    private:
        //! Exponent Component of equation of state.
        double _EOSExponent = 7.0;

        //! Negative pressure scaling factor.
        //!  Zero means clamping, One means do nothing
        double _NegativePressureScale = 0.0;

        //! Viscosity Coefficient
        double _ViscosityCoefficient = 0.01;

        //! Pseudo-viscosity coefficient velocity filtering.
        //! This is the minimum for SPH solver which is quite sensitive
        //! to the parameters.
        double _PseudoViscosityCoefficient = 10.0;

        //! Speed of sound in medium to determine the stiffness of the system.
        double _SpeedOfSound = 100.0;

        //! Sclaes the max allowed time-step
        double _TimeStepLimitScale = 1.0;
    };

    typedef std::shared_ptr<SPHSolver2> SPHSolver2Ptr;


    //! \brief Base class for SPH-based fluid solver builder
    template<typename DerivedBuilder>
    class SPHSolverBuilderBase2
    {
    public:
        //! Returns builder with target density
        DerivedBuilder& WithTargetDensity(double targetDensity);

        //! Returns builder with target spacing
        DerivedBuilder& WithTargetSpacing(double targetSpacing);

        //! Returns builder with Relative Radius.
        DerivedBuilder& WithRelativeKernelRadius(double relativeKernelRadius);

    protected:
        double _TargetDensity = kWaterDensity;
        double _TargetSpacing = 0.1;
        double _RelativeKernelRadius = 1.8;
    };

    template <typename T>
    T& SPHSolverBuilderBase2<T>::WithTargetDensity(double targetDensity)
    {
        _TargetDensity = targetDensity;
        return static_cast<T&>(*this);
    }

    template<typename T>
    T& SPHSolverBuilderBase2<T>::WithTargetSpacing(double targetSpacing)
    {
        _TargetSpacing = targetSpacing;
        return static_cast<T&>(*this);
    }

    template<typename T>
    T& SPHSolverBuilderBase2<T>::WithRelativeKernelRadius(double relativeRadius)
    {
        _RelativeKernelRadius = relativeRadius;
        return static_cast<T&>(*this);
    }

    //! \brief Frontend to create SPHSolver2 object instance
    class SPHSolver2::Builder final : public SPHSolverBuilderBase2<SPHSolver2::Builder>
    {
    public:
        //! Builds SPHSolver2
        SPHSolver2 Build() const;

        //! Builds Shared pointer of SPHSolver2 instance
        SPHSolver2Ptr MakeShared() const;
    };

}