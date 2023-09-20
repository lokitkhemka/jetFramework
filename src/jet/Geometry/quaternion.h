#pragma once

#include<Matrix/matrix4.h>

namespace jet
{
    //! \brief Quaternion class defined as q = w + xi + yj + zk. where i,j,k are sqrt(-1).

    template<typename T>
    class Quaternion
    {
    public:
        static_assert(std::is_floating_point<T>::value, "Quaternion can only be instantiated with floating point types");

        //! Real Part
        T w;

        //! Imaginary Part (i)
        T x;

        //! Imaginary Part (j)
        T y;

        //! Imaginary Part (k)
        T z;

        //! Make an identity quaternion
        Quaternion();

        //! Constructs a quaternion with given elements.
        Quaternion(T newW, T newX, T newY, T newZ);

        //! Constructs a quaternion with a list \p list.
        Quaternion(const std::initializer_list<T>& list);

        //! Constructs a quaternion with given rotation \p axis and \p angle.
        Quaternion(const Vector3<T>& axis, T angle);

        //! Constructs a quaternion using \p from and \p to vectors.
        Quaternion(const Vector3<T>& from, const Vector3<T>& to);

        //! Constructs a quaternion with three basis vectors.
        Quaternion(const Vector3<T>& axis0, const Vector3<T>& axis1, const Vector3<T>& axis2);

        //! Constructs a quaternion with 3x3 rotational matrix.
        explicit Quaternion(const Matrix3x3<T>& matrix);

        //! Copy Constructor
        Quaternion(const Quaternion& other);



        //! Sets the quaternion with other quanternion
        void Set(const Quaternion& other);

        //! Sets the quaternion with given elements.
        void Set(T newW, T newX, T newY, T newZ);

        //! Sets the quaternion with given element list \p list.
        void Set(const std::initializer_list<T>& list);

        //! Sets the quaternion with given rotation axis and angle.
        void Set(const Vector3<T>& axis, T angle);

        //! Sets the quaternion using \p from and \p to vectors.
        void Set(const Vector3<T>& from, const Vector3<T>& to);

        //! Sets quaternion with three basis vectors.
        void Set(const Vector3<T>& rotationBasis0, const Vector3<T>& rotationBasis1,
                    const Vector3<T>& rotationBasis2);

        //! Sets the quaternion with 3x3 rotational matrix.
        void Set(const Matrix3x3<T>& matrix);


        //! Casts quaternion to other type.
        template<typename U>
        Quaternion<U> CastTo() const;

        //! Returns normalized quaternion.
        Quaternion Normalized() const;

        
        //! Returns this quaternion * vector.
        Vector3<T> Mul(const Vector3<T>& v) const;

        //! Returns quaternion this * other quaternion
        Quaternion Mul(const Quaternion& other) const;

        //! Computes the dot product with other quaterion
        T Dot(const Quaternion<T>& other);

        //! Returns other quaternion * this quaternion.
        Quaternion RMul(const Quaternion& other) const;

        //! Returns this quaternion *= other quaternion
        void IMul(const Quaternion& other);

        
        //! Makes quaternion identity.
        void SetIdentity();

        //! Rotate this quaternion with given \p angle in radians.
        void Rotate(T angleInRadians);

        //! Normalizes the quaternion.
        void Normalize();


        //! Returns the rotational axis.
        Vector3<T> Axis() const;

        //! Returns the rotational angle.
        T Angle() const;

        //! Returns the axis and angle.
        void GetAxisAngle(Vector3<T>* axis, T* angle) const;

        //! Returns the inverse quaternion.
        Quaternion Inverse() const;

        //! Converts to the 3x3 rotation matrix.
        Matrix3x3<T> Matrix3() const;

        //! Converts to the 4x4 rotation matrix.
        Matrix4x4<T> Matrix4() const;

        //! Returns L2 norm of the quaternion.
        T L2Norm() const;


        //! Assigns other quaternion
        Quaternion& operator=(const Quaternion& other);

        //! Returns this quaternion *= other quaternion
        Quaternion& operator*=(const Quaternion& other);


        //! Returns the reference to the \p i-th element.
        T& operator[](size_t i);

        //! Returns the const reference to the \p i-th element.
        const T& operator[](size_t i) const;

        //! Returns true if two quaternions are equal
        bool operator==(const Quaternion& other) const;

        //! Returns false if two quaternions are equal.
        bool operator!=(const Quaternion& other) const;



        //! Returns the identity quaternion.
        static Quaternion MakeIdentity();
    };


    //! Computes spherical linear interpolation.
    template<typename T>
    Quaternion<T> Slerp(const Quaternion<T>& a, const Quaternion<T>& b, T t);

    //! Returns Quaternion q * vector v.
    template<typename T>
    Vector3<T> operator*(const Quaternion<T>& q, const Vector3<T>& v);

    //! Multiplies two quaternions.
    template<typename T>
    Quaternion<T> operator*(const Quaternion<T>& a, const Quaternion<T>& b);

    typedef Quaternion<float> QuaternionF;

    typedef Quaternion<double> QuaternionD;


    /*
        IMPLEMENTATIONS OF METHODS DECLARED ABOVE.
    */

   template<typename T>
   inline Quaternion<T>::Quaternion()
   {
    SetIdentity();
   }

   template<typename T>
   inline Quaternion<T>::Quaternion(T newW, T newX, T newY, T newZ)
   {
    Set(newW, newX, newY, newZ);
   }

   template<typename T>
   inline Quaternion<T>::Quaternion(const std::initializer_list<T>& list)
   {
    Set(list);
   }

   template<typename T>
   inline Quaternion<T>::Quaternion(const Vector3<T>& axis, T angle)
   {
    Set(axis, angle);
   }

   template<typename T>
   inline Quaternion<T>::Quaternion(const Vector3<T>& from, const Vector3<T>& to)
   {
    Set(from, to);
   }

   template<typename T>
   inline Quaternion<T>::Quaternion(const Vector3<T>& rotationBasis0, const Vector3<T>& rotationBasis1,
                                    const Vector3<T>& rotationBasis2)
    {
        Set(rotationBasis0, rotationBasis1, rotationBasis2);
    }

    template<typename T>
    inline Quaternion<T>::Quaternion(const Matrix3x3<T>& matrix)
    {
        Set(matrix);
    }

    template<typename T>
    inline Quaternion<T>::Quaternion(const Quaternion& other)
    {
        Set(other);
    }

    template<typename T>
    inline void Quaternion<T>::Set(const Quaternion& other)
    {
        Set(other.w, other.x, other.y, other.z);
    }

    template<typename T>
    inline void Quaternion<T>::Set(T newW, T newX, T newY, T newZ)
    {
        w = newW;
        x = newX;
        y = newY;
        z = newZ;
    }

    template<typename T>
    inline void Quaternion<T>::Set(const std::initializer_list<T>& list)
    {
        assert(list.size() == 4);

        auto inputElem = list.begin();
        w = *inputElem;
        x = *(++inputElem);
        y = *(++inputElem);
        z = *(++inputElem);
    }

    template<typename T>
    inline void Quaternion<T>::Set(const Vector3<T>& axis, T angle)
    {
        static const T eps = std::numeric_limits<T>::epsilon();

        T axisLengthSquared = axis.LengthSquared();

        if(axisLengthSquared < eps)
        {
            SetIdentity();
        }
        else
        {
            Vector3<T> NormalizedAxis = axis.Normalized();
            T s = std::sin(angle/2);

            x = NormalizedAxis.x * s;
            y = NormalizedAxis.y * s;
            z = NormalizedAxis.z * s;
            w = std::cos(angle/2);
        }
    }


    template<typename T>
    inline void Quaternion<T>::Set(const Vector3<T>& from, const Vector3<T>& to)
    {
        static const T eps = std::numeric_limits<T>::epsilon();

        Vector3<T> axis = from.Cross(to);

        T FromLengthSquared = from.LengthSquared();
        T ToLengthSquared = to.LengthSquared();

        if (FromLengthSquared < eps || ToLengthSquared < eps)
        {
            SetIdentity();
        }
        else
        {
            T AxisLengthSquared = axis.LengthSquared();

            // In case two vectors are exactly opposite, pick orthogonal vector for axis.
            if(AxisLengthSquared < eps)
            {
                axis = std::get<0>(from.Tangential());
            }

            Set(from.Dot(to), axis.x, axis.y, axis.z);
            w+= L2Norm();
            Normalize();
        }
    }

    template<typename T>
    inline void Quaternion<T>::Set(const Vector3<T>& rotationBasis0, const Vector3<T>& rotationBasis1,
                                    const Vector3<T>& rotationBasis2)
    {
        Matrix3x3<T> matrix;

        matrix.SetCol(0, rotationBasis0.Normalized());
        matrix.SetCol(1, rotationBasis1.Normalized());
        matrix.SetCol(2, rotationBasis2.Normalized());

        Set(matrix);
    }

    template<typename T>
    inline void Quaternion<T>::Set(const Matrix3x3<T>& m)
    {
        static const T eps = std::numeric_limits<T>::epsilon();
        static const T quarter = static_cast<T>(0.25);

        T OnePlusTrace = m.Trace() + 1;

        if (OnePlusTrace > eps)
        {
            T S = std::sqrt(OnePlusTrace) * 2;
            w = quarter * S;
            x = (m(2,1) - m(1,2)) / S;
            y = (m(0,2) - m(2,0)) / S;
            z = (m(1,0) - m(0,1)) / S;
        }
        else if(m(0,0) > m(1,1) && m(0,0) > m(2,2))
        {
            T S = std::sqrt(1.0 + m(0,0) - m(1,1) - m(2,2)) * 2;
            w = (m(2,1) - m(1,2)) / S;
            x = quarter * S;
            y = (m(0,1) + m(1,0))/S;
            z = (m(0,2) + m(2,0))/S;
        }
        else if(m(1,1) > m(2,2))
        {
            T S = std::sqrt(1.0 + m(1,1) - m(0,0) - m(2,2)) * 2;
            w = (m(0,2) - m(2,0)) / S;
            x = (m(0,1) - m(1,0)) / S;
            y = quarter * S;
            z = (m(1,2) + m(2,1)) / S;
        }
        else
        {
            T S = std::sqrt(1.0 + m(2,2) - m(0,0) - m(1,1)) * 2;
            w = (m(1,0) - m(0,1)) / S;
            x = (m(0,2) + m(2,0)) / S;
            y = (m(1,2) + m(2,1)) / S;
            z = quarter * S;
        }
    }

    template<typename T>
    template<typename U>
    Quaternion<U> Quaternion<T>::CastTo() const
    {
        return Quaternion<U>(static_cast<U>(w),
                            static_cast<U>(x),
                            static_cast<U>(y),
                            static_cast<U>(z));
    }


    template<typename T>
    Quaternion<T> Quaternion<T>::Normalized() const
    {
        Quaternion q(*this);
        q.Normalize();
        return q;
    }

    template<typename T>
    inline Vector3<T> Quaternion<T>::Mul(const Vector3<T>& v) const
    {
        T _2xx = 2*x*x;
        T _2yy = 2 * y*y;
        T _2zz = 2 * z*z;
        T _2xy = 2 * x*y;
        T _2xz = 2 * x*z;
        T _2xw = 2 * x*w;
        T _2yz = 2 * y*z;
        T _2yw = 2 * y*w;
        T _2zw = 2 * z*w;

        return Vector3<T>(
            (1 - _2yy - _2zz)*v.x + (_2xy - _2zw) * v.y + (_2xz + _2yw) * v.z,
            (_2xy + _2zw) * v.x + (1 - _2zz - _2xx) * v.y + (_2yz - _2xw) * v.z,
            (_2xz - _2yw) * v.x + (_2yz + _2xw) * v.y + (1 - _2yy - _2xx) * v.z
        );
    }

    template<typename T>
    inline Quaternion<T> Quaternion<T>::Mul(const Quaternion& other) const
    {
        return Quaternion(
            w * other.w - x * other.x - y * other.y - z * other.z,
            w * other.x + x * other.w + y * other.z - z * other.y,
            w * other.y - x * other.z + y * other.w + z * other.x,
            w * other.z + x * other.y - y * other.x + z * other.w
        );
    }

    template<typename T>
    inline T Quaternion<T>::Dot(const Quaternion<T>& other)
    {
        return w * other.w + x * other.x + y * other.y + z * other.z;
    }

    template<typename T>
    inline Quaternion<T> Quaternion<T>::RMul(const Quaternion& other) const
    {
        return Quaternion(
            other.w * w - other.x * x - other.y * y - other.z * z,
            other.w * x + other.x * w + other.y * z - other.z * y,
            other.w * y - other.x * z + other.y * w + other.z * x,
            other.w * z + other.x * y - other.y * x + other.z * w
        );
    }

    template<typename T>
    inline void Quaternion<T>::IMul(const Quaternion& other)
    {
        *this = Mul(other);
    }


    template<typename T>
    inline void Quaternion<T>::SetIdentity()
    {
        Set(1, 0, 0, 0);
    }

    template<typename T>
    inline void Quaternion<T>::Rotate(T angleInRadians)
    {
        Vector3<T> axis;
        T currentAngle;

        GetAxisAngle(&axis, &currentAngle);
        currentAngle += angleInRadians;
        Set(axis, currentAngle);
    }

    template<typename T>
    inline void Quaternion<T>::Normalize()
    {
        T norm = L2Norm();

        if (norm > 0)
        {
            w /= norm;
            x /= norm;
            y /= norm;
            z /= norm;
        }
    }

    
    
    template<typename T>
    inline Vector3<T> Quaternion<T>::Axis() const
    {
        Vector3<T> result(x,y,z);
        result.Normalize();

        if (2 * std::acos(w) < Pi<T>())
            return result;
        else
            return -result;
    }


    template<typename T>
    inline T Quaternion<T>::Angle() const
    {
        T result = 2 * std::acos(w);

        if(result < Pi<T>())
            return result;
        else
            return 2 * Pi<T>() - result;
    }

    template<typename T>
    inline void Quaternion<T>::GetAxisAngle(Vector3<T>* axis, T* angle) const
    {
        axis->Set(x, y, z);
        axis->Normalize();
        *angle = 2 * std::acos(w);

        if(*angle > Pi<T>())
        {
            (*axis) = -(*axis);
            *angle = 2 * Pi<T>() - (*angle);
        }
    }

    template<typename T>
    inline Quaternion<T> Quaternion<T>::Inverse() const
    {
        T denom = w * w + x * x + y * y + z * z;
        return Quaternion(w / denom, -x / denom, -y / denom, -z / denom);
    }

    template<typename T>
    inline Matrix3x3<T> Quaternion<T>::Matrix3() const
    {
        T _2xx = 2*x*x;
        T _2yy = 2 * y*y;
        T _2zz = 2 * z*z;
        T _2xy = 2 * x*y;
        T _2xz = 2 * x*z;
        T _2xw = 2 * x*w;
        T _2yz = 2 * y*z;
        T _2yw = 2 * y*w;
        T _2zw = 2 * z*w;

        Matrix3x3<T> m(
            1 - _2yy - _2zz, _2xy - _2zw, _2xz + _2yw,
            _2xy + _2zw, 1 - _2zz - _2xx, _2yz - _2xw,
            _2xz - _2yw, _2yz + _2xw, 1 - _2yy - _2xx);
        return m;
    }

    template <typename T>
    inline Matrix4x4<T> Quaternion<T>::Matrix4() const {
        T _2xx = 2 * x * x;
        T _2yy = 2 * y * y;
        T _2zz = 2 * z * z;
        T _2xy = 2 * x * y;
        T _2xz = 2 * x * z;
        T _2xw = 2 * x * w;
        T _2yz = 2 * y * z;
        T _2yw = 2 * y * w;
        T _2zw = 2 * z * w;

        Matrix4x4<T> m(
            1 - _2yy - _2zz, _2xy - _2zw, _2xz + _2yw, 0,
            _2xy + _2zw, 1 - _2zz - _2xx, _2yz - _2xw, 0,
            _2xz - _2yw, _2yz + _2xw, 1 - _2yy - _2xx, 0,
            0, 0, 0, 1);

        return m;
    }

    template<typename T>
    inline T Quaternion<T>::L2Norm() const
    {
        return std::sqrt(w * w + x * x + y * y + z * z);
    }

    template<typename T>
    inline Quaternion<T>& Quaternion<T>::operator=(const Quaternion& other)
    {
        Set(other);
        return *this;
    }

    template<typename T>
    inline Quaternion<T>& Quaternion<T>::operator*=(const Quaternion& other)
    {
        IMul(other);
        return *this;
    }

    template<typename T>
    inline T& Quaternion<T>::operator[](size_t i)
    {
        return (&w)[i];
    }

    template<typename T>
    inline const T& Quaternion<T>::operator[](size_t i) const
    {
        return (&w)[i];
    }

    template<typename T>
    bool Quaternion<T>::operator==(const Quaternion& other) const
    {
        return w == other.w && x == other.x &&
                y == other.y && z == other.z;
    }

    template<typename T>
    bool Quaternion<T>::operator!=(const Quaternion& other) const
    {
        return w != other.w || x != other.x ||
                y != other.y || z != other.z;
    }

    template<typename T>
    Quaternion<T> Quaternion<T>::MakeIdentity()
    {
        return Quaternion();
    }

    template <typename T>
    inline Quaternion<T> Slerp(
        const Quaternion<T>& a,
        const Quaternion<T>& b,
        T t) {
        static const double threshold = 0.01;
        static const T eps = std::numeric_limits<T>::epsilon();

        T cosHalfAngle = Dot(a, b);
        T weightA, weightB;

        // For better accuracy, return lerp result when a and b are close enough.
        if (1.0 - std::fabs(cosHalfAngle) < threshold) {
            weightA = 1.0 - t;
            weightB = t;
        } else {
            T halfAngle = std::acos(cosHalfAngle);
            T sinHalfAngle = std::sqrt(1 - cosHalfAngle * cosHalfAngle);

            // In case of angle ~ 180, pick middle value.
            // If not, perform slerp.
            if (std::fabs(sinHalfAngle) < eps) {
                weightA = static_cast<T>(0.5);
                weightB = static_cast<T>(0.5);
            } else {
                weightA = std::sin((1 - t) * halfAngle) / sinHalfAngle;
                weightB = std::sin(t * halfAngle) / sinHalfAngle;
            }
        }

        return Quaternion<T>(
            weightA * a.w + weightB * b.w,
            weightA * a.x + weightB * b.x,
            weightA * a.y + weightB * b.y,
            weightA * a.z + weightB * b.z);
    }


    template<typename T>
    inline Vector3<T> operator*(const Quaternion<T>& q, const Vector3<T>& v)
    {
        return q.Mul(v);
    }

    template<typename T>
    inline Quaternion<T> operator*(const Quaternion<T>& a, const Quaternion<T>& b)
    {
        return a.Mul(b);
    }


}