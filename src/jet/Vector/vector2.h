#pragma once

#include<Vector/vector.h>
#include<limits>
#include<algorithm>

#include<macros.h>
#include<math-utils.h>


namespace jet {

//!
//! \brief 2-D vector class.
//!
//! This class defines simple 2-D vector data.
//!
//! \tparam T - Type of the element
//!
template <typename T>
class Vector<T, 2> final {
 public:
    static_assert(
        std::is_floating_point<T>::value,
        "Vector only can be instantiated with floating point types");

    //! X (or the first) component of the vector.
    T x;

    //! Y (or the second) component of the vector.
    T y;



    //! Constructs default vector (0, 0).
    Vector();

    //! Constructs vector with given parameters \p x and \p y.
    Vector(T x, T y);

    //! Constructs vector with initializer list.
    template <typename U>
    Vector(const std::initializer_list<U>& list);

    //! Copy constructor.
    Vector(const Vector& v);

    // MARK: Basic setters

    //! Set both x and y components to \p s.
    void Set(T s);

    //! Set x and y components with given parameters.
    void Set(T x, T y);

    //! Set x and y components with given initializer list.
    template <typename U>
    void Set(const std::initializer_list<U>& lst);

    //! Set x and y with other vector \p pt.
    void Set(const Vector& pt);

    //! Set both x and y to zero.
    void SetZero();

    //! Normalizes this vector.
    void Normalize();

    // MARK: Binary operations: new instance = this (+) v

    //! Computes this + (v, v).
    Vector Add(T v) const;

    //! Computes this + (v.x, v.y).
    Vector Add(const Vector& v) const;

    //! Computes this - (v, v).
    Vector Sub(T v) const;

    //! Computes this - (v.x, v.y).
    Vector Sub(const Vector& v) const;

    //! Computes this * (v, v).
    Vector Mul(T v) const;

    //! Computes this * (v.x, v.y).
    Vector Mul(const Vector& v) const;

    //! Computes this / (v, v).
    Vector Div(T v) const;

    //! Computes this / (v.x, v.y).
    Vector Div(const Vector& v) const;

    //! Computes dot product.
    T Dot(const Vector& v) const;

    //! Comptues cross product.
    T Cross(const Vector& v) const;

    // MARK: Binary operations: new instance = v (+) this

    //! Computes (v, v) - this.
    Vector RSub(T v) const;

    //! Computes (v.x, v.y) - this.
    Vector RSub(const Vector& v) const;

    //! Computes (v, v) / this.
    Vector RDiv(T v) const;

    //! Computes (v.x, v.y) / this.
    Vector RDiv(const Vector& v) const;

    //! Computes \p v cross this.
    T RCross(const Vector& v) const;

    // MARK: Augmented operations: this (+)= v

    //! Computes this += (v, v).
    void IAdd(T v);

    //! Computes this += (v.x, v.y).
    void IAdd(const Vector& v);

    //! Computes this -= (v, v).
    void ISub(T v);

    //! Computes this -= (v.x, v.y).
    void ISub(const Vector& v);

    //! Computes this *= (v, v).
    void IMul(T v);

    //! Computes this *= (v.x, v.y).
    void IMul(const Vector& v);

    //! Computes this /= (v, v).
    void IDiv(T v);

    //! Computes this /= (v.x, v.y).
    void IDiv(const Vector& v);

    // MARK: Basic getters

    //! Returns const reference to the \p i -th element of the vector.
    const T& At(size_t i) const;

    //! Returns reference to the \p i -th element of the vector.
    T& At(size_t i);

    //! Returns the sum of all the components (i.e. x + y).
    T Sum() const;

    //! Returns the average of all the components.
    T Avg() const;

    //! Returns the minimum value among x and y.
    T Min() const;

    //! Returns the maximum value among x and y.
    T Max() const;

    //! Returns the absolute minimum value among x and y.
    T AbsMin() const;

    //! Returns the absolute maximum value among x and y.
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
    Vector Tangential() const;

    //! Returns a vector with different value type.
    template <typename U>
    Vector<U, 2> CastTo() const;

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


//! Type alias for two dimensional vector.
template <typename T> using Vector2 = Vector<T, 2>;

//! Positive sign operator.
template <typename T>
Vector2<T> operator+(const Vector2<T>& a);

//! Negative sign operator.
template <typename T>
Vector2<T> operator-(const Vector2<T>& a);

//! Computes (a, a) + (b.x, b.y).
template <typename T>
Vector2<T> operator+(T a, const Vector2<T>& b);

//! Computes (a.x, a.y) + (b.x, b.y).
template <typename T>
Vector2<T> operator+(const Vector2<T>& a, const Vector2<T>& b);

//! Computes (a.x, a.y) - (b, b).
template <typename T>
Vector2<T> operator-(const Vector2<T>& a, T b);

//! Computes (a, a) - (b.x, b.y).
template <typename T>
Vector2<T> operator-(T a, const Vector2<T>& b);

//! Computes (a.x, a.y) - (b.x, b.y).
template <typename T>
Vector2<T> operator-(const Vector2<T>& a, const Vector2<T>& b);

//! Computes (a.x, a.y) * (b, b).
template <typename T>
Vector2<T> operator*(const Vector2<T>& a, T b);

//! Computes (a, a) * (b.x, b.y).
template <typename T>
Vector2<T> operator*(T a, const Vector2<T>& b);

//! Computes (a.x, a.y) * (b.x, b.y).
template <typename T>
Vector2<T> operator*(const Vector2<T>& a, const Vector2<T>& b);

//! Computes (a.x, a.y) / (b, b).
template <typename T>
Vector2<T> operator/(const Vector2<T>& a, T b);

//! Computes (a, a) / (b.x, b.y).
template <typename T>
Vector2<T> operator/(T a, const Vector2<T>& b);

//! Computes (a.x, a.y) / (b.x, b.y).
template <typename T>
Vector2<T> operator/(const Vector2<T>& a, const Vector2<T>& b);

//! Returns element-wise min vector: (min(a.x, b.x), min(a.y, b.y)).
template <typename T>
Vector2<T> Min(const Vector2<T>& a, const Vector2<T>& b);

//! Returns element-wise max vector: (max(a.x, b.x), max(a.y, b.y)).
template <typename T>
Vector2<T> Max(const Vector2<T>& a, const Vector2<T>& b);

//! Returns element-wise clamped vector.
template <typename T>
Vector2<T> Clamp(
    const Vector2<T>& v, const Vector2<T>& low, const Vector2<T>& high);

//! Returns element-wise ceiled vector.
template <typename T>
Vector2<T> Ceil(const Vector2<T>& a);

//! Returns element-wise floored vector.
template <typename T>
Vector2<T> Floor(const Vector2<T>& a);

//! Float-type 2D vector.
typedef Vector2<float> Vector2F;

//! Double-type 2D vector.
typedef Vector2<double> Vector2D;

// MARK: Extensions

// //! Returns float-type zero vector.
// template <>
// inline Vector2F Zero<Vector<float, 2>>() {
//     return Vector2F(0.f, 0.f);
// }

// //! Returns double-type zero vector.
// template <>
// inline Vector2D Zero<Vector<double, 2>() {
//     return Vector2D(0.0, 0.0);
// }

//! Returns the type of the value itself.
template <typename T>
struct ScalarType<Vector2<T>> {
    typedef T value;
};

//! Computes monotonic Catmull-Rom interpolation.
template <typename T>
Vector2<T> MonotonicCatmullRom(
    const Vector2<T>& v0,
    const Vector2<T>& v1,
    const Vector2<T>& v2,
    const Vector2<T>& v3,
    T f);

// Constructors
template <typename T>
Vector<T, 2>::Vector() :
    x(0),
    y(0) {
}

template <typename T>
Vector<T, 2>::Vector(T newX, T newY) :
    x(newX),
    y(newY) {
}

template <typename T>
template <typename U>
Vector<T, 2>::Vector(const std::initializer_list<U>& lst) {
    Set(lst);
}

template <typename T>
Vector<T, 2>::Vector(const Vector& v) :
    x(v.x),
    y(v.y) {
}

// Basic setters
template <typename T>
void Vector<T, 2>::Set(T s) {
    x = s;
    y = s;
}

template <typename T>
void Vector<T, 2>::Set(T newX, T newY) {
    x = newX;
    y = newY;
}

template <typename T>
template <typename U>
void Vector<T, 2>::Set(const std::initializer_list<U>& list) {
    JET_ASSERT(list.size() >= 2);

    auto inputElem = list.begin();
    x = static_cast<T>(*inputElem);
    y = static_cast<T>(*(++inputElem));
}

template <typename T>
void Vector<T, 2>::Set(const Vector& v) {
    x = v.x;
    y = v.y;
}

template <typename T>
void Vector<T, 2>::SetZero() {
    x = y = 0;
}

template <typename T>
void Vector<T, 2>::Normalize() {
    T l = Length();
    x /= l;
    y /= l;
}

// Binary operators: new instance = this (+) v
template <typename T>
Vector<T, 2> Vector<T, 2>::Add(T v) const {
    return Vector(x + v, y + v);
}

template <typename T>
Vector<T, 2> Vector<T, 2>::Add(const Vector& v) const {
    return Vector(x + v.x, y + v.y);
}

template <typename T>
Vector<T, 2> Vector<T, 2>::Sub(T v) const {
    return Vector(x - v, y - v);
}

template <typename T>
Vector<T, 2> Vector<T, 2>::Sub(const Vector& v) const {
    return Vector(x - v.x, y - v.y);
}

template <typename T>
Vector<T, 2> Vector<T, 2>::Mul(T v) const {
    return Vector(x * v, y * v);
}

template <typename T>
Vector<T, 2> Vector<T, 2>::Mul(const Vector& v) const {
    return Vector(x * v.x, y * v.y);
}

template <typename T>
Vector<T, 2> Vector<T, 2>::Div(T v) const {
    return Vector(x / v, y / v);
}

template <typename T>
Vector<T, 2> Vector<T, 2>::Div(const Vector& v) const {
    return Vector(x / v.x, y / v.y);
}

template <typename T>
T Vector<T, 2>::Dot(const Vector& v) const {
    return x * v.x + y * v.y;
}

template <typename T>
T Vector<T, 2>::Cross(const Vector& v) const {
    return x * v.y - v.x * y;
}

// Binary operators: new instance = v (+) this
template <typename T>
Vector<T, 2> Vector<T, 2>::RSub(T v) const {
    return Vector(v - x, v - y);
}

template <typename T>
Vector<T, 2> Vector<T, 2>::RSub(const Vector& v) const {
    return Vector(v.x - x, v.y - y);
}

template <typename T>
Vector<T, 2> Vector<T, 2>::RDiv(T v) const {
    return Vector(v / x, v / y);
}

template <typename T>
Vector<T, 2> Vector<T, 2>::RDiv(const Vector& v) const {
    return Vector(v.x / x, v.y / y);
}

template <typename T>
T Vector<T, 2>::RCross(const Vector& v) const {
    return v.x * y - x * v.y;
}

// Augmented operators: this (+)= v
template <typename T>
void Vector<T, 2>::IAdd(T v) {
    x += v;
    y += v;
}

template <typename T>
void Vector<T, 2>::IAdd(const Vector& v) {
    x += v.x;
    y += v.y;
}

template <typename T>
void Vector<T, 2>::ISub(T v) {
    x -= v;
    y -= v;
}

template <typename T>
void Vector<T, 2>::ISub(const Vector& v) {
    x -= v.x;
    y -= v.y;
}

template <typename T>
void Vector<T, 2>::IMul(T v) {
    x *= v;
    y *= v;
}

template <typename T>
void Vector<T, 2>::IMul(const Vector& v) {
    x *= v.x;
    y *= v.y;
}

template <typename T>
void Vector<T, 2>::IDiv(T v) {
    x /= v;
    y /= v;
}

template <typename T>
void Vector<T, 2>::IDiv(const Vector& v) {
    x /= v.x;
    y /= v.y;
}

// Basic getters
template <typename T>
const T& Vector<T, 2>::At(size_t i) const {
    JET_ASSERT(i < 2);
    return (&x)[i];
}

template <typename T>
T& Vector<T, 2>::At(size_t i) {
    JET_ASSERT(i < 2);
    return (&x)[i];
}

template <typename T>
T Vector<T, 2>::Sum() const {
    return x + y;
}

template <typename T>
T Vector<T, 2>::Avg() const {
    return (x + y)/2;
}

template <typename T>
T Vector<T, 2>::Min() const {
    return std::min(x, y);
}

template <typename T>
T Vector<T, 2>::Max() const {
    return std::max(x, y);
}

template <typename T>
T Vector<T, 2>::AbsMin() const {
    return jet::AbsMin(x, y);
}

template <typename T>
T Vector<T, 2>::AbsMax() const {
    return jet::AbsMax(x, y);
}

template <typename T>
size_t Vector<T, 2>::DominantAxis() const {
    return (std::fabs(x) > std::fabs(y)) ? 0 : 1;
}

template <typename T>
size_t Vector<T, 2>::SubminantAxis() const {
    return (std::fabs(x) < std::fabs(y)) ? 0 : 1;
}

template <typename T>
Vector<T, 2> Vector<T, 2>::Normalized() const {
    T l = Length();
    return Vector(x/l, y/l);
}

template <typename T>
T Vector<T, 2>::Length() const {
    return std::sqrt(x*x + y*y);
}

template <typename T>
T Vector<T, 2>::LengthSquared() const {
    return x*x + y*y;
}

template <typename T>
T Vector<T, 2>::DistanceTo(const Vector<T, 2>& other) const {
    return Sub(other).Length();
}

template <typename T>
T Vector<T, 2>::DistanceSquaredTo(const Vector<T, 2>& other) const {
    return Sub(other).LengthSquared();
}

template <typename T>
Vector<T, 2> Vector<T, 2>::Reflected(const Vector<T, 2>& normal) const {
    // this - 2(this.n)n
    return Sub(normal.Mul(2 * Dot(normal)));
}

template <typename T>
Vector<T, 2> Vector<T, 2>::Projected(const Vector<T, 2>& normal) const {
    // this - this.n n
    return Sub(normal.Mul(Dot(normal)));
}

template <typename T>
Vector<T, 2> Vector<T, 2>::Tangential() const {
    // Rotate 90 degrees
    return Vector<T, 2>(-y, x);
}

template <typename T>
template <typename U>
Vector<U, 2> Vector<T, 2>::CastTo() const {
    return Vector<U, 2>(static_cast<U>(x), static_cast<U>(y));
}

template <typename T>
bool Vector<T, 2>::IsEqual(const Vector& other) const {
    return x == other.x && y == other.y;
}

template <typename T>
bool Vector<T, 2>::IsSimilar(const Vector& other, T epsilon) const {
    return (std::fabs(x - other.x) < epsilon) &&
        (std::fabs(y - other.y) < epsilon);
}

// Operators
template <typename T>
T& Vector<T, 2>::operator[](size_t i) {
    JET_ASSERT(i < 2);
    return (&x)[i];
}

template <typename T>
const T& Vector<T, 2>::operator[](size_t i) const {
    JET_ASSERT(i < 2);
    return (&x)[i];
}

template <typename T>
template <typename U>
Vector<T, 2>& Vector<T, 2>::operator=(const std::initializer_list<U>& lst) {
    Set(lst);
    return (*this);
}

template <typename T>
Vector<T, 2>& Vector<T, 2>::operator=(const Vector& v) {
    Set(v);
    return (*this);
}

template <typename T>
Vector<T, 2>& Vector<T, 2>::operator+=(T v) {
    IAdd(v);
    return (*this);
}

template <typename T>
Vector<T, 2>& Vector<T, 2>::operator+=(const Vector& v) {
    IAdd(v);
    return (*this);
}

template <typename T>
Vector<T, 2>& Vector<T, 2>::operator-=(T v) {
    ISub(v);
    return (*this);
}

template <typename T>
Vector<T, 2>& Vector<T, 2>::operator-=(const Vector& v) {
    ISub(v);
    return (*this);
}

template <typename T>
Vector<T, 2>& Vector<T, 2>::operator*=(T v) {
    IMul(v);
    return (*this);
}

template <typename T>
Vector<T, 2>& Vector<T, 2>::operator*=(const Vector& v) {
    IMul(v);
    return (*this);
}

template <typename T>
Vector<T, 2>& Vector<T, 2>::operator/=(T v) {
    IDiv(v);
    return (*this);
}

template <typename T>
Vector<T, 2>& Vector<T, 2>::operator/=(const Vector& v) {
    IDiv(v);
    return (*this);
}

template <typename T>
bool Vector<T, 2>::operator==(const Vector& v) const {
    return IsEqual(v);
}

template <typename T>
bool Vector<T, 2>::operator!=(const Vector& v) const {
    return !IsEqual(v);
}

// Math functions
template <typename T>
Vector<T, 2> operator+(const Vector<T, 2>& a) {
    return a;
}

template <typename T>
Vector<T, 2> operator-(const Vector<T, 2>& a) {
    return Vector<T, 2>(-a.x, -a.y);
}

template <typename T>
Vector<T, 2> operator+(const Vector<T, 2>& a, T b) {
    return a.Add(b);
}

template <typename T>
Vector<T, 2> operator+(T a, const Vector<T, 2>& b) {
    return b.Add(a);
}

template <typename T>
Vector<T, 2> operator+(const Vector<T, 2>& a, const Vector<T, 2>& b) {
    return a.Add(b);
}

template <typename T>
Vector<T, 2> operator-(const Vector<T, 2>& a, T b) {
    return a.Sub(b);
}

template <typename T>
Vector<T, 2> operator-(T a, const Vector<T, 2>& b) {
    return b.RSub(a);
}

template <typename T>
Vector<T, 2> operator-(const Vector<T, 2>& a, const Vector<T, 2>& b) {
    return a.Sub(b);
}

template <typename T>
Vector<T, 2> operator*(const Vector<T, 2>& a, T b) {
    return a.Mul(b);
}

template <typename T>
Vector<T, 2> operator*(T a, const Vector<T, 2>& b) {
    return b.Mul(a);
}

template <typename T>
Vector<T, 2> operator*(const Vector<T, 2>& a, const Vector<T, 2>& b) {
    return a.Mul(b);
}

template <typename T>
Vector<T, 2> operator/(const Vector<T, 2>& a, T b) {
    return a.Div(b);
}

template <typename T>
Vector<T, 2> operator/(T a, const Vector<T, 2>& b) {
    return b.RDiv(a);
}

template <typename T>
Vector<T, 2> operator/(const Vector<T, 2>& a, const Vector<T, 2>& b) {
    return a.Div(b);
}

template <typename T>
Vector<T, 2> Min(const Vector<T, 2>& a, const Vector<T, 2>& b) {
    return Vector<T, 2>(std::min(a.x, b.x), std::min(a.y, b.y));
}

template <typename T>
Vector<T, 2> Max(const Vector<T, 2>& a, const Vector<T, 2>& b) {
    return Vector<T, 2>(std::max(a.x, b.x), std::max(a.y, b.y));
}

template <typename T>
Vector<T, 2> Clamp(
    const Vector<T, 2>& v, const Vector<T, 2>& low, const Vector<T, 2>& high) {
    return Vector<T, 2>(Clamp(v.x, low.x, high.x), Clamp(v.y, low.y, high.y));
}

template <typename T>
Vector<T, 2> Ceil(const Vector<T, 2>& a) {
    return Vector<T, 2>(std::ceil(a.x), std::ceil(a.y));
}

template <typename T>
Vector<T, 2> Floor(const Vector<T, 2>& a) {
    return Vector<T, 2>(std::floor(a.x), std::floor(a.y));
}

// Extensions
template <typename T>
Vector<T, 2> MonotonicCatmullRom(
    const Vector<T, 2>& v0,
    const Vector<T, 2>& v1,
    const Vector<T, 2>& v2,
    const Vector<T, 2>& v3,
    T f) {
    static const T two = static_cast<T>(2);
    static const T three = static_cast<T>(3);

    Vector<T, 2> d1 = (v2 - v0) / two;
    Vector<T, 2> d2 = (v3 - v1) / two;
    Vector<T, 2> D1 = v2 - v1;

    if (std::fabs(D1.x) < std::numeric_limits<T>::epsilon() ||
        Sign(D1.x) != Sign(d1.x) ||
        Sign(D1.x) != Sign(d2.x)) {
        d1.x = d2.x = 0;
    }

    if (std::fabs(D1.y) < std::numeric_limits<T>::epsilon() ||
        Sign(D1.y) != Sign(d1.y) ||
        Sign(D1.y) != Sign(d2.y)) {
        d1.y = d2.y = 0;
    }

    Vector<T, 2> a3 = d1 + d2 - two * D1;
    Vector<T, 2> a2 = three * D1 - two * d1 - d2;
    Vector<T, 2> a1 = d1;
    Vector<T, 2> a0 = v1;

    return a3 * Cube(f) + a2 * Square(f) + a1 * f + a0;
}


}