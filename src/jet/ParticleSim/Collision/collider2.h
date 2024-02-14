#pragma once

#include <Geometry/Surface/surface2.h>
#include <functional>

namespace jet
{
    //! \brief Abstract base class for generic collider object.   
    //!   
    //! This class contains basic interfaces for colliders. Most of the funtionalities
    //! are implemented within this class, except the member function Collider2::VelocityAt.
    //! This class also let the subclasses to provide a Surface2 instance to define collider
    //! surface using Collider2::SetSurface function.
    class Collider2
    {
    public:
        //! \brief Callback function type for update calls.
        //!
        //! This type of callback function will take the collider pointer, current time,
        //! and time interval in seconds.
        typedef std::function<void(Collider2*, double, double)> OnBeginUpdateCallback;

        //! Default Constructor
        Collider2();

        //! Default Destructor
        virtual ~Collider2();

        //! Returns the velocity of the collider at given \p point.
        virtual Vector2D VelocityAt(const Vector2D& point) const = 0;

        //! Resolves the collision for given point.
        //!
        //! \param radius Radius of the Colliding point.
        //! \param restitutionCoefficient Defines the Restitution Coefficient.
        //! \param position Input and output position of the point.
        //! \param velocity Input and output velocity of the point.
        void ResolveCollision(double radius, double restitutionCoefficient, Vector2D* position, Vector2D* velocity);

        //! Returns friction Coefficient
        double FrictionCoefficient() const;

        //! \brief Sets the friction coefficient.
        //!
        //! This function assigns the friction coefficient to the collider. Any
        //! negative inputs will be clamped to zero.
        void SetFrictionCoefficient(double newFrictionCoefficient);

        //!  Returns the surface instance.
        const Surface2Ptr& Surface() const;

        //! Updates the collider state.
        void Update(double CurrentTimeInSeconds, double TimeIntervalInSeconds);

        //! \brief Sets the callback function is to be called when Collider2::Update function is invoked.
        //!
        //! The callback function takes current simulation time in seconds unit. Use
        //! this callback to track any motion or state changes related to this collider.
        //!
        //! \param[in] callback The callback function.
        void SetOnBeginUpdateCallback(const OnBeginUpdateCallback& callback);

    protected:
        //! Internal Query result structure.
        struct ColliderQueryResult final
        {
            double Distance;
            Vector2D Point;
            Vector2D Normal;
            Vector2D Velocity;
        };

        //! Assigns the surface instance from the subclass.
        void SetSurface(const Surface2Ptr& NewSurface);

        //! Outputs closest points information.
        void GetClosestPoint(const Surface2Ptr& surface, const Vector2D& queryPoint, ColliderQueryResult* result) const;


        //! Returns true if given point is in the opposite side of the surface.
        bool IsPenetrating(const ColliderQueryResult& colliderPoint, const Vector2D& position, double radius);

    private:
        Surface2Ptr _Surface;
        double _FrictionCoefficient = 0.0;
        OnBeginUpdateCallback _OnUpdateCallback;
    };

    typedef std::shared_ptr<Collider2> Collider2Ptr;
}
