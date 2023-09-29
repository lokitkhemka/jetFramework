#pragma once

#include<Geometry/ImplicitSurface/implicit_surface2.h>
#include<memory>

namespace jet
{
    //! \brief 2D implicit Surface wrapper for generic Surface2 instance.
    //!
    //! This class represents 2D implicit surface that converts Surface2 instance to
    //! an ImplicitSurface2 object.
    class SurfaceToImplicit2 final : public ImplicitSurface2
    {
    public:
        class Builder;

        //! Constructs an instance with generic Surface2 instance.
        SurfaceToImplicit2(const Surface2Ptr& surface, const Transform2& transform = Transform2(),
                            bool IsNormalFlipped = false);

        //! Copy constructor
        SurfaceToImplicit2(const SurfaceToImplicit2& other);

        //! Returns the explicit surface instance.
        Surface2Ptr Surface() const;

        //! Returns the builder for SurfaceToImplicit2
        static Builder builder();

    protected:
        Vector2D ClosestPointLocal(const Vector2D& otherPoint) const override;

        double ClosestDistanceLocal(const Vector2D& otherPoint) const override;

        bool IntersectsLocal(const Ray2D& ray) const override;

        BoundingBox2D BoundingBoxLocal() const override;

        Vector2D ClosestNormalLocal(const Vector2D& otherPoint) const override;

        SurfaceRayIntersection2 ClosestIntersectionLocal(const Ray2D& ray) const override;

        double SignedDistanceLocal(const Vector2D& otherPoint) const override;

    private:
        Surface2Ptr _Surface;
    };

    typedef std::shared_ptr<SurfaceToImplicit2> SurfaceToImplicit2Ptr;

    //! \brief Frontend to create SurfaceToImplicit2 objects.
    class SurfaceToImplicit2::Builder final :
                public SurfaceBuilderBase2<SurfaceToImplicit2::Builder>
    {
    public:
        //! Returns builder with surface.
        Builder& WithSurface(const Surface2Ptr& surface);

        //! Builds SurfaceToImplicit2
        SurfaceToImplicit2 Build() const;

        //! Builds shared pointer of SurfaceToImplicit2 instance.
        SurfaceToImplicit2Ptr MakeShared() const;

    private:
        Surface2Ptr _Surface;
    };
}