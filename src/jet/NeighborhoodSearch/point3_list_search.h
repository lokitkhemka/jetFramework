#pragma once

#include <NeighborhoodSearch/point3_neighbor_search.h>
#include <vector>

namespace jet
{
    //! \brief Simple ad-hoc 3D point search.
    //!
    //! This class implements 3D point search simply by looking up every point in
    //! the list. Thus, this class is not for search involving large number of
    //! points, but only for small set of items.
    class PointListSearch3 final : public PointNeighborSearch3
    {
    public:
        JET_NEIGHBOR_SEARCH3_TYPE_NAME(PointListSearch3)
        class Builder;

        //! Default Constructor
        PointListSearch3();

        //! Copy Constructor
        PointListSearch3(const PointListSearch3& other);

        //! \brief Builds Internal structure for given points list
        //!
        //! For this class, this function simply copies the given point list to the
        //! internal list.
        //!
        //! \param[in] points The points to search.
        void Build(const ConstArrayAccessor1<Vector3D>& points) override;

        //! Invokes the callback function for each nearby point around the
        //! origin within given radius.
        //!
        //! \param[in] origin The origin position.
        //! \param[in] radius The search radius.
        //! \param[in] callback The callback function.
        void ForEachNearbyPoint(const Vector3D& origin, double radius, 
            const ForEachNearbyPointCallback& callback) const override;

        //! Returns true if there are any nearby points for given origin within
        //! the given radius.
        //!
        //! \param[in] origin The origin
        //! \param[in] radius The radius
        //!
        //! \return True if has point, False otherwise
        bool HasNearbyPoint(const Vector3D& origin, double radius) const override;

        //! \brief Creates a new instance of the object with same properties as original
        //!
        //! \return Copy of this object.
        PointNeighborSearch3Ptr Clone() const override;

        //! Assignment operator
        PointListSearch3& operator=(const PointListSearch3& other);

        //! Copy from the other instance.
        void Set(const PointListSearch3& other);

        //! Serializes the neighbor search into the buffer
        void Serialize(std::vector<uint8_t>* buffer) const override;

        //! Deserializes the neighbor serach from the buffer.
        void Deserialize(const std::vector<uint8_t>& buffer) override;

        //! Returns builder for PointListSearch3
        static Builder builder();
    private:
        std::vector<Vector3D> _Points;

    };

    typedef std::shared_ptr<PointListSearch3> PointListSearch3Ptr;

    //! \brief Frontend to create PointListSearch3 objects step by step.
    class PointListSearch3::Builder final
        : public PointNeighborSearchBuilder3
    {
    public:
        //! Builds PointListSearch3 instance.
        PointListSearch3 Build() const;

        //! Builds shared pointer of PointListSearch3 instance.
        PointListSearch3Ptr MakeShared() const;

        //! Returns the shared pointer of PointNeighborSearch3 type.
        PointNeighborSearch3Ptr BuildPointNeighborSearch() const override;
    };
}