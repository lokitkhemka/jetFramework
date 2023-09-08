#include<jet.h>
#include"vector_field3.h"

namespace jet
{
    VectorField3::VectorField3()
    {}

    VectorField3::~VectorField3()
    {}

    double VectorField3::Divergence(const Vector3D&) const
    {
         return 0.0;
    }

    Vector3D VectorField3::Curl(const Vector3D&) const
    {
        return Vector3D();
    }

    std::function<Vector3D(const Vector3D&)> VectorField3::Sampler() const
    {
        const VectorField3* self = this;
        return [self](const Vector3D& x) -> Vector3D {
            return self->Sample(x);
        };
    }
}