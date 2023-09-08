#include<jet.h>
#include "scalar_field2.h"

namespace jet
{
    ScalarField2::ScalarField2(){}

    ScalarField2::~ScalarField2(){}

    Vector2D ScalarField2::Gradient(const Vector2D&) const
    {
        return Vector2D();
    }

    double ScalarField2::Laplacian(const Vector2D&) const
    {
        return 0.0;
    }

    std::function<double(const Vector2D&)> ScalarField2::Sampler() const
    {
        const ScalarField2* self = this;
        return [self](const Vector2D& x) -> double{
            return self-> Sample(x);
        };
    }

}
