#include<jet.h>

#include <parallel.h>
#include <ParticleSim/SPH/sph_kernels2.h>
#include <ParticleSim/SPH/sph_solver2.h>
#include <timer.h>
#include <physics-utils.h>

#include <algorithm>
#include <memory>

namespace jet
{
    static double kTimeStepLimitBySpeedFactor = 0.4;
    static double kTimeStepLimitByForceFactor = 0.25;

    SPHSolver2::SPHSolver2()
    {
        SetParticleSystemData(std::make_shared<SPHSystemData2>());
        SetIsUsingFixedSubTimeSteps(false);
    }

    SPHSolver2::SPHSolver2(double targetDensity, double targetSpacing,
                            double relativeKernelRadius)
    {
        auto SPHParticles = std::make_shared<SPHSystemData2>();
        SetParticleSystemData(SPHParticles);
        SPHParticles->SetTargetDensity(targetDensity);
        SPHParticles->SetTargetSpacing(targetSpacing);
        SPHParticles->SetRelativeKernelRadius(relativeKernelRadius);
        SetIsUsingFixedSubTimeSteps(false);
    }

    SPHSolver2::~SPHSolver2()
    {}

    double SPHSolver2::EOSExponent() const
    {
        return _EOSExponent;
    }

    void SPHSolver2::SetEOSExponent(double newEOSExponent)
    {
        _EOSExponent = std::max(newEOSExponent, 1.0);
    }

    double SPHSolver2::NegativePressureScale() const
    {
        return _NegativePressureScale;
    }

    void SPHSolver2::SetNegativePressureScale(double newNegativePressureScale)
    {
        _NegativePressureScale = Clamp(newNegativePressureScale, 0.0, 1.0);
    }

    double SPHSolver2::ViscosityCoefficient() const
    {
        return _ViscosityCoefficient;
    }

    void SPHSolver2::SetViscosityCoefficient(double newViscosityCoeff)
    {
        _ViscosityCoefficient = std::max(newViscosityCoeff, 0.0);
    }

    double SPHSolver2::PseudoViscosityCoefficient() const
    {
        return _PseudoViscosityCoefficient;
    }

    void SPHSolver2::SetPseudoViscosityCoefficient(double newPseudoViscosityCoeff)
    {
        _PseudoViscosityCoefficient = std::max(newPseudoViscosityCoeff, 0.0);
    }

    double SPHSolver2::SpeedOfSound() const
    {
        return _SpeedOfSound;
    }

    void SPHSolver2::SetSpeedOfSound(double newSpeedOfSound)
    {
        _SpeedOfSound = std::max(newSpeedOfSound, kEpsilonD);
    }

    double SPHSolver2::TimeStepLimitScale() const
    {
        return _TimeStepLimitScale;
    }

    void SPHSolver2::SetTimeStepLimitScale(double newScale)
    {
        _TimeStepLimitScale = std::max(newScale, 0.0);
    }

    SPHSystemData2Ptr SPHSolver2::SPHSystemData() const
    {
        return std::dynamic_pointer_cast<SPHSystemData2>(ParticleSystemData());
    }

    unsigned int SPHSolver2::NumberOfSubTimeSteps(double TimeIntervalInSeconds) const
    {
        auto particles = SPHSystemData();
        size_t numParticles = particles->NumberOfParticles();
        auto f = particles->Forces();

        const double kernelRadius = particles->KernelRadius();
        const double mass = particles->Mass();

        double MaxForceMagnitude = 0.0;

        for(size_t i = 0; i < numParticles; ++i)
        {
            MaxForceMagnitude = std::max(MaxForceMagnitude, f[i].Length());
        }

        double TimeStepLimitBySpeed = kTimeStepLimitBySpeedFactor * kernelRadius / _SpeedOfSound;

        double TimeStepLimitByForce = kTimeStepLimitByForceFactor * std::sqrt(kernelRadius * mass / MaxForceMagnitude);

        double DesiredTimeStep = _TimeStepLimitScale * std::min(TimeStepLimitByForce, TimeStepLimitBySpeed);

        return static_cast<unsigned int>(std::ceil(TimeIntervalInSeconds/ DesiredTimeStep));
    }

    void SPHSolver2::AccumulateForces(double TimeStepInSeconds)
    {
        AccumulateNonPressureForces(TimeStepInSeconds);
        AccumulatePressureForce(TimeStepInSeconds);
    }

    void SPHSolver2::OnBeginAdvanceTimeStep(double TimeStepInSeconds)
    {
        UNUSED_VARAIBLE(TimeStepInSeconds);

        auto particles = SPHSystemData();

        Timer timer;
        particles->BuildNeighborSearch();
        particles->BuildNeighborLists();
        particles->UpdateDensities();

        JET_INFO << "Building neighbor lists and updating densities took "
                << timer.DurationInSeconds()
                << " seconds";
    }

    void SPHSolver2::OnEndAdvanceTimeStep(double TimeStepInSeconds)
    {
        ComputePseudoViscosity(TimeStepInSeconds);

        auto particles = SPHSystemData();
        size_t numParticles = particles->NumberOfParticles();
        auto densities = particles->Densities();

        double maxDensity = 0.0;
        for (size_t i = 0; i < numParticles; ++i)
        {
            maxDensity = std::max(maxDensity, densities[i]);
        }

        JET_INFO << "Max Density: " << maxDensity << " "
                << "Max Density / target density ratio: "
                << maxDensity / particles->TargetDensity();
    }

    void SPHSolver2::AccumulateNonPressureForces(double TimeStepInSeconds)
    {
        ParticleSystemSolver2::AccumulateForces(TimeStepInSeconds);
        AccumulateViscosityForce();
    }

    void SPHSolver2::AccumulatePressureForce(double TimeStepInSeconds)
    {
        UNUSED_VARAIBLE(TimeStepInSeconds);

        auto particles = SPHSystemData();
        auto x = particles->Positions();
        auto d = particles->Densities();
        auto p = particles->Pressures();
        auto f = particles->Forces();

        ComputePressure();
        AccumulatePressureForce(x, d, p, f);
    }

    void SPHSolver2::ComputePressure()
    {
        auto particles = SPHSystemData();
        size_t numParticles = particles->NumberOfParticles();
        auto d = particles->Densities();
        auto p = particles->Pressures();


        const double targetDensity = particles->TargetDensity();
        const double EOSScale = targetDensity * Square(_SpeedOfSound)/_EOSExponent;

        ParallelFor(kZeroSize, numParticles,
                    [&](size_t i)
                    {
                        p[i] = ComputePressureFromEOS(d[i], targetDensity,
                                        EOSScale, EOSExponent(), NegativePressureScale());
        });
    }

    void SPHSolver2::AccumulatePressureForce(const ConstArrayAccessor1<Vector2D>& positions,
                const ConstArrayAccessor1<double>& densities,
                const ConstArrayAccessor1<double>& pressures,
                ArrayAccessor1<Vector2D> pressureForces)
    {
        auto particles = SPHSystemData();
        size_t numParticles = particles->NumberOfParticles();

        const double massSq = Square(particles->Mass());
        const SPHSpikyKernel2 kernel(particles->KernelRadius());

        ParallelFor(kZeroSize, numParticles,
                        [&](size_t i)
                        {
                            const auto& neighbors = particles->NeighborLists()[i];
                            for (size_t j : neighbors)
                            {
                                double dist = positions[i].DistanceTo(positions[j]);

                                if (dist > 0.0)
                                {
                                    Vector2D dir = (positions[j] - positions[i]) / dist;
                                    pressureForces[i] -= massSq * (pressures[i] / (densities[i] * densities[i])
                                                                    + pressures[j] / (densities[j] * densities[j]))
                                                                    * kernel.Gradient(dist, dir);
                                }
                            }
        });
    }

    void SPHSolver2::AccumulateViscosityForce()
    {
        auto particles = SPHSystemData();
        size_t numParticles = particles->NumberOfParticles();
        auto x = particles->Positions();
        auto d = particles->Densities();
        auto v = particles->Velocities();
        auto f = particles->Forces();

        const double massSq = Square(particles->Mass());
        const SPHSpikyKernel2 kernel(particles->KernelRadius());

        ParallelFor(kZeroSize, numParticles,
                    [&](size_t i){
                        const auto& neighbors = particles->NeighborLists()[i];
                        for (size_t j : neighbors)
                        {
                            double dist = x[i].DistanceTo(x[j]);

                            f[i] += ViscosityCoefficient() * massSq
                                    * (v[j] - v[i]) / d[j]
                                    * kernel.SecondDerivative(dist);
                        }
        });
    }

    void SPHSolver2::ComputePseudoViscosity(double TimeStepInSeconds)
    {
        auto particles = SPHSystemData();
        size_t numParticles = particles->NumberOfParticles();
        auto x = particles->Positions();
        auto v = particles->Velocities();
        auto d = particles->Densities();

        const double mass = particles->Mass();
        const SPHSpikyKernel2 kernel(particles->KernelRadius());

        Array1<Vector2D> SmoothedVelocities(numParticles);

        ParallelFor(kZeroSize, numParticles,
            [&](size_t i){
                double weightSum = 0.0;
                Vector2D smoothedVelocity;

                const auto& neighbors = particles->NeighborLists()[i];
                for (size_t j : neighbors)
                {
                    double dist = x[i].DistanceTo(x[j]);
                    double wj = mass / d[j] * kernel(dist);
                    weightSum += wj;
                    smoothedVelocity += wj * v[j];
                }

                double wi = mass / d[i];
                weightSum += wi;
                smoothedVelocity += wi * v[i];

                if (weightSum > 0.0)
                    smoothedVelocity /= weightSum;
                
                SmoothedVelocities[i] = smoothedVelocity;
        });

        double factor = TimeStepInSeconds * _PseudoViscosityCoefficient;

        factor = Clamp(factor, 0.0, 1.0);

        ParallelFor(kZeroSize, numParticles,
                [&](size_t i){
                    v[i] = Lerp(v[i], SmoothedVelocities[i], factor);
        });

    }

    SPHSolver2::Builder SPHSolver2::builder()
    {
        return Builder();
    }

    SPHSolver2 SPHSolver2::Builder::Build() const
    {
        return SPHSolver2(_TargetDensity, _TargetSpacing, _RelativeKernelRadius);
    }

    SPHSolver2Ptr SPHSolver2::Builder::MakeShared() const
    {
        return std::shared_ptr<SPHSolver2>(new SPHSolver2(_TargetDensity, _TargetSpacing, _RelativeKernelRadius),
                    [](SPHSolver2* obj){delete obj;});
    }

}