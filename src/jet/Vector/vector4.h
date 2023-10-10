#pragma once

#include <Vector/vector3.h>
#include<algorithm>
#include<limits>
#include<tuple>

#include<macros.h>
#include<math-utils.h>

namespace jet
{
    //!
//! \brief 4-D vector class.
//!
//! This class defines simple 3-D vector data.
//!
//! \tparam T - Type of the element
//!
template <typename T>
class Vector<T, 4> final {
 public:
    static_assert(
        std::is_floating_point<T>::value,
        "Vector only can be instantiated with floating point types");

    //! X (or the first) component of the vector.
    T x;

    //! Y (or the second) component of the vector.
    T y;

    //! Z (or the third) component of the vector.
    T z;

    //! W component of the vector.
    T w;

    // MARK: Constructors

    //! Constructs default vector (0, 0, 0).
    Vector();

    //! Constructs vector with given parameters \p x, \p y, \p z and \p w.
    Vector(T x, T y, T z, T w);

    //! Constructs vector with a 3-D vector and a scalar.
    Vector(const Vector3<T>& pt, T w);

    //! Constructs vector with initializer list.
    template <typename U>
    Vector(const std::initializer_list<U>& list);

    //! Copy constructor.
    Vector(const Vector& v);

    // MARK: Basic setters

    //! Set all x, y, z and w components to \p s.
    void Set(T s);

    //! Set x, y, z and w components with given parameters.
    void Set(T x, T y, T z, T w);

    //! Set x, y, and z components with given \p pt.x, \p pt.y, \p z and \p w.
    void Set(const Vector3<T>& pt, T z);

    //! Set x, y, z and w components with given initializer list.
    template <typename U>
    void Set(const std::initializer_list<U>& list);

    //! Set x, y, z and w with other vector \p v.
    void Set(const Vector& v);

    //! Set all x, y, z and w to zero.
    void SetZero();

    //! Normalizes this vector.
    void Normalize();

    // MARK: Binary operations: new instance = this (+) v

    //! Computes this + (v, v, v).
    Vector Add(T v) const;

    //! Computes this + (v.x, v.y, v.z).
    Vector Add(const Vector& v) const;

    //! Computes this - (v, v, v).
    Vector Sub(T v) const;

    //! Computes this - (v.x, v.y, v.z).
    Vector Sub(const Vector& v) const;

    //! Computes this * (v, v, v).
    Vector Mul(T v) const;

    //! Computes this * (v.x, v.y, v.z).
    Vector Mul(const Vector& v) const;
    
    //! Computes this / (v, v, v).
    Vector Div(T v) const;

    //! Computes this / (v.x, v.y, v.z).
    Vector Div(const Vector& v) const;

    //! Computes dot product.
    T Dot(const Vector& v) const;

    // MARK: Binary operations: new instance = v (+) this

    //! Computes (v, v, v) - this.
    Vector RSub(T v) const;

    //! Computes (v.x, v.y, v.z) - this.
    Vector RSub(const Vector& v) const;

    //! Computes (v, v, v) / this.
    Vector RDiv(T v) const;

    //! Computes (v.x, v.y, v.z) / this.
    Vector RDiv(const Vector& v) const;

    // MARK: Augmented operations: this (+)= v

    //! Computes this += (v, v, v).
    void IAdd(T v);

    //! Computes this += (v.x, v.y, v.z).
    void IAdd(const Vector& v);

    //! Computes this -= (v, v, v).
    void ISub(T v);

    //! Computes this -= (v.x, v.y, v.z).
    void ISub(const Vector& v);

    //! Computes this *= (v, v, v).
    void IMul(T v);

    //! Computes this *= (v.x, v.y, v.z).
    void IMul(const Vector& v);

    //! Computes this /= (v, v, v).
    void IDiv(T v);

    //! Computes this /= (v.x, v.y, v.z).
    void IDiv(const Vector& v);

    // MARK: Basic getters

    //! Returns const reference to the \p i -th element of the vector.
    const T& At(size_t i) const;

    //! Returns reference to the \p i -th element of the vector.
    T& At(size_t i);

    //! Returns the sum of all the components (i.e. x + y + z).
    T Sum() const;

    //! Returns the average of all the components.
    T Avg() const;

    //! Returns the minimum value among x, y, and z.
    T Min() const;

    //! Returns the maximum value among x, y, and z.
    T Max() const;

    //! Returns the absolute minimum value among x, y, and z.
    T AbsMin() const;

    //! Returns the absolute maximum value among x, y, and z.
    T AbsMax() const;

    //! Returns the index of the dominant axis.
    size_t DominantAxis() const;

    //! Returns the index of the subminant axis.
    size_t SubminantAxis() const;

    //! Returns normalized vector.
    Vector Normalized() const;

    //! Returns the length of the vector.
    T Length() const;

    //! Returns the squared length of the vector.
    T LengthSquared() const;

    //! Returns the distance to the other vector.
    T DistanceTo(const Vector& other) const;

    //! Returns the squared distance to the other vector.
    T DistanceSquaredTo(const Vector& other) const;

    //! Returns a vector with different value type.
    template <typename U>
    Vector<U, 4> CastTo() const;

    //! Returns true if \p other is the same as this vector.
    bool IsEqual(const Vector& other) const;

    //! Returns true if \p other is similar to this vector.
    bool IsSimilar(
        const Vector& other,
        T epsilon = std::numeric_limits<T>::epsilon()) const;

    // MARK: Operators

    //! Returns reference to the \p i -th element of the vector.
    T& operator[](size_t i);

    //! Returns const reference to the \p i -th element of the vector.
    const T& operator[](size_t i) const;

    //! Set x and y components with given initializer list.
    template <typename U>
    Vector& operator=(const std::initializer_list<U>& list);

    //! Set x and y with other vector \p pt.
    Vector& operator=(const Vector& v);

    //! Computes this += (v, v)
    Vector& operator+=(T v);

    //! Computes this += (v.x, v.y)
    Vector& operator+=(const Vector& v);

    //! Computes this -= (v, v)
    Vector& operator-=(T v);

    //! Computes this -= (v.x, v.y)
    Vector& operator-=(const Vector& v);

    //! Computes this *= (v, v)
    Vector& operator*=(T v);

    //! Computes this *= (v.x, v.y)
    Vector& operator*=(const Vector& v);

    //! Computes this /= (v, v)
    Vector& operator/=(T v);

    //! Computes this /= (v.x, v.y)
    Vector& operator/=(const Vector& v);

    //! Returns true if \p other is the same as this vector.
    bool operator==(const Vector& v) const;

    //! Returns true if \p other is the not same as this vector.
    bool operator!=(const Vector& v) const;
};


//! Type alias for three dimensional vector.
template <typename T> using Vector4 = Vector<T, 4>;

//! Positive sign operator.
template <typename T>
Vector4<T> operator+(const Vector4<T>& a);

//! Negative sign operator.
template <typename T>
Vector4<T> operator-(const Vector4<T>& a);

//! Computes (a, a, a) + (b.x, b.y, b.z).
template <typename T>
Vector4<T> operator+(T a, const Vector4<T>& b);

//! Computes (a.x, a.y, a.z) + (b.x, b.y, b.z).
template <typename T>
Vector4<T> operator+(const Vector4<T>& a, const Vector4<T>& b);

//! Computes (a.x, a.y, a.z) - (b, b, b).
template <typename T>
Vector4<T> operator-(const Vector4<T>& a, T b);

//! Computes (a, a, a) - (b.x, b.y, b.z).
template <typename T>
Vector4<T> operator-(T a, const Vector4<T>& b);

//! Computes (a.x, a.y, a.z) - (b.x, b.y, b.z).
template <typename T>
Vector4<T> operator-(const Vector4<T>& a, const Vector4<T>& b);

//! Computes (a.x, a.y, a.z) * (b, b, b).
template <typename T>
Vector4<T> operator*(const Vector4<T>& a, T b);

//! Computes (a, a, a) * (b.x, b.y, b.z).
template <typename T>
Vector4<T> operator*(T a, const Vector4<T>& b);

//! Computes (a.x, a.y, a.z) * (b.x, b.y, b.z).
template <typename T>
Vector4<T> operator*(const Vector4<T>& a, const Vector4<T>& b);

//! Computes (a.x, a.y, a.z) / (b, b, b).
template <typename T>
Vector4<T> operator/(const Vector4<T>& a, T b);

//! Computes (a, a, a) / (b.x, b.y, b.z).
template <typename T>
Vector4<T> operator/(T a, const Vector4<T>& b);

//! Computes (a.x, a.y, a.z) / (b.x, b.y, b.z).
template <typename T>
Vector4<T> operator/(const Vector4<T>& a, const Vector4<T>& b);

//! Returns element-wise min vector.
template <typename T>
Vector4<T> Min(const Vector4<T>& a, const Vector4<T>& b);

//! Returns element-wise max vector.
template <typename T>
Vector4<T> Max(const Vector4<T>& a, const Vector4<T>& b);

//! Returns element-wise clamped vector.
template <typename T>
Vector4<T> Clamp(
    const Vector4<T>& v, const Vector4<T>& low, const Vector4<T>& high);

//! Returns element-wise ceiled vector.
template <typename T>
Vector4<T> Ceil(const Vector4<T>& a);

//! Returns element-wise floored vector.
template <typename T>
Vector4<T> Floor(const Vector4<T>& a);

//! Float-type 3D vector.
typedef Vector4<float> Vector4F;

//! Double-type 3D vector.
typedef Vector4<double> Vector4D;

// MARK: Extensions
//! Returns the type of the value itself.
template <typename T>
struct ScalarType<Vector4<T>> {
    typedef T value;
};

//! Computes monotonic Catmull-Rom interpolation.
template <typename T>
Vector4<T> MonotonicCatmullRom(
    const Vector4<T>& v0,
    const Vector4<T>& v1,
    const Vector4<T>& v2,
    const Vector4<T>& v3,
    T f);

// Constructors
template <typename T>
Vector<T, 4>::Vector() :
    x(0),
    y(0),
    z(0),
    w(0) {
}

template <typename T>
Vector<T, 4>::Vector(T newX, T newY, T newZ, T newW) :
    x(newX),
    y(newY),
    z(newZ),
    w(newW) {
}

template <typename T>
Vector<T, 4>::Vector(const Vector3<T>& pt, T newW) :
    x(pt.x),
    y(pt.y),
    z(pt.z),
    w(newW) {
}

template <typename T>
template <typename U>
Vector<T, 4>::Vector(const std::initializer_list<U>& list) {
    Set(list);
}

template <typename T>
Vector<T, 4>::Vector(const Vector& v) :
    x(v.x),
    y(v.y),
    z(v.z),
    w(v.w) {
}

// Basic setters
template <typename T>
void Vector<T, 4>::Set(T s) {
    x = s;
    y = s;
    z = s;
    w = s;
}

template <typename T>
void Vector<T, 4>::Set(T newX, T newY, T newZ, T newW) {
    x = newX;
    y = newY;
    z = newZ;
    w = newW;
}

template <typename T>
void Vector<T, 4>::Set(const Vector3<T>& pt, T newW) {
    x = pt.x;
    y = pt.y;
    z = pt.z;
    w = newW;
}

template <typename T>
template <typename U>
void Vector<T, 4>::Set(const std::initializer_list<U>& list) {
    assert(list.size() >= 4);

    auto inputElem = list.begin();
    x = static_cast<T>(*inputElem);
    y = static_cast<T>(*(++inputElem));
    z = static_cast<T>(*(++inputElem));
    w = static_cast<T>(*(++inputElem));
}

template <typename T>
void Vector<T, 4>::Set(const Vector& v) {
    x = v.x;
    y = v.y;
    z = v.z;
    w = v.w;
}

template <typename T>
void Vector<T, 4>::SetZero() {
    x = y = z = w = 0;
}

template <typename T>
void Vector<T, 4>::Normalize() {
    T l = Length();
    x /= l;
    y /= l;
    z /= l;
    w /= l;
}

// Binary operators: new instance = this (+) v
template <typename T>
Vector<T, 4> Vector<T, 4>::Add(T v) const {
    return Vector(x + v, y + v, z + v, w + v);
}

template <typename T>
Vector<T, 4> Vector<T, 4>::Add(const Vector& v) const {
    return Vector(x + v.x, y + v.y, z + v.z, w + v.w);
}

template <typename T>
Vector<T, 4> Vector<T, 4>::Sub(T v) const {
    return Vector(x - v, y - v, z - v, w - v);
}

template <typename T>
Vector<T, 4> Vector<T, 4>::Sub(const Vector& v) const {
    return Vector(x - v.x, y - v.y, z - v.z, w - v.w);
}

template <typename T>
Vector<T, 4> Vector<T, 4>::Mul(T v) const {
    return Vector(x * v, y * v, z * v, w * v);
}

template <typename T>
Vector<T, 4> Vector<T, 4>::Mul(const Vector& v) const {
    return Vector(x * v.x, y * v.y, z * v.z, w * v.w);
}

template <typename T>
Vector<T, 4> Vector<T, 4>::Div(T v) const {
    return Vector(x / v, y / v, z / v, w / v);
}

template <typename T>
Vector<T, 4> Vector<T, 4>::Div(const Vector& v) const {
    return Vector(x / v.x, y / v.y, z / v.z, w / v.w);
}

template <typename T>
T Vector<T, 4>::Dot(const Vector& v) const {
    return x * v.x + y * v.y + z * v.z + w * v.w;
}

// Binary operators: new instance = v (+) this
template <typename T>
Vector<T, 4> Vector<T, 4>::RSub(T v) const {
    return Vector(v - x, v - y, v - z, v - w);
}

template <typename T>
Vector<T, 4> Vector<T, 4>::RSub(const Vector& v) const {
    return Vector(v.x - x, v.y - y, v.z - z, v.w - w);
}

template <typename T>
Vector<T, 4> Vector<T, 4>::RDiv(T v) const {
    return Vector(v / x, v / y, v / z, v / w);
}

template <typename T>
Vector<T, 4> Vector<T, 4>::RDiv(const Vector& v) const {
    return Vector(v.x / x, v.y / y, v.z / z, v.w / w);
}


// Augmented operators: this (+)= v
template <typename T>
void Vector<T, 4>::IAdd(T v) {
    x += v;
    y += v;
    z += v;
    w += v;
}

template <typename T>
void Vector<T, 4>::IAdd(const Vector& v) {
    x += v.x;
    y += v.y;
    z += v.z;
    w += v.w;
}

template <typename T>
void Vector<T, 4>::ISub(T v) {
    x -= v;
    y -= v;
    z -= v;
    w -= v;
}

template <typename T>
void Vector<T, 4>::ISub(const Vector& v) {
    x -= v.x;
    y -= v.y;
    z -= v.z;
    w -= v.w;
}

template <typename T>
void Vector<T, 4>::IMul(T v) {
    x *= v;
    y *= v;
    z *= v;
    w *= v;
}

template <typename T>
void Vector<T, 4>::IMul(const Vector& v) {
    x *= v.x;
    y *= v.y;
    z *= v.z;
    w *= v.w;
}

template <typename T>
void Vector<T, 4>::IDiv(T v) {
    x /= v;
    y /= v;
    z /= v;
    w /= v;
}

template <typename T>
void Vector<T, 4>::IDiv(const Vector& v) {
    x /= v.x;
    y /= v.y;
    z /= v.z;
    w /= v.w;
}

// Basic getters
template <typename T>
const T& Vector<T, 4>::At(size_t i) const {
    assert(i < 4);
    return (&x)[i];
}

template <typename T>
T& Vector<T, 4>::At(size_t i) {
    assert(i < 4);
    return (&x)[i];
}

template <typename T>
T Vector<T, 4>::Sum() const {
    return x + y + z + w;
}

template <typename T>
T Vector<T, 4>::Avg() const {
    return (x + y + z + w)/4;
}

template <typename T>
T Vector<T, 4>::Min() const {
    return std::min(std::min(x, y), std::min(z, w));
}

template <typename T>
T Vector<T, 4>::Max() const {
    return std::max(std::max(x, y), std::max(z, w));
}

template <typename T>
T Vector<T, 4>::AbsMin() const {
    return jet::AbsMin(jet::AbsMin(x, y), jet::AbsMin(z, w));
}

template <typename T>
T Vector<T, 4>::AbsMax() const {
    return jet::AbsMax(jet::AbsMax(x, y), jet::AbsMax(z, w));
}

template <typename T>
size_t Vector<T, 4>::DominantAxis() const {
    return (std::fabs(x) > std::fabs(y))
        ? ((std::fabs(x) > std::fabs(z))
            ? ((std::fabs(x) > std::fabs(w)) ? 0 : 3)
            : ((std::fabs(x) > std::fabs(w)) ? 2 : 3))
        : ((std::fabs(y) > std::fabs(z))
            ? ((std::fabs(y) > std::fabs(w)) ? 1 : 3)
            : ((std::fabs(z) > std::fabs(w)) ? 2 : 3));
}

template <typename T>
size_t Vector<T, 4>::SubminantAxis() const {
    return (std::fabs(x) < std::fabs(y))
        ? ((std::fabs(x) < std::fabs(z))
            ? ((std::fabs(x) < std::fabs(w)) ? 0 : 3)
            : ((std::fabs(x) < std::fabs(w)) ? 2 : 3))
        : ((std::fabs(y) < std::fabs(z))
            ? ((std::fabs(y) < std::fabs(w)) ? 1 : 3)
            : ((std::fabs(z) < std::fabs(w)) ? 2 : 3));
}

template <typename T>
Vector<T, 4> Vector<T, 4>::Normalized() const {
    T l = Length();
    return Vector(x / l, y / l, z / l, w / l);
}

template <typename T>
T Vector<T, 4>::Length() const {
    return std::sqrt(x * x + y * y + z * z + w * w);
}

template <typename T>
T Vector<T, 4>::LengthSquared() const {
    return x * x + y * y + z * z + w * w;
}

template <typename T>
T Vector<T, 4>::DistanceTo(const Vector<T, 4>& other) const {
    return Sub(other).Length();
}

template <typename T>
T Vector<T, 4>::DistanceSquaredTo(const Vector<T, 4>& other) const {
    return Sub(other).LengthSquared();
}

template <typename T>
template <typename U>
Vector<U, 4> Vector<T, 4>::CastTo() const {
    return Vector<U, 4>(
        static_cast<U>(x),
        static_cast<U>(y),
        static_cast<U>(z),
        static_cast<U>(w));
}

template <typename T>
bool Vector<T, 4>::IsEqual(const Vector& other) const {
    return x == other.x && y == other.y && z == other.z && w == other.w;
}

template <typename T>
bool Vector<T, 4>::IsSimilar(const Vector& other, T epsilon) const {
    return (std::fabs(x - other.x) < epsilon) &&
        (std::fabs(y - other.y) < epsilon) &&
        (std::fabs(z - other.z) < epsilon) &&
        (std::fabs(w - other.w) < epsilon);
}

// Operators
template <typename T>
T& Vector<T, 4>::operator[](size_t i) {
    assert(i < 4);
    return (&x)[i];
}

template <typename T>
const T& Vector<T, 4>::operator[](size_t i) const {
    assert(i < 4);
    return (&x)[i];
}

template <typename T>
template <typename U>
Vector<T, 4>& Vector<T, 4>::operator=(const std::initializer_list<U>& list) {
    Set(list);
    return (*this);
}

template <typename T>
Vector<T, 4>& Vector<T, 4>::operator=(const Vector& v) {
    Set(v);
    return (*this);
}

template <typename T>
Vector<T, 4>& Vector<T, 4>::operator+=(T v) {
    IAdd(v);
    return (*this);
}

template <typename T>
Vector<T, 4>& Vector<T, 4>::operator+=(const Vector& v) {
    IAdd(v);
    return (*this);
}

template <typename T>
Vector<T, 4>& Vector<T, 4>::operator-=(T v) {
    ISub(v);
    return (*this);
}

template <typename T>
Vector<T, 4>& Vector<T, 4>::operator-=(const Vector& v) {
    ISub(v);
    return (*this);
}

template <typename T>
Vector<T, 4>& Vector<T, 4>::operator*=(T v) {
    IMul(v);
    return (*this);
}

template <typename T>
Vector<T, 4>& Vector<T, 4>::operator*=(const Vector& v) {
    IMul(v);
    return (*this);
}

template <typename T>
Vector<T, 4>& Vector<T, 4>::operator/=(T v) {
    IDiv(v);
    return (*this);
}

template <typename T>
Vector<T, 4>& Vector<T, 4>::operator/=(const Vector& v) {
    IDiv(v);
    return (*this);
}

template <typename T>
bool Vector<T, 4>::operator==(const Vector& v) const {
    return IsEqual(v);
}

template <typename T>
bool Vector<T, 4>::operator!=(const Vector& v) const {
    return !IsEqual(v);
}


template <typename T>
Vector<T, 4> operator+(const Vector<T, 4>& a) {
    return a;
}

template <typename T>
Vector<T, 4> operator-(const Vector<T, 4>& a) {
    return Vector<T, 4>(-a.x, -a.y, -a.z, -a.w);
}

template <typename T>
Vector<T, 4> operator+(const Vector<T, 4>& a, T b) {
    return a.Add(b);
}

template <typename T>
Vector<T, 4> operator+(T a, const Vector<T, 4>& b) {
    return b.Add(a);
}

template <typename T>
Vector<T, 4> operator+(const Vector<T, 4>& a, const Vector<T, 4>& b) {
    return a.Add(b);
}

template <typename T>
Vector<T, 4> operator-(const Vector<T, 4>& a, T b) {
    return a.Sub(b);
}

template <typename T>
Vector<T, 4> operator-(T a, const Vector<T, 4>& b) {
    return b.RSub(a);
}

template <typename T>
Vector<T, 4> operator-(const Vector<T, 4>& a, const Vector<T, 4>& b) {
    return a.Sub(b);
}

template <typename T>
Vector<T, 4> operator*(const Vector<T, 4>& a, T b) {
    return a.Mul(b);
}

template <typename T>
Vector<T, 4> operator*(T a, const Vector<T, 4>& b) {
    return b.Mul(a);
}

template <typename T>
Vector<T, 4> operator*(const Vector<T, 4>& a, const Vector<T, 4>& b) {
    return a.Mul(b);
}

template <typename T>
Vector<T, 4> operator/(const Vector<T, 4>& a, T b) {
    return a.Div(b);
}

template <typename T>
Vector<T, 4> operator/(T a, const Vector<T, 4>& b) {
    return b.RDiv(a);
}

template <typename T>
Vector<T, 4> operator/(const Vector<T, 4>& a, const Vector<T, 3>& b) {
    return a.Div(b);
}

template <typename T>
Vector<T, 4> Min(const Vector<T, 4>& a, const Vector<T, 4>& b) {
    return Vector<T, 4>(
        std::min(a.x, b.x), std::min(a.y, b.y), std::min(a.z, b.z), std::min(a.w, b.w));
}

template <typename T>
Vector<T, 4> Max(const Vector<T, 4>& a, const Vector<T, 4>& b) {
    return Vector<T, 4>(
        std::max(a.x, b.x), std::max(a.y, b.y), std::max(a.z, b.z), std::max(a.w, b.w));
}

template <typename T>
Vector<T, 4> Clamp(
    const Vector<T, 4>& v, const Vector<T, 4>& low, const Vector<T, 4>& high) {
    return Vector<T, 4>(
        Clamp(v.x, low.x, high.x),
        Clamp(v.y, low.y, high.y),
        Clamp(v.z, low.z, high.z),
        Clamp(v.w, low.w, high.w));
}

template <typename T>
Vector<T, 4> Ceil(const Vector<T, 4>& a) {
    return Vector<T, 4>(std::ceil(a.x), std::ceil(a.y), std::ceil(a.z), std::ceil(a.w));
}

template <typename T>
Vector<T, 4> Floor(const Vector<T, 4>& a) {
    return Vector<T, 4>(std::floor(a.x), std::floor(a.y), std::floor(a.z), std::floor(a.w));
}

// Extensions
template <typename T>
Vector<T, 3> MonotonicCatmullRom(
    const Vector<T, 4>& v0,
    const Vector<T, 4>& v1,
    const Vector<T, 4>& v2,
    const Vector<T, 4>& v3,
    T f) {
    static const T two = static_cast<T>(2);
    static const T three = static_cast<T>(3);

    Vector<T, 4> d1 = (v2 - v0) / two;
    Vector<T, 4> d2 = (v3 - v1) / two;
    Vector<T, 4> D1 = v2 - v1;

    if (std::fabs(D1.x) < std::numeric_limits<float>::epsilon() ||
        Sign(D1.x) != Sign(d1.x) ||
        Sign(D1.x) != Sign(d2.x)) {
        d1.x = d2.x = 0;
    }

    if (std::fabs(D1.y) < std::numeric_limits<float>::epsilon() ||
        Sign(D1.y) != Sign(d1.y) ||
        Sign(D1.y) != Sign(d2.y)) {
        d1.y = d2.y = 0;
    }

    if (std::fabs(D1.z) < std::numeric_limits<float>::epsilon() ||
        Sign(D1.z) != Sign(d1.z) ||
        Sign(D1.z) != Sign(d2.z)) {
        d1.z = d2.z = 0;
    }

    if (std::fabs(D1.w) < std::numeric_limits<float>::epsilon() ||
        Sign(D1.w) != Sign(d1.w) ||
        Sign(D1.w) != Sign(d2.w)) {
        d1.w = d2.w = 0;
    }

    Vector<T, 4> a3 = d1 + d2 - two * D1;
    Vector<T, 4> a2 = three * D1 - two * d1 - d2;
    Vector<T, 4> a1 = d1;
    Vector<T, 4> a0 = v1;

    return a3 * Cube(f) + a2 * Square(f) + a1 * f + a0;
}

}