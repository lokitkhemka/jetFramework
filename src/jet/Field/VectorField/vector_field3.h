#pragma once

#include<Field/field3.h>
#include<Vector/vector3.h>
#include<functional>
#include<memory>

namespace jet
{
    //! Abstract Base Class for 3D vector Field.
    class VectorField3 : public Field3
    {
    public:
        //! Default Constructor
        VectorField3();

        //! Default Destructor
        virtual ~VectorField3();

        //! Returns sampled value at given position \p x.
        virtual Vector3D Sample(const Vector3D& x) const = 0;

        //! Returns Divergence at given position \p x.
        virtual double Divergence(const Vector3D& x) const;

        //! Returns Curl at given position \p x.
        virtual Vector3D Curl(const Vector3D& x) const;

        //! Returns sampler function object
        virtual std::function<Vector3D(const Vector3D&)> Sampler() const;
    };

    typedef std::shared_ptr<VectorField3> VectorField3Ptr;
}