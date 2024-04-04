#pragma once

#include <Geometry/Surface/surface3.h>
#include <functional>

namespace jet
{
    //! \brief Abstract base class for generic collider object.
    //!
    //! This class contains basic interfaces for colliders. Most of the functionalities
    //! are implemented inside this class, except for the member function Collider3::VelocityAt.
    //! This class also let the subclasses to provide a Surface3 instance to define collider surface
    //! using Collider3::SetSurface() function.
    class Collider3
    {
    public:
        //! Callback function type for update calls.
        typedef std::function<void(Collider3*, double, double)> OnBeginUpdateCallback;

        //! Default constructor
        Collider3();

        //! Default Destructor
        virtual ~Collider3();

        //! Returns the velocity of the collider at given \p point
        virtual Vector3D VelocityAt(const Vector3D& point) const = 0;

        //! Resolves collision for given point.
        void ResolveCollision(double radius, double restitutionCoeff, Vector3D* position, Vector3D* velocity);

        //! Returns the friction coefficient.
        double FrictionCoefficient() const;

        //! Sets the friction coefficient.
        void SetFrictionCoefficient(double newFrictionCoefficient);


        //! Returns the surface instance.
        const Surface3Ptr& Surface() const;

        //! Updates the collider state
        void Update(double CurrentTimeInSeconds, double TimeIntervalInSeconds);

        //! Sets the callback function to be called when Collider3::Update function is called
        void SetOnBeginUpdateCallback(const OnBeginUpdateCallback& callback);

    protected:
        //! Internal query result structure.
        struct ColliderQueryResult final
        {
            double Distance;
            Vector3D Point;
            Vector3D Normal;
            Vector3D Velocity;
        };

        //! Assigns the surface instance from the subclass
        void SetSurface(const Surface3Ptr& newSurface);


        //! Outputs closest Point's information
        void GetClosestPoint(const Surface3Ptr& surface, const Vector3D& queryPoint, ColliderQueryResult* result) const;


        //! Returns true if given point is in opposite side of the surface.
        bool IsPenetrating(const ColliderQueryResult& colliderPoint, const Vector3D& position,
                            double radius);
        
    private:
        Surface3Ptr _Surface;
        double _FrictionCoefficient = 0.0;
        OnBeginUpdateCallback _OnUpdateCallback;

    };

    typedef std::shared_ptr<Collider3> Collider3Ptr;
}