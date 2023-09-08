#pragma once

#include<Field/field2.h>
#include<Vector/vector2.h>
#include<functional>
#include<memory>

namespace jet
{
    //! Abstract Base Class for 2D vector Field.
    class VectorField2 : public Field2
    {
    public:
        //! Default Constructor
        VectorField2();

        //! Default Destructor
        virtual ~VectorField2();

        //! Returns sampled value at given position \p x.
        virtual Vector2D Sample(const Vector2D& x) const = 0;

        //! Returns Divergence at given position \p x.
        virtual double Divergence(const Vector2D& x) const;

        //! Returns Curl at given position \p x.
        virtual double Curl(const Vector2D& x) const;

        //! Returns sampler function object
        virtual std::function<Vector2D(const Vector2D&)> Sampler() const;
    };

    typedef std::shared_ptr<VectorField2> VectorField2Ptr;
}