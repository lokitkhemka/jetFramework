#include <jet.h>
#include "scalar_field3.h"

namespace jet
{
    ScalarField3::ScalarField3()
    {}

    ScalarField3::~ScalarField3()
    {}

    Vector3D ScalarField3::Gradient(const Vector3D&) const
    {
        return Vector3D();
    }

    double ScalarField3::Laplacian(const Vector3D&) const {
        return 0.0;
    }

    std::function<double(const Vector3D&)> ScalarField3::Sampler() const {
        const ScalarField3* self = this;
        return [self](const Vector3D& x) -> double {
            return self->Sample(x);
        };
    }
}