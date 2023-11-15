#pragma once

#include "point3_neighbor_search.h"
#include <Points/point3.h>
#include <Size/size3.h>

#include <vector>

namespace jet
{
    //! \brief Hash grid based 3D point search.
    //!
    //! This class implements 3D point search by using hash grid for its internal
    //! acceleration data structure. Each point is recorded to its corresponding
    //! bucket where the hashing function is 3D grid mapping.
    class PointHashGridSearch3 final : public PointNeighborSearch3
    {
    public:
        JET_NEIGHBOR_SEARCH3_TYPE_NAME(PointHashGridSearch3)

        class Builder;

        //! \brief Constructs a hash grid with given resolution and grid spacing.
        //!
        //! This constructor takes hash grid resolution and its grid spacing as
        //! its input parameters. The grid spacing must be 2x or greater than
        //! search radius.
        //!
        //! \param[in] resolution The resolution.
        //! \param[in] gridSpacing The grid spacing.
        PointHashGridSearch3(const Size3& resolution, double gridSpacing);

        //! \brief Construct hash grid with given resolution and grid spacing.
        //!
        //! This constructor takes hash grid resolution and its grid spacing as
        //! its input parameters. The grid spacing must be 2x or greater than
        //! search radius.
        //!
        //! \param[in] resolutionX The resolution in X direction
        //! \param[in] resolutionY The resolution in Y direction
        //! \param[in] resolutionZ The resolution in Z direction
        //! \param[in] gridSpacing The Grid Spacing
        PointHashGridSearch3(size_t resolutionX, size_t resolutionY, size_t resolutionZ, double gridSpacing);


        //! Copy Constructor
        PointHashGridSearch3(const PointHashGridSearch3& other);

        //! Builds internal acceleration structure for given points list.
        void Build(const ConstArrayAccessor1<Vector3D>& points) override;


        //! Invokes the callback function for each nearby point around the origin
        //! within given radius.
        //!
        //! \param[in] origin The origin position.
        //! \param[in] radius The search radius.
        //! \param[in] callback The callback function
        void ForEachNearbyPoint(const Vector3D& origin, double radius, 
                    const ForEachNearbyPointCallback& callback) const override;
        

        //! Returns true if there are any nearby points for given origin within radius.
        //!
        //!
        //! \param[in] origin The origin
        //! \param[in] radius The radius.
        //!
        //! \return True if has nearby point, false otherwise.
        bool HasNearbyPoint(const Vector3D& origin, double radius) const override;

        //! \brief Adds a single point to the hash grid.
        //!
        //! This function adds a single point to the hash grid for future queries.
        //! It can be used for a hash grid that is already built by calling function
        //! PointHashGridSearch2::Build.
        //!
        //! \param[in] point The point to be added.
        void Add(const Vector3D& point);


        //! \brief Returns the internal bucket.
        //!
        //! A bucket is a list of point indices that has same hash value. This function
        //! returns the internal bucket struct.
        //!
        //! \return List of buckets.
        const std::vector<std::vector<size_t>>& Buckets() const;

        
        //! Returns the hash value for given 3D bucket index.
        //!
        //! \param[in] BucketIndex The bucket index.
        //!
        //! \return The hash key from bucket index.
        size_t GetHashKeyFromBucketIndex(const Point3I& BucketIndex) const;


        //! Gets the bucket index from a point.
        //!
        //! \param[in] position The position of the point.
        //!
        //! \return The bucket index.
        Point3I GetBucketIndex(const Vector3D& position) const;


        //! \brief Creates a new instance of the object with same properties as original
        //!
        //! \return Copy of this object
        PointNeighborSearch3Ptr Clone() const override;


        //! Assignment operator.
        PointHashGridSearch3& operator=(const PointHashGridSearch3& other);


        //! Copy from the other instance.
        void Set(const PointHashGridSearch3& other);

        //! Serializes the neighbor search into the buffer.
        void Serialize(std::vector<uint8_t>* buffer) const override;

        //! Deserializes the neighbor searcher from the buffer.
        void Deserialize(const std::vector<uint8_t>& buffer) override;


        //! Returns builder for PointHashGridSearch2
        static Builder builder();

    private:
        double _GridSpacing = 1.0;
        Point3I _Resolution = Point3I(1, 1, 1);
        std::vector<Vector3D> _Points;
        std::vector<std::vector<size_t>> _Buckets;

        size_t GetHashKeyFromPosition(const Vector3D& position) const;

        void GetNearbyKeys(const Vector3D& position, size_t* bucketIndices) const;
    };

     //! Shared pointer for the PointHasGridSearch3 type.
    typedef std::shared_ptr<PointHashGridSearch3> PointHashGridSearch3Ptr;


    //! \brief Frontend to create PointHashGridSearch3 objects step by step.
    class PointHashGridSearch3::Builder final
        : public PointNeighborSearchBuilder3
    {
    public:
        //! Returns builder with resolution.
        Builder& WithResolution(const Size3& resolution);

        //! Returns builder with grid spacing.
        Builder& WithGridSpacing(double gridSpacing);

        //! Builds PointHasGridSearch2 isntace.
        PointHashGridSearch3 Build() const;

        //! Builds shared pointer of PointHashGridSearch2 instace.
        PointHashGridSearch3Ptr MakeShared() const;

        //! Returns shared pointer of PointNeighborSearch2 type.
        PointNeighborSearch3Ptr BuildPointNeighborSearch() const override;

    private:
        Size3 _Resolution{64,64,64};
        double _GridSpacing = 1.0;
    };
}