#pragma once

#include<Geometry/Surface/surface2.h>
#include <vector>

namespace jet
{
    //! \brief A set of 2D surfaces.
    //!
    //! This class represents a collection of 2D surface instances.
    class SurfaceSet2 final : public Surface2
    {
    public:
        class Builder;

        //! Constructs an empty surface set.
        SurfaceSet2();

        //! Constructs with a list of surface instances.
        explicit SurfaceSet2(const std::vector<Surface2Ptr>& others, const Transform2& transform = Transform2(),
                        bool IsNormalFlipped = false);
        
        //! Copy Constructor
        SurfaceSet2(const SurfaceSet2& other);

        //! Returns the Number of Surfaces.
        size_t NumberOfSurfaces() const;

        //! Returns the iTh surface.
        const Surface2Ptr& SurfaceAt(size_t i) const;

        //! Adds a surface instance.
        void AddSurface(const Surface2Ptr& surface);

        //! Returns builder for SurfaceSet2
        static Builder builder();
    private:
        std::vector<Surface2Ptr> _Surfaces;

        Vector2D ClosestPointLocal(const Vector2D& otherPoint) const override;

        BoundingBox2D BoundingBoxLocal() const override;

        double ClosestDistanceLocal(const Vector2D& otherPoint) const override;

        bool IntersectsLocal(const Ray2D& ray) const override;

        Vector2D ClosestNormalLocal(const Vector2D& otherPoint) const override;

        SurfaceRayIntersection2 ClosestIntersectionLocal(const Ray2D& ray) const override;
    };

    //! Shared Pointer for the SurfaceSet2 type
    typedef std::shared_ptr<SurfaceSet2> SurfaceSet2Ptr;

    //! \brief Frontend to create SurfaceSet2 object
    class SurfaceSet2::Builder final : public SurfaceBuilderBase2<SurfaceSet2>
    {
    public:
        //! Returns builder with other surfaces.
        Builder& WithSurfaces(const std::vector<Surface2Ptr>& others);

        //! Builds SurfaceSet2
        SurfaceSet2 Build() const;

        //! Builds shared pointer to SurfaceSet2 instance.
        SurfaceSet2Ptr MakeShared() const;
    private:
        std::vector<Surface2Ptr> _Surfaces;
    };
}