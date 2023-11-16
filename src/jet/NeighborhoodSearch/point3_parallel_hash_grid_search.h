#pragma once

#include <NeighborhoodSearch/point3_neighbor_search.h>
#include <Points/point3.h>
#include <Size/size3.h>
#include <vector>

namespace jet
{
    //! \brief Parallel version of hash grid based 3D point search.
    //!
    //! This class implements parallel version of 3D point search by using hash
    //! grid for its internal acceleration data structure. Each point is recorded to
    //! its corresponding bucket where the hashing function is 3D grid mapping.
    class PointParallelHashGridSearch3 final : public PointNeighborSearch3
    {
    public:
        JET_NEIGHBOR_SEARCH3_TYPE_NAME(PointParallelHashGridSearch3)

        class Builder;

        //! \brief Constructs a hash grid with given resolution and grid spacing.
        //!
        //! This constructor takes has grid resolution and its grid spacing as its input
        //! parameters. The grid spacing must be 2x or greater than search radius.
        //!
        //! \param[in] resolution The resolution.
        //! \param[in] gridSpacing The grid Spacing.
        PointParallelHashGridSearch3(const Size3& resolution, double gridSpacing);
        
        //! \brief Constructs a hash grid with given resolution and grid spacing.
        //!
        //! This constructor takes has grid resolution and its grid spacing as its input
        //! parameters. The grid spacing must be 2x or greater than search radius.
        //!
        //! \param[in] resolutionX The resolution x.
        //! \param[in] resolutionY The resolution y.
        //! \param[in] resolutionZ The resolution z
        //! \param[in] gridSpacing The grid Spacing.
        PointParallelHashGridSearch3(size_t resolutionX, size_t resolutionY, size_t resolutionZ, double gridSpacing);

        //! Copy constructor.
        PointParallelHashGridSearch3(const PointParallelHashGridSearch3& other);


        //! \brief Builds internal acceleration structure for given points list.
        //!
        //! This function builds the hash grid for given points in parallel.
        //!
        //! \param[in] points The points to be added.
        void Build(const ConstArrayAccessor1<Vector3D>& points) override;


        //! Invokes the callback function for each nearby point around the origin
        //! within given radius.
        //!
        //! \param[in] origin The origin position.
        //! \param[in] radius The search radius.
        //! \param[in] callback The callback function.
        void ForEachNearbyPoint(const Vector3D& origin, double radius, 
                    const ForEachNearbyPointCallback& callback) const override;


        //! Returns true if there are any nearby points for given origin within radius
        //!
        //! \param[in] origin The origin.
        //! \param[in] radius The radius.
        //!
        //! \return True if has nearby point, false otherwise.
        bool HasNearbyPoint(const Vector3D& origin, double radius) const override;


        //! \brief Returns the hash key list.
        //!
        //! The hash key list maps sorted point index i to its hash key value.
        //! The sorting order is based on the key value itself.
        //!
        //! \return The hash key list.
        const std::vector<size_t>& Keys() const;


        //! \brief Returns the start index table.
        //!
        //! The start index table maps the hash grid bucket index to strating index
        //! of the sorted point list. Consider the hash key list below:
        //!
        //! \code
        //! [5|8|8|10|10|10]
        //! \endcode
        //!
        //! Then StartIndexTable and EndIndexTable should be like:
        //! \code
        //! [.....|0|...|1|..|3|..]
        //! [.....|1|...|3|..|6|..]
        //!       ^5    ^8   ^10
        //! \endcode
        //! so that EndIndexTable[i] - StartIndexTable[i] is the number points
        //! in i-th table bucket.
        //!
        //! \return The Start Index Table.
        const std::vector<size_t>& StartIndexTable() const;

        const std::vector<size_t>& EndIndexTable() const;

        //! \brief Returns the sorted indices of the points.
        //!
        //! When the hash grid is build, it sorts the points in hash key order. But rather than
        //! sorting the original points, this class keeps the shuffled indices of hte points.
        //! This list this function returns maps sorted index i to original index j.
        //!
        //! \return The sorted indices of the points.
        const std::vector<size_t>& SortedIndices() const;

        //! Returns the hash value of given 2D bucket index.
        //!
        //! \param[in] BucketIndex The bucket index.
        //!
        //! \return The hash key from the bucket index.
        size_t GetHashKeyFromBucketIndex(const Point3I& BucketIndex) const;

        //! Gets the bucket index from a point.
        //!
        //! \param[in] position The position of the point.
        //!
        //! \return The Bucket index.
        Point3I GetBucketIndex(const Vector3D& position) const;


        //! \brief Creates a new instance of the object with same properties than original
        //!
        //!
        //! \return Copy of this object.
        PointNeighborSearch3Ptr Clone() const override;


        //! Assignment Operator.
        PointParallelHashGridSearch3& operator=(const PointParallelHashGridSearch3& other);


        //! Copy from the other instalce
        void Set(const PointParallelHashGridSearch3& other);

        //! Serializes the neighbor search into the buffer.
        void Serialize(std::vector<uint8_t>* buffer) const override;

        //! Deserializes the neighbor search from the buffer.
        void Deserialize(const std::vector<uint8_t>& buffer) override;

        //! Returns builder for PointParallelHashGridSearch3
        static Builder builder();

    private:
        // friend class PointParallelHashGridSearch3Tests;

        double _GridSpacing = 1.0;
        Point3I _Resolution = Point3I(1, 1, 1);
        std::vector<Vector3D> _Points;
        std::vector<size_t> _Keys;
        std::vector<size_t> _StartIndexTable;
        std::vector<size_t> _EndIndexTable;
        std::vector<size_t> _SortedIndices;

        size_t GetHashKeyFromPosition(const Vector3D& position) const;
        void GetNearbyKeys(const Vector3D& position, size_t* BucketIndices) const;
    };

    typedef std::shared_ptr<PointParallelHashGridSearch3> PointParallelHashGridSearch3Ptr;


    //! \brief Frontend to create PointParallelHashGridSearch3 object.
    class PointParallelHashGridSearch3::Builder final
        : public PointNeighborSearchBuilder3
    {
    public:
        //! Returns the builder with resolution.
        Builder& WithResolution(const Size3& resolution);

        //! Returns the builder with grid spacing.
        Builder& WithGridSpacing(double gridSpacing);

        //! Builder PointParallelHashGridSearch3 Instance.
        PointParallelHashGridSearch3 Build() const;

        //! Builder shared pointer of PointParallelHashGridSearch3 instance.
        PointParallelHashGridSearch3Ptr MakeShared() const;

        //! Returns the shared pointer of PointNeightSearch2 type.
        PointNeighborSearch3Ptr BuildPointNeighborSearch() const override;

    private:
        Size3 _Resolution{64,64,64};
        double _GridSpacing = 1.0;
    };
}