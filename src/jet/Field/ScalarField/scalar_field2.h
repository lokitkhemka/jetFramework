#pragma once

#include<Field/field2.h>
#include<Vector/vector2.h>
#include<functional>
#include<memory>

namespace jet
{
    //! Abstract Base Class for 2D Scalar Field
    class ScalarField2 : public Field2
    {
    public:
        //! Default Constructor
        ScalarField2();

        //! Default Destructor
        virtual ~ScalarField2();

        //! Returns sampled value at given position \p x.
        virtual double Sample(const Vector2D& x) const = 0;

        //! Returns gradient vector at given position \p x.
        virtual Vector2D Gradient(const Vector2D& x) const;

        //! Returns Laplacian at given position \p x.
        virtual double Laplacian(const Vector2D& x) const;

        //! Returns sampler function object.
        virtual std::function<double(const Vector2D&)> Sampler() const;
    };

    //! Shared Pointer for the ScalarField2 type.
    typedef std::shared_ptr<ScalarField2> ScalarField2Ptr;
}