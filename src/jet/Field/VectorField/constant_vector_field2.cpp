#include <jet.h>

#include "constant_vector_field2.h"

namespace jet
{
    ConstantVectorField2::ConstantVectorField2(const Vector2D& value):
        _Value(value)
    {}

    Vector2D ConstantVectorField2::Sample(const Vector2D& x) const
    {
        UNUSED_VARAIBLE(x);
        return _Value;
    }

    std::function<Vector2D(const Vector2D&)> ConstantVectorField2::Sampler() const
    {
        return [this](const Vector2D&)->Vector2D
        {
            return _Value;
        };
    }

    ConstantVectorField2::Builder ConstantVectorField2::builder()
    {
        return Builder();
    }

    ConstantVectorField2::Builder& ConstantVectorField2::Builder::WithValue(const Vector2D& value)
    {
        _Value = value;
        return *this;
    }

    ConstantVectorField2 ConstantVectorField2::Builder::Build() const
    {
        return ConstantVectorField2(_Value);
    }

    ConstantVectorField2Ptr ConstantVectorField2::Builder::MakeShared() const
    {
        return std::shared_ptr<ConstantVectorField2>( new ConstantVectorField2(_Value),
                            [](ConstantVectorField2* obj){
                                delete obj;
                            });
    }


}