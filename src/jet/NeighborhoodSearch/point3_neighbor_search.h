#pragma once

#include <Arrays/array1_accessor.h>
#include <IO/Serialization/serialization.h>
#include <Vector/vector3.h>
#include <functional>
#include <memory>
#include <string>
#include <vector>

namespace jet
{
    //! \brief Abstract base class for 3D neighbor point search.
    //!
    //! This class provides interface for 3D neighbor point search. For given
    //! list of points, the class builds internal cache to accelerate the search.
    //! Once built, the data structure is used to search enarby points for given
    //! origin point.
    class PointNeighborSearch3: public Serializable
    {
    public:
        //! Callback funciton for nearby search query. The first parameter is the
        //! index of the nearby point, and the second is the position of the point.
        typedef std::function<void(size_t, const Vector3D&)>
            ForEachNearbyPointCallback;

        //! Default Constructor
        PointNeighborSearch3();

        //! Destructor
        virtual ~PointNeighborSearch3();
        
        //! Returns the type name of the derived classname.
        virtual std::string TypeName() const = 0;

        //! Builds internal acceleration structure for given points list.
        virtual void Build(const ConstArrayAccessor1<Vector3D>& points) = 0;

        //! Invokes the callback function for each nearby point around the origin 
        //! within given radius.
        //! 
        //! \param[in] origin The origin position.
        //! \param[in] radius The radius position.
        //! \param[in] callback The callback function
        virtual void ForEachNearbyPoint(const Vector3D& origin, double radius, 
                const ForEachNearbyPointCallback& callback) const = 0;

        
        //! Returns true if there are any nearby points for givne origin within radius.
        //!
        //! \param[in] origin The origin
        //! \param[in] radius The radius
        //!
        //! \return True if has Nearby point, false otherwise.
        virtual bool HasNearbyPoint(const Vector3D& origin, double radius) const = 0;


        //! \brief Creates a new instance of the object with same properties than original.
        //!
        //! \return Copy of this object.
        virtual std::shared_ptr<PointNeighborSearch3> Clone() const = 0;
    };

    typedef std::shared_ptr<PointNeighborSearch3> PointNeighborSearch3Ptr;

    //! Abstract base class for 3D point Neighbor search builder.
    class PointNeighborSearchBuilder3
    {
    public:
        //! Returns the shared pointer of PointNeighborSearch3 type.
        virtual PointNeighborSearch3Ptr BuildPointNeighborSearch() const = 0;
    };

    typedef std::shared_ptr<PointNeighborSearchBuilder3> PointNeighborSearchBuilder3Ptr;

    #define JET_NEIGHBOR_SEARCH3_TYPE_NAME(DerivedClassName) \
        std::string TypeName() const override { \
            return #DerivedClassName; \
        }
}