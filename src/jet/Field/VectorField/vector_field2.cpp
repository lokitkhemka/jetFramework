#include<jet.h>
#include"vector_field2.h"

namespace jet
{
    VectorField2::VectorField2()
    {}

    VectorField2::~VectorField2()
    {}

    double VectorField2::Divergence(const Vector2D&) const
    {
         return 0.0;
    }

    double VectorField2::Curl(const Vector2D&) const
    {
        return 0.0;
    }

    std::function<Vector2D(const Vector2D&)> VectorField2::Sampler() const
    {
        const VectorField2* self = this;
        return [self](const Vector2D& x) -> Vector2D {
            return self->Sample(x);
        };
    }
}