#include <jet.h>
#include "constant_vector_field3.h"

using namespace jet;

ConstantVectorField3::ConstantVectorField3(const Vector3D& value) :
    _value(value) {
}

Vector3D ConstantVectorField3::Sample(const Vector3D& x) const {
    UNUSED_VARIABLE(x);

    return _value;
}

std::function<Vector3D(const Vector3D&)> ConstantVectorField3::Sampler() const {
    return [this](const Vector3D&) -> Vector3D {
        return _value;
    };
}

ConstantVectorField3::Builder ConstantVectorField3::builder() {
    return Builder();
}


ConstantVectorField3::Builder&
ConstantVectorField3::Builder::WithValue(const Vector3D& value) {
    _value = value;
    return *this;
}

ConstantVectorField3 ConstantVectorField3::Builder::Build() const {
    return ConstantVectorField3(_value);
}

ConstantVectorField3Ptr ConstantVectorField3::Builder::MakeShared() const {
    return std::shared_ptr<ConstantVectorField3>(
        new ConstantVectorField3(_value),
        [] (ConstantVectorField3* obj) {
            delete obj;
        });
}