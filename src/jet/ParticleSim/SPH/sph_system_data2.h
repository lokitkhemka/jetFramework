#pragma once

#include <constants.h>
#include <ParticleSim/particle_system_data2.h>
#include <vector>

namespace jet
{
    //! \brief 2D SPH particle system data.
    //!
    //! This class extends ParticleSystemData2 to specialize the data model for SPH.
    //! It includes density and pressure array as a default particle attribute, and it
    //! also contains SPH utilities such as interpolation operator.
    class SPHSystemData2 : public ParticleSystemData2
    {
    public:
        //! Constructs an empty SPH system.
        SPHSystemData2();

        //! Constructs SPH system data with given number of particles.
        explicit SPHSystemData2(size_t NumberOfParticles);

        //! Copy Constructor
        SPHSystemData2(const SPHSystemData2& other);

        //! Destructor
        virtual ~SPHSystemData2();

        //! \brief Sets the radius
        //!
        //! The radius will be interpreted as target spacing.
        void SetRadius(double newRadius) override;


        //! \brief Sets the mass of a particle
        //!
        //! Setting the mass of a particle will change the target density.
        void SetMass(double newMass) override;

        //! Returns the density const array accessor.
        ConstArrayAccessor1<double> Densities() const;

        //! Returns the non-const Array accessor for density.
        ArrayAccessor1<double> Densities();

        //! Returns the pressure const array accessor.
        ConstArrayAccessor1<double> Pressures() const;

        //! Returns the non-const pressure array accessor.
        ArrayAccessor1<double> Pressures();

        //! \brief Updates the density array with latest particle positions.
        //!
        //! This function updates the density array by recalculating each particle's
        //! latest nearby particles' position.
        //!
        //! \warning The neighbour search must be update (by calling SPHSystemData2::BuildNeighborSearch)
        //! before calling this function.
        void UpdateDensities();

        //! Sets the target density of the particle system.
        void SetTargetDensity(double TargetDensity);

        //! Returns the target density of the particle system.
        double TargetDensity() const;
        
        //! Sets the target particle spacing in meters
        void SetTargetSpacing(double spacing);

        //! Returns the target particle spacing in meters
        double TargetSpacing() const;

        //! \brief Sets the relative kernel radius.
        //!
        //! Sets the relative kernel radius compared to the target particle spacing
        //! (i.e., kernel radius/ target spacing). Once this function is called,
        //! hash grid and density should be updated using UpdateHashGrid() and UpdateDensities()
        void SetRelativeKernelRadius(double RelRadius);

        //! \brief Returns the relative kernel radius (kernel radius/ target spacing)
         double RelativeKernelRadius() const;

        //! Returns the kernel raidus in metres.
        double KernelRadius() const;

        //! Returns the sum of kernel function evaluation for each nearby particle.
        double SumOfKernelsNearby(const Vector2D& position) const;

        //! \brief Returns interpolated value at given origin point.
        //!
        //! Returns interpolated scalar data from the given position using
        //! standar SPH weighted average. The data array should match the particle layout.
        //!
        //! \warning The neighbor search object must be updated by calling
        //! SPHSystemData2::BuildNeighborSearch before calling this function.
        double Interpolate(const Vector2D& origin, const ConstArrayAccessor1<double>& values) const;

        //! \brief Returns interpolated vector value at given origin point.
        //!
        //! Returns interpolated vector data from the given position using
        //! standar SPH weighted average. The data array should match the particle layout.
        //!
        //! \warning The neighbor search object must be updated by calling
        //! SPHSystemData2::BuildNeighborSearch before calling this function.
        Vector2D Interpolate(const Vector2D& origin, const ConstArrayAccessor1<Vector2D>& values) const;

        //! \brief Returns the gradient of the given values at the i-th particle
        //!
        //! \warning The neighbor search object must be updated by calling
        //! SPHSystemData2::BuildNeighborSearch before calling this function.
        Vector2D GradientAt(size_t i, const ConstArrayAccessor1<double>& values) const;

        //! \brief Returns the laplacian of the given values at the i-th particle
        //!
        //! \warning The neighbor search object must be updated by calling
        //! SPHSystemData2::BuildNeighborSearch before calling this function.
        double LaplacianAt(size_t i, const ConstArrayAccessor1<double>& values) const;

        //! \brief Returns the laplacian of the given values at the i-th particle
        //!
        //! \warning The neighbor search object must be updated by calling
        //! SPHSystemData2::BuildNeighborSearch before calling this function.
        Vector2D LaplacianAt(size_t i, const ConstArrayAccessor1<Vector2D>& values) const;

        //! Builds neighbor search instance with kernel radius
        void BuildNeighborSearch();

        //! Builds Neighbor Lists with kernel radius.
        void BuildNeighborLists();

        //! Serializes this SPH system data to the  buffer.
        void Serialize(std::vector<uint8_t>* buffer) const override;

        //! Deserializes the SPH system data from the buffer
        void Deserialize(const std::vector<uint8_t>& buffer) override;

        //! Copies from other SPH System data.
        void Set(const SPHSystemData2& other);

        //! Copies from other SPH system data.
        SPHSystemData2& operator=(const SPHSystemData2& other);
    
    private:
        //! Target density of the particle system in kg/m^2
        double _TargetDensity = kWaterDensity;

        //! Target spacing of this particle system in meters
        double _TargetSpacing = 0.1;

        //! Relative radius of SPH kernel
        double _RelativeRadius = 1.8;

        //! SPH kernel radius in meters.
        double _KernelRadius;

        size_t _PressureIdx;
        size_t _DensityIdx;

        //! Computes the mass based on the target density and spacing.
        void ComputeMass();
    };

    typedef std::shared_ptr<SPHSystemData2> SPHSystemData2Ptr;
}