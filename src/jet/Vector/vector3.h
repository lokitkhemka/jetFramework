#pragma once

#include <Vector/vector2.h>
#include<algorithm>
#include<limits>
#include<tuple>

#include<macros.h>
#include<math-utils.h>

namespace jet
{
    //!
//! \brief 3-D vector class.
//!
//! This class defines simple 3-D vector data.
//!
//! \tparam T - Type of the element
//!
template <typename T>
class Vector<T, 3> final {
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

    // MARK: Constructors

    //! Constructs default vector (0, 0, 0).
    Vector();

    //! Constructs vector with given parameters \p x, \p y, and \p z.
    Vector(T x, T y, T z);

    //! Constructs vector with a 2-D vector and a scalar.
    Vector(const Vector2<T>& pt, T z);

    //! Constructs vector with initializer list.
    template <typename U>
    Vector(const std::initializer_list<U>& list);

    //! Copy constructor.
    Vector(const Vector& v);

    // MARK: Basic setters

    //! Set all x, y, and z components to \p s.
    void Set(T s);

    //! Set x, y, and z components with given parameters.
    void Set(T x, T y, T z);

    //! Set x, y, and z components with given \p pt.x, \p pt.y, and \p z.
    void Set(const Vector2<T>& pt, T z);

    //! Set x, y, and z components with given initializer list.
    template <typename U>
    void Set(const std::initializer_list<U>& lst);

    //! Set x, y, and z with other vector \p v.
    void Set(const Vector& v);

    //! Set all x, y, and z to zero.
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

    //! Comptues cross product.
    Vector Cross(const Vector& v) const;

    // MARK: Binary operations: new instance = v (+) this

    //! Computes (v, v, v) - this.
    Vector RSub(T v) const;

    //! Computes (v.x, v.y, v.z) - this.
    Vector RSub(const Vector& v) const;

    //! Computes (v, v, v) / this.
    Vector RDiv(T v) const;

    //! Computes (v.x, v.y, v.z) / this.
    Vector RDiv(const Vector& v) const;

    //! Computes \p v cross this.
    Vector RCross(const Vector& v) const;

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

    //! Returns the reflection vector to the surface with given surface normal.
    Vector Reflected(const Vector& normal) const;

    //! Returns the projected vector to the surface with given surface normal.
    Vector Projected(const Vector& normal) const;

    //! Returns the tangential vector for this vector.
    std::tuple<Vector, Vector> Tangential() const;

    //! Returns a vector with different value type.
    template <typename U>
    Vector<U, 3> CastTo() const;

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
    Vector& operator=(const std::initializer_list<U>& lst);

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
template <typename T> using Vector3 = Vector<T, 3>;

//! Positive sign operator.
template <typename T>
Vector3<T> operator+(const Vector3<T>& a);

//! Negative sign operator.
template <typename T>
Vector3<T> operator-(const Vector3<T>& a);

//! Computes (a, a, a) + (b.x, b.y, b.z).
template <typename T>
Vector3<T> operator+(T a, const Vector3<T>& b);

//! Computes (a.x, a.y, a.z) + (b.x, b.y, b.z).
template <typename T>
Vector3<T> operator+(const Vector3<T>& a, const Vector3<T>& b);

//! Computes (a.x, a.y, a.z) - (b, b, b).
template <typename T>
Vector3<T> operator-(const Vector3<T>& a, T b);

//! Computes (a, a, a) - (b.x, b.y, b.z).
template <typename T>
Vector3<T> operator-(T a, const Vector3<T>& b);

//! Computes (a.x, a.y, a.z) - (b.x, b.y, b.z).
template <typename T>
Vector3<T> operator-(const Vector3<T>& a, const Vector3<T>& b);

//! Computes (a.x, a.y, a.z) * (b, b, b).
template <typename T>
Vector3<T> operator*(const Vector3<T>& a, T b);

//! Computes (a, a, a) * (b.x, b.y, b.z).
template <typename T>
Vector3<T> operator*(T a, const Vector3<T>& b);

//! Computes (a.x, a.y, a.z) * (b.x, b.y, b.z).
template <typename T>
Vector3<T> operator*(const Vector3<T>& a, const Vector3<T>& b);

//! Computes (a.x, a.y, a.z) / (b, b, b).
template <typename T>
Vector3<T> operator/(const Vector3<T>& a, T b);

//! Computes (a, a, a) / (b.x, b.y, b.z).
template <typename T>
Vector3<T> operator/(T a, const Vector3<T>& b);

//! Computes (a.x, a.y, a.z) / (b.x, b.y, b.z).
template <typename T>
Vector3<T> operator/(const Vector3<T>& a, const Vector3<T>& b);

//! Returns element-wise min vector.
template <typename T>
Vector3<T> Min(const Vector3<T>& a, const Vector3<T>& b);

//! Returns element-wise max vector.
template <typename T>
Vector3<T> Max(const Vector3<T>& a, const Vector3<T>& b);

//! Returns element-wise clamped vector.
template <typename T>
Vector3<T> Clamp(
    const Vector3<T>& v, const Vector3<T>& low, const Vector3<T>& high);

//! Returns element-wise ceiled vector.
template <typename T>
Vector3<T> Ceil(const Vector3<T>& a);

//! Returns element-wise floored vector.
template <typename T>
Vector3<T> Floor(const Vector3<T>& a);

//! Float-type 3D vector.
typedef Vector3<float> Vector3F;

//! Double-type 3D vector.
typedef Vector3<double> Vector3D;

// MARK: Extensions
//! Returns the type of the value itself.
template <typename T>
struct ScalarType<Vector3<T>> {
    typedef T value;
};

//! Computes monotonic Catmull-Rom interpolation.
template <typename T>
Vector3<T> MonotonicCatmullRom(
    const Vector3<T>& v0,
    const Vector3<T>& v1,
    const Vector3<T>& v2,
    const Vector3<T>& v3,
    T f);

// Constructors
template <typename T>
Vector<T, 3>::Vector() :
    x(0),
    y(0),
    z(0) {
}

template <typename T>
Vector<T, 3>::Vector(T newX, T newY, T newZ) :
    x(newX),
    y(newY),
    z(newZ) {
}

template <typename T>
Vector<T, 3>::Vector(const Vector2<T>& pt, T newZ) :
    x(pt.x),
    y(pt.y),
    z(newZ) {
}

template <typename T>
template <typename U>
Vector<T, 3>::Vector(const std::initializer_list<U>& list) {
    Set(list);
}

template <typename T>
Vector<T, 3>::Vector(const Vector& v) :
    x(v.x),
    y(v.y),
    z(v.z) {
}

// Basic setters
template <typename T>
void Vector<T, 3>::Set(T s) {
    x = s;
    y = s;
    z = s;
}

template <typename T>
void Vector<T, 3>::Set(T newX, T newY, T newZ) {
    x = newX;
    y = newY;
    z = newZ;
}

template <typename T>
void Vector<T, 3>::Set(const Vector2<T>& pt, T newZ) {
    x = pt.x;
    y = pt.y;
    z = newZ;
}

template <typename T>
template <typename U>
void Vector<T, 3>::Set(const std::initializer_list<U>& list) {
    assert(list.size() >= 3);

    auto inputElem = list.begin();
    x = static_cast<T>(*inputElem);
    y = static_cast<T>(*(++inputElem));
    z = static_cast<T>(*(++inputElem));
}

template <typename T>
void Vector<T, 3>::Set(const Vector& v) {
    x = v.x;
    y = v.y;
    z = v.z;
}

template <typename T>
void Vector<T, 3>::SetZero() {
    x = y = z = 0;
}

template <typename T>
void Vector<T, 3>::Normalize() {
    T l = Length();
    x /= l;
    y /= l;
    z /= l;
}

// Binary operators: new instance = this (+) v
template <typename T>
Vector<T, 3> Vector<T, 3>::Add(T v) const {
    return Vector(x + v, y + v, z + v);
}

template <typename T>
Vector<T, 3> Vector<T, 3>::Add(const Vector& v) const {
    return Vector(x + v.x, y + v.y, z + v.z);
}

template <typename T>
Vector<T, 3> Vector<T, 3>::Sub(T v) const {
    return Vector(x - v, y - v, z - v);
}

template <typename T>
Vector<T, 3> Vector<T, 3>::Sub(const Vector& v) const {
    return Vector(x - v.x, y - v.y, z - v.z);
}

template <typename T>
Vector<T, 3> Vector<T, 3>::Mul(T v) const {
    return Vector(x * v, y * v, z * v);
}

template <typename T>
Vector<T, 3> Vector<T, 3>::Mul(const Vector& v) const {
    return Vector(x * v.x, y * v.y, z * v.z);
}

template <typename T>
Vector<T, 3> Vector<T, 3>::Div(T v) const {
    return Vector(x / v, y / v, z / v);
}

template <typename T>
Vector<T, 3> Vector<T, 3>::Div(const Vector& v) const {
    return Vector(x / v.x, y / v.y, z / v.z);
}

template <typename T>
T Vector<T, 3>::Dot(const Vector& v) const {
    return x * v.x + y * v.y + z * v.z;
}

template <typename T>
Vector<T, 3> Vector<T, 3>::Cross(const Vector& v) const {
    return Vector(y*v.z - v.y*z, z*v.x - v.z*x, x*v.y - v.x*y);
}

// Binary operators: new instance = v (+) this
template <typename T>
Vector<T, 3> Vector<T, 3>::RSub(T v) const {
    return Vector(v - x, v - y, v - z);
}

template <typename T>
Vector<T, 3> Vector<T, 3>::RSub(const Vector& v) const {
    return Vector(v.x - x, v.y - y, v.z - z);
}

template <typename T>
Vector<T, 3> Vector<T, 3>::RDiv(T v) const {
    return Vector(v / x, v / y, v / z);
}

template <typename T>
Vector<T, 3> Vector<T, 3>::RDiv(const Vector& v) const {
    return Vector(v.x / x, v.y / y, v.z / z);
}

template <typename T>
Vector<T, 3> Vector<T, 3>::RCross(const Vector& v) const {
    return Vector(v.y*z - y*v.z, v.z*x - z*v.x, v.x*y - x*v.y);
}

// Augmented operators: this (+)= v
template <typename T>
void Vector<T, 3>::IAdd(T v) {
    x += v;
    y += v;
    z += v;
}

template <typename T>
void Vector<T, 3>::IAdd(const Vector& v) {
    x += v.x;
    y += v.y;
    z += v.z;
}

template <typename T>
void Vector<T, 3>::ISub(T v) {
    x -= v;
    y -= v;
    z -= v;
}

template <typename T>
void Vector<T, 3>::ISub(const Vector& v) {
    x -= v.x;
    y -= v.y;
    z -= v.z;
}

template <typename T>
void Vector<T, 3>::IMul(T v) {
    x *= v;
    y *= v;
    z *= v;
}

template <typename T>
void Vector<T, 3>::IMul(const Vector& v) {
    x *= v.x;
    y *= v.y;
    z *= v.z;
}

template <typename T>
void Vector<T, 3>::IDiv(T v) {
    x /= v;
    y /= v;
    z /= v;
}

template <typename T>
void Vector<T, 3>::IDiv(const Vector& v) {
    x /= v.x;
    y /= v.y;
    z /= v.z;
}

// Basic getters
template <typename T>
const T& Vector<T, 3>::At(size_t i) const {
    assert(i < 3);
    return (&x)[i];
}

template <typename T>
T& Vector<T, 3>::At(size_t i) {
    assert(i < 3);
    return (&x)[i];
}

template <typename T>
T Vector<T, 3>::Sum() const {
    return x + y + z;
}

template <typename T>
T Vector<T, 3>::Avg() const {
    return (x + y + z)/3;
}

template <typename T>
T Vector<T, 3>::Min() const {
    return std::min(std::min(x, y), z);
}

template <typename T>
T Vector<T, 3>::Max() const {
    return std::max(std::max(x, y), z);
}

template <typename T>
T Vector<T, 3>::AbsMin() const {
    return jet::AbsMin(jet::AbsMin(x, y), z);
}

template <typename T>
T Vector<T, 3>::AbsMax() const {
    return jet::AbsMax(jet::AbsMax(x, y), z);
}

template <typename T>
size_t Vector<T, 3>::DominantAxis() const {
    return (std::fabs(x) > std::fabs(y))
        ? ((std::fabs(x) > std::fabs(z)) ? 0 : 2)
        : ((std::fabs(y) > std::fabs(z)) ? 1 : 2);
}

template <typename T>
size_t Vector<T, 3>::SubminantAxis() const {
    return (std::fabs(x) < std::fabs(y))
        ? ((std::fabs(x) < std::fabs(z)) ? 0 : 2)
        : ((std::fabs(y) < std::fabs(z)) ? 1 : 2);
}

template <typename T>
Vector<T, 3> Vector<T, 3>::Normalized() const {
    T l = Length();
    return Vector(x / l, y / l, z / l);
}

template <typename T>
T Vector<T, 3>::Length() const {
    return std::sqrt(x * x + y * y + z * z);
}

template <typename T>
T Vector<T, 3>::LengthSquared() const {
    return x * x + y * y + z * z;
}

template <typename T>
T Vector<T, 3>::DistanceTo(const Vector<T, 3>& other) const {
    return Sub(other).Length();
}

template <typename T>
T Vector<T, 3>::DistanceSquaredTo(const Vector<T, 3>& other) const {
    return Sub(other).LengthSquared();
}

template <typename T>
Vector<T, 3> Vector<T, 3>::Reflected(const Vector<T, 3>& normal) const {
    // this - 2(this.n)n
    return Sub(normal.Mul(2 * Dot(normal)));
}

template <typename T>
Vector<T, 3> Vector<T, 3>::Projected(const Vector<T, 3>& normal) const {
    // this - this.n n
    return Sub(normal.Mul(Dot(normal)));
}

template <typename T>
std::tuple<Vector<T, 3>, Vector<T, 3>> Vector<T, 3>::Tangential() const {
    Vector<T, 3> a
        = ((std::fabs(y) > 0 || std::fabs(z) > 0)
        ? Vector<T, 3>(1, 0, 0)
        : Vector<T, 3>(0, 1, 0)).Cross(*this).Normalized();
    Vector<T, 3> b = Cross(a);
    return std::make_tuple(a, b);
}

template <typename T>
template <typename U>
Vector<U, 3> Vector<T, 3>::CastTo() const {
    return Vector<U, 3>(
        static_cast<U>(x),
        static_cast<U>(y),
        static_cast<U>(z));
}

template <typename T>
bool Vector<T, 3>::IsEqual(const Vector& other) const {
    return x == other.x && y == other.y && z == other.z;
}

template <typename T>
bool Vector<T, 3>::IsSimilar(const Vector& other, T epsilon) const {
    return (std::fabs(x - other.x) < epsilon) &&
        (std::fabs(y - other.y) < epsilon) &&
        (std::fabs(z - other.z) < epsilon);
}

// Operators
template <typename T>
T& Vector<T, 3>::operator[](size_t i) {
    assert(i < 3);
    return (&x)[i];
}

template <typename T>
const T& Vector<T, 3>::operator[](size_t i) const {
    assert(i < 3);
    return (&x)[i];
}

template <typename T>
template <typename U>
Vector<T, 3>& Vector<T, 3>::operator=(const std::initializer_list<U>& list) {
    Set(list);
    return (*this);
}

template <typename T>
Vector<T, 3>& Vector<T, 3>::operator=(const Vector& v) {
    Set(v);
    return (*this);
}

template <typename T>
Vector<T, 3>& Vector<T, 3>::operator+=(T v) {
    IAdd(v);
    return (*this);
}

template <typename T>
Vector<T, 3>& Vector<T, 3>::operator+=(const Vector& v) {
    IAdd(v);
    return (*this);
}

template <typename T>
Vector<T, 3>& Vector<T, 3>::operator-=(T v) {
    ISub(v);
    return (*this);
}

template <typename T>
Vector<T, 3>& Vector<T, 3>::operator-=(const Vector& v) {
    ISub(v);
    return (*this);
}

template <typename T>
Vector<T, 3>& Vector<T, 3>::operator*=(T v) {
    IMul(v);
    return (*this);
}

template <typename T>
Vector<T, 3>& Vector<T, 3>::operator*=(const Vector& v) {
    IMul(v);
    return (*this);
}

template <typename T>
Vector<T, 3>& Vector<T, 3>::operator/=(T v) {
    IDiv(v);
    return (*this);
}

template <typename T>
Vector<T, 3>& Vector<T, 3>::operator/=(const Vector& v) {
    IDiv(v);
    return (*this);
}

template <typename T>
bool Vector<T, 3>::operator==(const Vector& v) const {
    return IsEqual(v);
}

template <typename T>
bool Vector<T, 3>::operator!=(const Vector& v) const {
    return !IsEqual(v);
}


template <typename T>
Vector<T, 3> operator+(const Vector<T, 3>& a) {
    return a;
}

template <typename T>
Vector<T, 3> operator-(const Vector<T, 3>& a) {
    return Vector<T, 3>(-a.x, -a.y, -a.z);
}

template <typename T>
Vector<T, 3> operator+(const Vector<T, 3>& a, T b) {
    return a.Add(b);
}

template <typename T>
Vector<T, 3> operator+(T a, const Vector<T, 3>& b) {
    return b.Add(a);
}

template <typename T>
Vector<T, 3> operator+(const Vector<T, 3>& a, const Vector<T, 3>& b) {
    return a.Add(b);
}

template <typename T>
Vector<T, 3> operator-(const Vector<T, 3>& a, T b) {
    return a.Sub(b);
}

template <typename T>
Vector<T, 3> operator-(T a, const Vector<T, 3>& b) {
    return b.RSub(a);
}

template <typename T>
Vector<T, 3> operator-(const Vector<T, 3>& a, const Vector<T, 3>& b) {
    return a.Sub(b);
}

template <typename T>
Vector<T, 3> operator*(const Vector<T, 3>& a, T b) {
    return a.Mul(b);
}

template <typename T>
Vector<T, 3> operator*(T a, const Vector<T, 3>& b) {
    return b.Mul(a);
}

template <typename T>
Vector<T, 3> operator*(const Vector<T, 3>& a, const Vector<T, 3>& b) {
    return a.Mul(b);
}

template <typename T>
Vector<T, 3> operator/(const Vector<T, 3>& a, T b) {
    return a.Div(b);
}

template <typename T>
Vector<T, 3> operator/(T a, const Vector<T, 3>& b) {
    return b.RDiv(a);
}

template <typename T>
Vector<T, 3> operator/(const Vector<T, 3>& a, const Vector<T, 3>& b) {
    return a.Div(b);
}

template <typename T>
Vector<T, 3> Min(const Vector<T, 3>& a, const Vector<T, 3>& b) {
    return Vector<T, 3>(
        std::min(a.x, b.x), std::min(a.y, b.y), std::min(a.z, b.z));
}

template <typename T>
Vector<T, 3> Max(const Vector<T, 3>& a, const Vector<T, 3>& b) {
    return Vector<T, 3>(
        std::max(a.x, b.x), std::max(a.y, b.y), std::max(a.z, b.z));
}

template <typename T>
Vector<T, 3> Clamp(
    const Vector<T, 3>& v, const Vector<T, 3>& low, const Vector<T, 3>& high) {
    return Vector<T, 3>(
        Clamp(v.x, low.x, high.x),
        Clamp(v.y, low.y, high.y),
        Clamp(v.z, low.z, high.z));
}

template <typename T>
Vector<T, 3> Ceil(const Vector<T, 3>& a) {
    return Vector<T, 3>(std::ceil(a.x), std::ceil(a.y), std::ceil(a.z));
}

template <typename T>
Vector<T, 3> Floor(const Vector<T, 3>& a) {
    return Vector<T, 3>(std::floor(a.x), std::floor(a.y), std::floor(a.z));
}

// Extensions
template <typename T>
Vector<T, 3> MonotonicCatmullRom(
    const Vector<T, 3>& v0,
    const Vector<T, 3>& v1,
    const Vector<T, 3>& v2,
    const Vector<T, 3>& v3,
    T f) {
    static const T two = static_cast<T>(2);
    static const T three = static_cast<T>(3);

    Vector<T, 3> d1 = (v2 - v0) / two;
    Vector<T, 3> d2 = (v3 - v1) / two;
    Vector<T, 3> D1 = v2 - v1;

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

    Vector<T, 3> a3 = d1 + d2 - two * D1;
    Vector<T, 3> a2 = three * D1 - two * d1 - d2;
    Vector<T, 3> a1 = d1;
    Vector<T, 3> a0 = v1;

    return a3 * Cube(f) + a2 * Square(f) + a1 * f + a0;
}

}