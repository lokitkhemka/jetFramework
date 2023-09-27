#pragma once

#include <algorithm>
#include <Points/point2.h>



namespace jet
{
    //!
    //! \brief 3-D Point Class
    //!
    //! This class defines simple 3-D point data.
    //!
    //! \tparam T - Type of the Element.
    //!

    template<typename T>
    class Point<T,3> final
    {
    public:
        static_assert(
            std::is_arithmetic<T>::value,
            "Point only can be instantiated with arithmetic types.");
        
        //! X component of the point.
        T x;

        //! Y component of the point.
        T y;

        //! Z component of the point.
        T z;

        //! Constructs the default point (0, 0, 0)
        Point();

        //! Construct point with given parameters \p x and \p y and \p z.
        Point(T x, T y, T z);

        //! Constructs point with a 2D point and a scalar.
        Point(const Point2<T>& pt, T z);

        //! Construct point with initializer list \p list.
        template<typename U>
        Point(const std::initializer_list<U>& list);

        //! Copy Constructor
        Point(const Point& v);

        //! Set both x, y and z components to \p s.
        void Set(T s);

        //! Set x, y and z components with give parameters.
        void Set(T x, T y, T z);

        //! Set x, y and z components with given \p pt.x, \p pt.y and \p z.
        void Set(const Point2<T>& pt, T z);

        //! Set x and y components with given initializer list.
        template<typename U>
        void Set(const std::initializer_list<U>& list);

        //! Set x and y with other point \p point.
        void Set(const Point& point);

        //! Set both x and y to zero.
        void SetZero();

        //! Computes this + (v,v, v).
        Point Add(T v) const;

        //! Compute this + (v.x , v.y, v.z).
        Point Add(const Point& v) const;

        //! Computes this - (v,v, v).
        Point Sub(T v) const;

        //! Compute this - (v.x , v.y, v.z).
        Point Sub(const Point& v) const;

        //! Computes this * (v,v,v).
        Point Mul(T v) const;

        //! Compute this * (v.x , v.y, v.z).
        Point Mul(const Point& v) const;

        //! Computes this / (v,v,v).
        Point Div(T v) const;

        //! Compute this / (v.x , v.y, v.z).
        Point Div(const Point& v) const;

        //! Computes (v,v,v) - this.
        Point RSub(T v) const;

        //! Compute (v.x, v.y, v.z) - this.
        Point RSub(const Point& v) const;

        //! Computes (v,v,v) / this.
        Point RDiv(T v) const;

        //! Computes (v.x, v.y, v.z)/this.
        Point RDiv(const Point& v) const;


        //! Computes this += (v,v,v)
        void IAdd(T v);

        //! Compute this += (v.x, v.y, v.z)
        void IAdd(const Point& v);

        //! Computes this -= (v,v,v)
        void ISub(T v);

        //! Compute this -= (v.x, v.y, v.z)
        void ISub(const Point& v);

        //! Computes this *= (v,v,v)
        void IMul(T v);

        //! Compute this *= (v.x, v.y, v.z)
        void IMul(const Point& v);

        //! Computes this /= (v,v,v)
        void IDiv(T v);

        //! Compute this /= (v.x, v.y, v.z)
        void IDiv(const Point& v);


        //! Returns the const reference to the \p i-th element of the point.
        const T& At(size_t i) const;

        //! Returns the reference to the \p i-th of the point.
        T& At(size_t i);

        //! Returns the sum of all the components.
        T Sum() const;

        //! Returns the minimnum value among x, y and z.
        T Min() const;

        //! Returns the maximum value among x, y and z.
        T Max() const;

        //! Returns the absolute minimum value among x, y and z.
        T AbsMin() const;

        //! Returns the absolute maximum value among x, y and z.
        T AbsMax() const;

        //! Returns the index of the dominant axis.
        size_t DominantAxis() const;

        //! Returns the index of subminant axis.
        size_t SubminantAxis() const;

        //! Returns a point with different value type.
        template<typename U>
        Point<U,3> CastTo() const;

        //! Returns true if \p other is the same as this point
        bool IsEqual(const Point& other) const;

        //! Returns reference to the \p i-th element of the point.
        T& operator[](size_t i);

        //! Returns const reference to the \p i-th element of the point.
        const T& operator[](size_t i) const;

        //! Set x and y components with the given initializer list.
        Point& operator=(const std::initializer_list<T>& list);

        //! Set x and y with other point \p point.
        Point& operator=(const Point& v);

        //! Computes this+= (v,v,v).
        Point& operator+=(T v);

        //! Computes this += (v.x, v.y, v.z)
        Point& operator+=(const Point& v);

        //! Computes this-= (v,v,v).
        Point& operator-=(T v);

        //! Computes this -= (v.x, v.y, v.z)
        Point& operator-=(const Point& v);

        //! Computes this*= (v,v,v).
        Point& operator*=(T v);

        //! Computes this *= (v.x, v.y, v.z)
        Point& operator*=(const Point& v);
        
        //! Computes this /= (v,v,v).
        Point& operator/=(T v);

        //! Computes this /= (v.x, v.y, v.z)
        Point& operator/=(const Point& v);

        //! Returns true if \p other is the same as this point.
        bool operator==(const Point& v) const;

        //! Returns true if \p other is not the same as this point.
        bool operator!=(const Point& v) const;
        
    };

    //! Type alias for 2D point.
    template<typename T> using Point3 = Point<T,3>;

    //! Positive sign operator
    template<typename T>
    Point<T,3> operator+(const Point<T,3>& a);

    //! Negative sign operator
    template<typename T>
    Point3<T> operator-(const Point3<T>& a);

    //! Computes (a,a,a) + (b.x, b.y, b.z)
    template<typename T>
    Point3<T> operator+ (T a, const Point3<T>& b);

    //! Computes (a.x, a.y, a.z) + (b.x, b.y, b.z).
    template<typename T>
    Point3<T> operator+(const Point3<T>& a, const Point3<T>& b);

    //! Computes (a.x, a.y, a.z) - (b, b, b).
    template <typename T>
    Point3<T> operator-(const Point3<T>& a, T b);

    //! Computes (a, a, a) - (b.x, b.y, b.z).
    template <typename T>
    Point3<T> operator-(T a, const Point3<T>& b);

    //! Computes (a.x, a.y, a.z) - (b.x, b.y, b.z).
    template <typename T>
    Point3<T> operator-(const Point3<T>& a, const Point3<T>& b);

    //! Computes (a.x, a.y, a.z) * (b, b, b).
    template <typename T>
    Point3<T> operator*(const Point3<T>& a, T b);

    //! Computes (a, a, a) * (b.x, b.y, b.z).
    template <typename T>
    Point3<T> operator*(T a, const Point3<T>& b);

    //! Computes (a.x, a.y, a.z) * (b.x, b.y, b.z).
    template <typename T>
    Point3<T> operator*(const Point3<T>& a, const Point3<T>& b);

    //! Computes (a.x, a.y, a.z) / (b, b, b).
    template <typename T>
    Point3<T> operator/(const Point3<T>& a, T b);

    //! Computes (a, a, a) / (b.x, b.y, b.z).
    template <typename T>
    Point3<T> operator/(T a, const Point3<T>& b);

    //! Computes (a.x, a.y, a.z) / (b.x, b.y, b.z).
    template <typename T>
    Point3<T> operator/(const Point3<T>& a, const Point3<T>& b);

    //! Returns element-wise min vector.
    template <typename T>
    Point3<T> Min(const Point3<T>& a, const Point3<T>& b);

    //! Returns element-wise max vector.
    template <typename T>
    Point3<T> Max(const Point3<T>& a, const Point3<T>& b);

    //! Returns element-wise clamped vector.
    template <typename T>
    Point3<T> Clamp(
        const Point3<T>& v, const Point3<T>& low, const Point3<T>& high);

    //! Returns element-wise ceiled vector.
    template <typename T>
    Point3<T> Ceil(const Point3<T>& a);

    //! Returns element-wise floored vector.
    template <typename T>
    Point3<T> Floor(const Point3<T>& a);

    //! Float-type 2D point.
    typedef Point3<float> Point3F;

    //! Double-type 2D point.
    typedef Point3<double> Point3D;

    //! Integer-type 2D point.
    typedef Point3<ssize_t> Point3I;

    //! Unsigned integer-type 2D point.
    typedef Point3<size_t> Point3UI;




    /*
    
    * IMPLEMENTATIONS OF ABOVE METHODS
    
    */
   template<typename T>
   Point<T,3>::Point() :
    x(0), y(0), z(0)
    {

    }

    template<typename T>
    Point<T,3>::Point(T newX, T newY, T newZ)
    : x(newX), y(newY), z(newZ){}

    template<typename T>
    Point<T,3>::Point(const Point2<T>&pt, T newZ)
        : x(pt.x), y(pt.y), z(newZ){}

    template<typename T>
    template<typename U>
    Point<T,3>::Point(const std::initializer_list<U>& list)
    {
        Set(list);
    }

    template<typename T>
    Point<T,3>::Point(const Point& v)
    : x(v.x), y(v.y), z(v.z){}

    template<typename T>
    void Point<T,3>::Set(T s)
    {
        x = s;
        y = s;
        z = s;
    }
    
    template<typename T>
    void Point<T,3>::Set(T newX, T newY, T newZ)
    {
        x = newX;
        y = newY;
        z = newZ;
    }

    template<typename T>
    void Point<T,3>::Set(const Point2<T>& pt, T newZ)
    {
        x = pt.x;
        y = pt.y;
        z = newZ;
    }

    template<typename T>
    template<typename U>
    void Point<T,3>::Set(const std::initializer_list<U>& list)
    {
        JET_ASSERT(list.size() >= 3);

        auto InputElem = list.begin();
        x = static_cast<T>(*InputElem);
        y = static_cast<T>(*(++InputElem));
        z = static_cast<T>(*(++InputElem));
    }

    template<typename T>
    void Point<T,3>::Set(const Point& v)
    {
        x = v.x;
        y = v.y;
        z = v.z;
    }

    template<typename T>
    void Point<T,3>::SetZero()
    {
        x = y = z = 0;
    }


    template<typename T>
    Point<T,3> Point<T,3>::Add(T v) const
    {
        return Point(x+v, y+v, z+v);
    }

    template<typename T>
    Point<T,3> Point<T,3>::Add(const Point& v) const
    {
        return Point(x + v.x, y + v.y, z + v.z);
    }

    template<typename T>
    Point<T,3> Point<T,3>::Sub(T v) const
    {
        return Point(x - v, y - v, z - v);
    }

    template<typename T>
    Point<T,3> Point<T,3>::Sub(const Point& v) const
    {
        return Point(x - v.x, y - v.y, z - v.z);
    }

    template<typename T>
    Point<T,3> Point<T,3>::Mul(T v) const
    {
        return Point(x * v, y * v, z * v);
    }

    template<typename T>
    Point<T,3> Point<T,3>::Mul(const Point& v) const
    {
        return Point(x * v.x, y * v.y, z * v.z);
    }

    template<typename T>
    Point<T,3> Point<T,3>::Div(T v) const
    {
        return Point(x / v, y / v, z / v);
    }

    template<typename T>
    Point<T,3> Point<T,3>::Div(const Point& v) const
    {
        return Point(x / v.x, y / v.y, z / v.z);
    }

    template<typename T>
    Point<T,3> Point<T,3>::RSub(T v) const
    {
        return Point(v - x, v - y, v - z);
    }

    template<typename T>
    Point<T,3> Point<T,3>::RSub(const Point& v) const
    {
        return Point(v.x - x, v.y - y, v.z - z);
    }

    template<typename T>
    Point<T,3> Point<T,3>::RDiv(T v) const
    {
        return Point(v / x, v / y, v / z);
    }

    template<typename T>
    Point<T,3> Point<T,3>::RDiv(const Point& v) const
    {
        return Point(v.x / x, v.y / y, v.z / z);
    }

    template<typename T>
    void Point<T,3>::IAdd(T v)
    {
        x += v;
        y += v;
        z += v;
    }

    template<typename T>
    void Point<T,3>::IAdd(const Point& v)
    {
        x += v.x;
        y += v.y;
        z += v.z;
    }

    template<typename T>
    void Point<T,3>::ISub(T v)
    {
        x -= v;
        y -= v;
        z -= v;
    }

    template<typename T>
    void Point<T,3>::ISub(const Point& v)
    {
        x -= v.x;
        y -= v.y;
        z -= v.z;
    }

    template<typename T>
    void Point<T,3>::IMul(T v)
    {
        x *= v;
        y *= v;
        z *= v;
    }

    template<typename T>
    void Point<T,3>::IMul(const Point& v)
    {
        x *= v.x;
        y *= v.y;
        z *= v.z;
    }

    template<typename T>
    void Point<T,3>::IDiv(T v)
    {
        x /= v;
        y /= v;
        z /= v;
    }

    template<typename T>
    void Point<T,3>::IDiv(const Point& v)
    {
        x /= v.x;
        y /= v.y;
        z /= v.z;
    }

    template<typename T>
    const T& Point<T,3>::At(size_t i) const
    {
        assert(i<3);
        return (&x)[i];
    }

    template<typename T>
    T& Point<T,3>::At(size_t i)
    {
        assert(i<3);
        return (&x)[i];
    }

    template<typename T>
    T Point<T,3>::Sum() const
    {
        return x + y + z;
    }

    template<typename T>
    T Point<T,3>::Min() const
    {
        return std::min(std::min(x,y),z);
    }

    template<typename T>
    T Point<T,3>::Max() const
    {
        return std::max(std::max(x,y),z);
    }

    template<typename T>
    T Point<T,3>::AbsMin() const
    {
        return jet::AbsMin(jet::AbsMin(x,y),z);
    }

    template<typename T>
    T Point<T,3>::AbsMax() const
    {
        return jet::AbsMax(jet::AbsMax(x,y),z);
    }

    template<typename T>
    size_t Point<T,3>::DominantAxis() const
    {
        return (std::fabs(x) > std::fabs(y))
        ? ((std::fabs(x) > std::fabs(z)) ? 0 : 2)
        : ((std::fabs(y) > std::fabs(z)) ? 1 : 2);
    }

    template<typename T>
    size_t Point<T,3>::SubminantAxis() const
    {
        return (std::fabs(x) < std::fabs(y))
        ? ((std::fabs(x) < std::fabs(z)) ? 0 : 2)
        : ((std::fabs(y) < std::fabs(z)) ? 1 : 2);
    }

    template<typename T>
    template<typename U>
    Point3<U> Point<T,3>::CastTo() const
    {
        return Point3<U>(static_cast<U>(x), static_cast<U>(y), static_cast<U>(z));
    }

    template<typename T>
    bool Point<T,3>::IsEqual(const Point& other) const
    {
        return (x== other.x && y == other.y && z == other.z);
    }

    template<typename T>
    T& Point<T,3>::operator[](size_t i)
    {
        assert(i<3);
        return (&x)[i];
    }

    template<typename T>
    const T& Point<T,3>::operator[](size_t i) const
    {
        assert(i<3);
        return (&x)[i];
    }

    template<typename T>
    Point<T,3>& Point<T,3>::operator=(const std::initializer_list<T>& list)
    {
        Set(list);
        return (*this);
    }

    template<typename T>
    Point<T,3>& Point<T,3>::operator=(const Point& v)
    {
        Set(v);
        return (*this);
    }

    template<typename T>
    Point<T,3>& Point<T,3>::operator+=(T v)
    {
        IAdd(v);
        return (*this);
    }

    template<typename T>
    Point<T,3>& Point<T,3>::operator+=(const Point& v)
    {
        IAdd(v);
        return (*this);
    }

    template<typename T>
    Point<T,3>& Point<T,3>::operator-=(T v)
    {
        ISub(v);
        return (*this);
    }

    template<typename T>
    Point<T,3>& Point<T,3>::operator-=(const Point& v)
    {
        ISub(v);
        return (*this);
    }

    template<typename T>
    Point<T,3>& Point<T,3>::operator*=(T v)
    {
        IMul(v);
        return (*this);
    }

    template<typename T>
    Point<T,3>& Point<T,3>::operator*=(const Point& v)
    {
        IMul(v);
        return (*this);
    }

    template<typename T>
    Point<T,3>& Point<T,3>::operator/=(T v)
    {
        IDiv(v);
        return (*this);
    }

    template<typename T>
    Point<T,3>& Point<T,3>::operator/=(const Point& v)
    {
        IDiv(v);
        return (*this);
    }

    template<typename T>
    bool Point<T,3>::operator==(const Point& v) const
    {
        return IsEqual(v);
    }

    template<typename T>
    bool Point<T,3>::operator!=(const Point& v) const
    {
        return !IsEqual(v);
    }

    template<typename T>
    Point<T,3> operator+(const Point<T,3>& a)
    {
        return a;
    }
    
    template<typename T>
    Point<T,3> operator-(const Point<T,3>& a)
    {
        return Point<T,3>(-a.x, -a.y, -a.z);
    }

    template<typename T>
    Point<T,3> operator+(const Point<T,3>& a, T b)
    {
        return a.Add(b);
    }

    template<typename T>
    Point<T,3> operator+(T a, const Point<T,3>& b)
    {
        return b.Add(a);
    }

    template<typename T>
    Point<T,3> operator+(const Point<T,3>& a, const Point<T,3>& b)
    {
        return a.Add(b);
    }

    template<typename T>
    Point<T,3> operator-(const Point<T,3>& a, T b)
    {
        return a.Sub(b);
    }

    template<typename T>
    Point<T,3> operator-(T a, const Point<T,3>& b)
    {
        return b.RSub(a);
    }

    template<typename T>
    Point<T,3> operator-(const Point<T,3>& a, const Point<T,3>& b)
    {
        return a.Sub(b);
    }

    template<typename T>
    Point<T,3> operator*(const Point<T,3>& a, T b)
    {
        return a.Mul(b);
    }

    template<typename T>
    Point<T,3> operator*(T a, const Point<T,3>& b)
    {
        return b.Mul(a);
    }

    template<typename T>
    Point<T,3> operator*(const Point<T,3>& a, const Point<T,3>& b)
    {
        return a.Mul(b);
    }

    template<typename T>
    Point<T,3> operator/(const Point<T,3>& a, T b)
    {
        return a.Div(b);
    }

    template<typename T>
    Point<T,3> operator/(T a, const Point<T,3>& b)
    {
        return b.RDiv(a);
    }

    template<typename T>
    Point<T,3> operator/(const Point<T,3>& a, const Point<T,3>& b)
    {
        return a.Div(b);
    }

    template<typename T>
    Point<T,3> Min(const Point<T,3>& a, const Point<T,3>& b)
    {
        return Point<T,3>(std::min(a.x, b.x), std::min(a.y, b.y), std::min(a.z, b.z));
    }

    template<typename T>
    Point<T,3> Max(const Point<T,3>& a, const Point<T,3>& b)
    {
        return Point<T,3>(std::max(a.x, b.x), std::max(a.y, b.y), std::max(a.z, b.z));
    }

    template<typename T>
    Point<T,3> Clamp( const Point<T,3>& v, const Point<T,3>& low, const Point<T,3>& high)
    {
        return Point<T,3>(Clamp(v.x, low.x, high.x), Clamp(v.y, low.y, high.y), Clamp(v.z, low.z, high.z));
    }

    template<typename T>
    Point<T,3> Ceil(const Point<T,3>& a)
    {
        return Point<T,3>(std::ceil(a.x), std::ceil(a.y), std::ceil(a.z));
    }

    template<typename T>
    Point<T,3> Floor(const Point<T,3>& a)
    {
        return Point<T,3>(std::floor(a.x), std::floor(a.y), std::floor(a.z));
    }

}