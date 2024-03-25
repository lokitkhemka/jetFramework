#pragma once

#include <Geometry/ImplicitSurface/implicit_surface2.h>
#include <vector>

namespace jet
{
    //! \brief 2D implicit Surface Set
    //!
    //! This class represents 2D implicit surface set which extends
    //! ImplicitSurface2 by overrideing implicit surface-related queries.
    //! This class can hold a collection of other implicit surface instances.
    class ImplicitSurfaceSet2 final : public ImplicitSurface2
    {
    public:
        class Builder;

        //! Constructs an empty implicit surface set.
        ImplicitSurfaceSet2();

        
        //! Constructs an implicit surface set using list of other surfaces.
        ImplicitSurfaceSet2(const std::vector<ImplicitSurface2Ptr>& surfaces,
                        const Transform2& transform = Transform2(),
                        bool IsNormalFlipped = false);
        
        //! Constructs an implicit surface set using list of other surfaces.
        ImplicitSurfaceSet2(const std::vector<Surface2Ptr>& surfaces,
                            const Transform2& transform = Transform2(),
                            bool IsNormalFlipped = false);
        
        //! Copy Constructor
        ImplicitSurfaceSet2(const ImplicitSurfaceSet2& other);

        //! Returns the number of implicit surfaces.
        size_t NumberOfSurfaces() const;

        //! Returns the i-th implicit surface.
        const ImplicitSurface2Ptr& SurfaceAt(size_t i) const;

        //! Adds an explicit surface instance.
        void AddExplicitSurface(const Surface2Ptr& surface);

        //! Adds an implicit surface instance
        void AddSurface(const ImplicitSurface2Ptr& surface);

        //! Returns builder for ImplicitSurfaceSet2
        static Builder builder();

    private:
        std::vector<ImplicitSurface2Ptr> _Surfaces;

        // Surface2 Implementations
        Vector2D ClosestPointLocal(const Vector2D& otherPoint) const override;

        BoundingBox2D BoundingBoxLocal() const override;

        double ClosestDistanceLocal(const Vector2D& otherPoint) const override;

        bool IntersectsLocal(const Ray2D& ray) const override;

        Vector2D ClosestNormalLocal(const Vector2D& otherPoint) const override;

        SurfaceRayIntersection2 ClosestIntersectionLocal(const Ray2D& ray) const override;

        // ImplicitSurface2 Implementations
        double SignedDistanceLocal(const Vector2D& otherPoint) const override;
    };

    typedef std::shared_ptr<ImplicitSurfaceSet2> ImplicitSurfaceSet2Ptr;

    //! \brief Frontend to create ImplicitSurfaceSet2 objects
    class ImplicitSurfaceSet2::Builder final : public SurfaceBuilderBase2<ImplicitSurfaceSet2::Builder>
    {
    public:
        //! Returns builder with surfaces
        Builder& WithSurfaces(const std::vector<ImplicitSurface2Ptr>& surfaces);

        //! Returns builder with explicit surfaces.
        Builder& WithExplicitSurfaces(const std::vector<Surface2Ptr>& surfaces);

        //! Builds ImplicitSurfaceSet2
        ImplicitSurfaceSet2 Build() const;

        //! Builds shared pointer of ImplicitSurfaceSet2 instance.
        ImplicitSurfaceSet2Ptr MakeShared() const;

    private:
        bool _IsNormalFlipped = false;
        std::vector<ImplicitSurface2Ptr> _Surfaces;
    };
}