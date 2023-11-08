#pragma once

#include <Arrays/array1.h>
#include <NeighborhoodSearch/point2_neighbor_search.h>
#include <IO/Serialization/serialization.h>

#include <memory>
#include <vector>

namespace flatbuffers
{
    class FlatBufferBuilder;
    template<typename T> struct Offset;
}

namespace jet
{
    namespace fbs
    {
        struct ParticleSystemData2;
    }
}


namespace jet
{
    //! \brief 2D particle system data.
    //!
    //! This class is the key data structure for storing particle system data.
    //! A single particle has position, velocity, and force attributes by default. But
    //! it can also have additional custom scalar or vector attributes.
    class ParticleSystemData2: public Serializable
    {
    public:
        //! Scalar Data chunk
        typedef Array1<double> ScalarData;

        //! Vector Data Chunk
        typedef Array1<Vector2D> VectorData;

        //! Default Constructor
        ParticleSystemData2();

        //! Constructs a particle system data with given number of particles.
        explicit ParticleSystemData2(size_t NumParticles);

        //! Copy Constructor
        ParticleSystemData2(const ParticleSystemData2& other);

        //! Destructor
        virtual ~ParticleSystemData2();

        //! \brief Resizes the number of particles of the container.
        //!
        //! This funciton will resize internal containers to store given number of particles
        //! including custom data layers. However, this will in validate neighbor search and neighbor lists.
        //! It is users' responsibility to call ParticleSystemData2::BuildNeighborSearch and
        //! ParticleSystemData2::BuildNeighborLists to refresh that data.
        //!
        //! \param[in] NewNumParticles New Number of particles
        void Resize(size_t NewNumParticles);

        //! Returns the number of particles.
        size_t NumberOfParticles() const;

        //! \brief Adds a scalar data layer and returns its index.
        //!
        //! This function adds a new scalar data layer to the particle system. It can be used
        //! for adding a scalar attribute, such as temperature to the particles.
        //!
        //! \param[in] InitVal Initial Value of the new scalar data.
        size_t AddScalarData(double InitVal = 0.0);

        //! \brief Adds a vector data layer and returns its index.
        //!
        //! This function adds a new vector data layer to the system. It can be used
        //! for adding a vector attribute, such as velocity, to the particles.
        //!
        //! \param[in] InitVal Initial Value of the new vector data.
        size_t AddVectorData(const Vector2D& InitVal = Vector2D());

        //! Returns the radius of the particles.
        double Radius() const;

        //! Sets the radius of the particles.
        virtual void SetRadius(double NewRadius);

        //! Returns the mass of the particles.
        double Mass() const;

        //! Sets the mass of the particles.
        virtual void SetMass(double NewMass);

        //! Returns the position array (immutable)
        ConstArrayAccessor1<Vector2D> Positions() const;

        //! Returns the position array(mutable)
        ArrayAccessor1<Vector2D> Positions();

        //! Returns the Velocity array
        ConstArrayAccessor1<Vector2D> Velocities() const;

        //! Returns the Velocity array.
        ArrayAccessor1<Vector2D> Velocities();

        //! Returns the force array.
        ConstArrayAccessor1<Vector2D> Forces() const;

        //! Returns the force array.
        ArrayAccessor1<Vector2D> Forces();

        //! Returns the custom scalar data layer at given index.
        ConstArrayAccessor1<double> ScalarDataAt(size_t idx) const;

        //! Returns the custom scalar data layer at given index.
        ArrayAccessor1<double> ScalarDataAt(size_t idx);

        //! Returns the custom vector data layer at given index.
        ConstArrayAccessor1<Vector2D> VectorDataAt(size_t idx) const;

        //! Returns the custom vector data layer at given index.
        ArrayAccessor1<Vector2D> VectorDataAt(size_t idx);

        //! \brief Adds a particle to the data structure.
        //!
        //! This function will add a single particle to the data structure. For Custom data layers,
        //! zeros will be assigned for new particles. However, This will invalidate neighbor search
        //! and neighbor lists. Users must call ParticleSystemData2::BuildNeighborSearch and
        //! ParticleSystemData2::BuildNeighborLists to refresh the data.
        //!
        //! \param[in] NewPos The New Position
        //! \param[in] NewVelocity The New Velocity.
        //! \param[in] NewForce The New Force.
        void AddParticle(const Vector2D& NewPos, const Vector2D& NewVelocity = Vector2D(), 
                            const Vector2D& NewForce = Vector2D());
        

        //! \brief Adds particles to the data structure.
        //!
        //! This function will add particles to the data structure. For custom data layers,
        //! zeros will be assigned for new particles. However, this will invalidate neighbor search
        //! and neighbor lists. Users must call ParticleSystemData2::BuildNeighborSearch and
        //! ParticleSystemData2::BuildNeighborLists to refresh the data.
        //!
        //! \param[in] NewPos The New Position.
        //! \param[in] NewVelocity The New Velocity.
        //! \param[in] NewForce The New Force.
        void AddParticles(const ConstArrayAccessor1<Vector2D>& NewPos,
                    const ConstArrayAccessor1<Vector2D>& NewVelocity = ConstArrayAccessor1<Vector2D>(),
                    const ConstArrayAccessor1<Vector2D>& NewForce = ConstArrayAccessor1<Vector2D>());

        

        //! \brief Returns Neighbor Search Instance.
        //!
        //! This function returns the currently set neighbor search object. By
        //! default, PointParallelHashGridSearch2 si used.
        //!
        //! \return Current Neighbor Search.
        const PointNeighborSearch2Ptr& NeighborSearch() const;

        //! Sets Neighbor Search Instance.
        void SetNeighborSearch(const PointNeighborSearch2Ptr& NewNeighborSearch);


        //! \brief Returns Neighbor lists.
        //!
        //! This function returns neighbor lists which is available after calling
        //! ParticleSystemData2::BuildNeighborLists. Each lists stores
        //! indices of the neighbors.
        //!
        //! \return Neighbor Lists.
        const std::vector<std::vector<size_t>>& NeighborLists() const;

        //! Builds Neighbor Search Instace with given search radius.
        void BuildNeighborSearch(double MaxSearchRadius);

        //! Builds NeighborLists with given search radius.
        void BuildNeighborLists(double MaxSearchRadius);

        //! Serializes the particle system data to the buffer.
        void Serialize(std::vector<uint8_t>* buffer) const override;

        //! Deserializes the particle system data from the buffer.
        void Deserialize(const std::vector<uint8_t>& buffer) override;

        //! Copies from other Particle System data class instance.
        void Set(const ParticleSystemData2& other);

        ParticleSystemData2& operator=(const ParticleSystemData2& other);

    protected:
        void SerializeParticleSystemData(flatbuffers::FlatBufferBuilder* builder,
                                flatbuffers::Offset<fbs::ParticleSystemData2>* fbsParticleSystemData) const;

        void DeserializeParticleSystemData(const fbs::ParticleSystemData2* fbsParticleSystemData);

    private:
        double _Radius = 1e-3;
        double _Mass = 1e-3;
        size_t _NumberOfParticles = 0;
        size_t _PositionIdx;
        size_t _VelocityIdx;
        size_t _ForceIdx;

        std::vector<ScalarData> _ScalarDataList;
        std::vector<VectorData> _VectorDataList;

        PointNeighborSearch2Ptr _NeighborSearch;
        std::vector<std::vector<size_t>> _NeighborLists;
    };

    typedef std::shared_ptr<ParticleSystemData2> ParticleSystemData2Ptr;
}