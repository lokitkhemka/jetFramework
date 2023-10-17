#pragma once

#include <NeighborhoodSearch/point2_neighbor_search.h>
#include <Points/point2.h>
#include <Size/size2.h>

#include <vector>

namespace jet
{
    //! \brief Hash Grid Based 2D Point Search Class
    //!
    //! This class implements 2D point search by using hash grid for its internal
    //! acceleration data structure. Each point is recorded to its corresponding
    //! bucket where the hashing function is the 2D grid mapping.
    class PointHashGridSearch2 final : public PointNeighborSearch2
    {
    public:
        JET_NEIGHBOR_SEARCH2_TYPE_NAME(PointHashGridSearch2)
        
        class Builder;

        //! \brief Construct hash grid with given resolution and grid spacing
        //!
        //! This constructor takes hash grid resolution and its grid spacing as
        //! its input parameters. The grid spacing must be 2x or greater than
        //! search radius.
        //!
        //! \param[in] resolution The resolution
        //! \param[in] gridSpacing The grid spacing
        PointHashGridSearch2(const Size2& resolution, double gridSpacing);


        //! \brief Construct hash grid with given resolution and grid spacing.
        //!
        //! This constructor takes hash grid resolution and its grid spacing as
        //! its input parameters. The grid spacing must be 2x or greater than
        //! search radius.
        //!
        //! \param[in] resolutionX The resolution in X direction
        //! \param[in] resolutionY The resolution in Y direction
        //! \param[in] gridSpacing The Grid Spacing
        PointHashGridSearch2(size_t resolutionX, size_t resolutionY, double gridSpacing);


        //! Copy Constructor
        PointHashGridSearch2(const PointHashGridSearch2& other);

        //! Builds internal acceleration structure for given points list.
        void Build(const ConstArrayAccessor1<Vector2D>& points) override;


        //! Invokes the callback function for each nearby point around the origin
        //! within given radius.
        //!
        //! \param[in] origin The origin position.
        //! \param[in] radius The search radius.
        //! \param[in] callback The callback function
        void ForEachNearbyPoint(const Vector2D& origin, double radius, 
                    const ForEachNearbyPointCallback& callback) const override;
        

        //! Returns true if there are any nearby points for given origin within radius.
        //!
        //!
        //! \param[in] origin The origin
        //! \param[in] radius The radius.
        //!
        //! \return True if has nearby point, false otherwise.
        bool HasNearbyPoint(const Vector2D& origin, double radius) const override;

        //! \brief Adds a single point to the hash grid.
        //!
        //! This function adds a single point to the hash grid for future queries.
        //! It can be used for a hash grid that is already built by calling function
        //! PointHashGridSearch2::Build.
        //!
        //! \param[in] point The point to be added.
        void Add(const Vector2D& point);


        //! \brief Returns the internal bucket.
        //!
        //! A bucket is a list of point indices that has same hash value. This function
        //! returns the internal bucket struct.
        //!
        //! \return List of buckets.
        const std::vector<std::vector<size_t>>& Buckets() const;

        
        //! Returns the hash value for given 2D bucket index.
        //!
        //! \param[in] BucketIndex The bucket index.
        //!
        //! \return The hash key from bucket index.
        size_t GetHashKeyFromBucketIndex(const Point2I& BucketIndex) const;


        //! Gets the bucket index from a point.
        //!
        //! \param[in] position The position of the point.
        //!
        //! \return The bucket index.
        Point2I GetBucketIndex(const Vector2D& position) const;


        //! \brief Creates a new instance of the object with same properties as original
        //!
        //! \return Copy of this object
        PointNeighborSearch2Ptr Clone() const override;


        //! Assignment operator.
        PointHashGridSearch2& operator=(const PointHashGridSearch2& other);


        //! Copy from the other instance.
        void Set(const PointHashGridSearch2& other);

        //! Serializes the neighbor search into the buffer.
        void Serialize(std::vector<uint8_t>* buffer) const override;

        //! Deserializes the neighbor searcher from the buffer.
        void Deserialize(const std::vector<uint8_t>& buffer) override;


        //! Returns builder for PointHashGridSearch2
        static Builder builder();

    private:
        double _GridSpacing = 1.0;
        Point2I _Resolution = Point2I(1, 1);
        std::vector<Vector2D> _Points;
        std::vector<std::vector<size_t>> _Buckets;

        size_t GetHashKeyFromPosition(const Vector2D& position) const;

        void GetNearbyKeys(const Vector2D& position, size_t* bucketIndices) const;
    };

    //! Shared pointer for the PointHasGridSearch2 type.
    typedef std::shared_ptr<PointHashGridSearch2> PointHashGridSearch2Ptr;


    //! \brief Frontend to create PointHashGridSearch2 objects step by step.
    class PointHashGridSearch2::Builder final
        : public PointNeighborSearchBuilder2
    {
    public:
        //! Returns builder with resolution.
        Builder& WithResolution(const Size2& resolution);

        //! Returns builder with grid spacing.
        Builder& WithGridSpacing(double gridSpacing);

        //! Builds PointHasGridSearch2 isntace.
        PointHashGridSearch2 Build() const;

        //! Builds shared pointer of PointHashGridSearch2 instace.
        PointHashGridSearch2Ptr MakeShared() const;

        //! Returns shared pointer of PointNeighborSearch2 type.
        PointNeighborSearch2Ptr BuildPointNeighborSearch() const override;

    private:
        Size2 _Resolution{64,64};
        double _GridSpacing = 1.0;
    };
}