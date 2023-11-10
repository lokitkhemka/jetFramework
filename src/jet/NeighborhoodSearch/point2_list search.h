#pragma once

#include <NeighborhoodSearch/point2_neighbor_search.h>
#include <vector>

namespace jet
{
    //! \brief Simple ad-hoc 2D point search class.
    //!
    //! This class implements 2D point search simply by looking up every point in
    //! the list. This class should not be used in searches involving large number of
    //! points, but only for small set of items.
    class PointListSearch2 final : public PointNeighborSearch2
    {
    public:
        JET_NEIGHBOR_SEARCH2_TYPE_NAME(PointListSearch2)

        class Builder;

        //! Default Constructor.
        PointListSearch2();

        //! Copy Constructor
        PointListSearch2(const PointListSearch2& other);

        //! \brief Builds internal strucutre for given points list
        //!
        //! For this class, this function simply copies the given point list
        //! to the internal list.
        //!
        //! \param[in] points The points to search.
        void Build(const ConstArrayAccessor1<Vector2D>& points) override;

        //! Invokes the callback function for each nearby point around the origin
        //! within the given radius.
        //!
        //! \param[in] origin The origin position.
        //! \param[in] radius The search radius.
        //! \param[in] callback The callback function
        void ForEachNearbyPoint(const Vector2D& origin, double radius,
                        const ForEachNearbyPointCallback& callback) const override;

        //! Returns true if there are any nearby points for given origin within
        //! given radius.
        //!
        //! \param[in] origin The origin
        //! \param[in] radius The radius
        //!
        //! \return True if has nearby points, false otherwise.
        bool HasNearbyPoint(const Vector2D& origin, double radius) const override;


        //! \brief Creates a new instance of the object with the same properties as original
        //!
        //! \return Copy of this object.
        PointNeighborSearch2Ptr Clone() const override;

        //! Assignment operator
        PointListSearch2& operator=(const PointListSearch2& other);

        //! Copy from another instance
        void Set(const PointListSearch2& other);

        //! Serializes the neighbor search into the buffer.
        void Serialize(std::vector<uint8_t>* buffer) const override;

        //! Deserialize the neighbor search from the buffer.
        void Deserialize(const std::vector<uint8_t>& buffer) override;

        //! Returns builder for PointListSearch2
        static Builder builder();

    private:
        std::vector<Vector2D> _Points;
    };

    typedef std::shared_ptr<PointListSearch2> PointListSearch2Ptr;

    //! \brief Frontend to create PointListSearch2 object step by step.
    class PointListSearch2::Builder final
        : public PointNeighborSearchBuilder2
    {
    public:
        //! Builds PointListSearch2 instance.
        PointListSearch2 Build() const;

        //! Builds shared pointer of PointListSearch2 instance.
        PointListSearch2Ptr MakeShared() const;

        //! Returns shared pointer of PointNeighborSearch2 type.
        PointNeighborSearch2Ptr BuildPointNeighborSearch() const override;
    };
}