#pragma once

#include<jet.h>
#include<Field/field3.h>
#include<Vector/vector3.h>
#include<functional>
#include<memory>

namespace jet
{
    class ScalarField3 : public Field3
    {
    public:
        //! Default Constructor
        ScalarField3();

        //! Default Destructor
        virtual ~ScalarField3();

        //! Returns Sampled value at given position \p x
        virtual double Sample(const Vector3D& x) const = 0;

        //! Returns Gradient vector at given position \p x.
        virtual Vector3D Gradient(const Vector3D& x) const;

        //! Returns Laplacian at given position \p x.
        virtual double Laplacian(const Vector3D& x) const;

        //! Returns Sampler Function object.
        virtual std::function<double(const Vector3D&)> Sampler() const;
    };

    //! Shared Pointer for the ScalarField3 type.
    typedef std::shared_ptr<ScalarField3> ScalarField3Ptr;
}