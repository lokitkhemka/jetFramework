#pragma once

#include <Field/VectorField/vector_field2.h>
#include <memory.h>

namespace jet
{
    //! 2D constant vector field
    class ConstantVectorField2 final : public VectorField2
    {
    public:
        class Builder;

        //! Constructs a constant vector field with given \p value.
        explicit ConstantVectorField2(const Vector2D& value);

        //! Returns the sampled value at given position \p x.
        Vector2D Sample(const Vector2D& x) const override;

        //! Returns the sampler function.
        std::function<Vector2D(const Vector2D&)> Sampler() const override;

        //! Returns Builder for ConstantVectorField2.
        static Builder builder();
    private:
        Vector2D _Value;
    };

    typedef std::shared_ptr<ConstantVectorField2> ConstantVectorField2Ptr;

    //! \brief Frontend to create ConstantVectorField2 objects
    class ConstantVectorField2::Builder final
    {
    public:
        //! Returns builder with value.
        Builder& WithValue(const Vector2D& value);

        //! Builds ConstantVectorField2
        ConstantVectorField2 Build() const;

        //! Builds a shared pointer of ConstantVectorField2 instance.
        ConstantVectorField2Ptr MakeShared() const;
    private:
        Vector2D _Value{0,0};
    };
}