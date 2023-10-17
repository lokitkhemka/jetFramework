#pragma once

#include <Arrays/array1_accessor.h>
#include <IO/Serialization/serialization.h>
#include <Vector/vector2.h>
#include <functional>
#include <memory>
#include <string>
#include <vector>

namespace jet
{
    //! \brief Abstract base class for 2D neighbor point search.
    //!
    //! This class provides interface for 2D neighbor point searcher. For given
    //! list of points, the class builds an internal cache to accelerate the search.
    //! Once the cache is built, the data structure is used to search nearby points
    //! for given origin point.

    class PointNeighborSearch2 : public Serializable
    {
    public:
        //! Callback function for nearby search query. The first parameter is the index of the
        //! nearby point, and the second is the position of the point.
        typedef std::function<void(size_t, const Vector2D&)> ForEachNearbyPointCallback;

        //! Default constructor.
        PointNeighborSearch2();

        //! Destructor
        virtual ~PointNeighborSearch2();

        //! Returns the typename of the derived class.
        virtual std::string TypeName() const = 0;

        //! Builds internal acceleration structure for given points list.
        virtual void Build(const ConstArrayAccessor1<Vector2D>& points) = 0;

        //! Invokes the callback function for each nearby point around the origin
        //! within given radius.
        //! \param[in] origin The origin position.
        //! \param[in] radius The search radius.
        //! \param[in] callback The callback function.
        virtual void ForEachNearbyPoint(const Vector2D& origin, double radius, 
                    const ForEachNearbyPointCallback& callback) const = 0;


        //! Returns true if there are any nearby points for given origin within radius.
        //!
        //! \param[in] origin The origin.
        //! \param[in] radius The radius.
        //!
        //! \return True if has nearby  point, false otherwise.
        virtual bool HasNearbyPoint(const Vector2D& origin, double radius) const = 0;


        //! \brief Creates a new instance of the object with same properties as the original
        //!
        //! \returns Returns the copy
        virtual std::shared_ptr<PointNeighborSearch2> Clone() const = 0;
        
    };

    //! Shared pointer for the PointNeightborSearcher2 type.
    typedef std::shared_ptr<PointNeighborSearch2> PointNeighborSearch2Ptr;


    //! Abstract base calss for 2D Point Neighbor Searcher Builder.
    class PointNeighborSearchBuilder2
    {
    public:
        //! Returns shared pointer of PointNeighborSearcher2 type.
        virtual PointNeighborSearch2Ptr BuildPointNeighborSearch() const = 0;
    };

    //! Shared pointer for the PointNeighborSearcherBuilder2 type.
    typedef std::shared_ptr<PointNeighborSearchBuilder2> PointNeighborSearchBuilder2Ptr;

    #define JET_NEIGHBOR_SEARCH2_TYPE_NAME(DerivedClassName) \
        std::string TypeName() const override { \
            return #DerivedClassName; \
        }
}