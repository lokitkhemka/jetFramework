#pragma once

#include <Points/point.h>
#include <algorithm>
#include <math-utils.h>

#include <macros.h>



namespace jet
{
    //!
    //! \brief 2-D Point Class
    //!
    //! This class defines simple 2-D point data.
    //!
    //! \tparam T - Type of the Element.
    //!

    template<typename T>
    class Point<T,2> final
    {
    public:
        static_assert(
            std::is_arithmetic<T>::value,
            "Point only can be instantiated with arithmetic types.");
        
        //! X component of the point.
        T x;

        //! Y component of the point.
        T y;

        //! Constructs the default point (0,0)
        Point();

        //! Construct point with given parameters \p x and \p y.
        Point(T x, T y);

        //! Construct point with initializer list \p list.
        template<typename U>
        Point(const std::initializer_list<U>& list);

        //! Copy Constructor
        Point(const Point& v);

        //! Set both x and y components to \p s.
        void Set(T s);

        //! Set x and y components with give parameters.
        void Set(T x, T y);

        //! Set x and y components with given initializer list.
        template<typename U>
        void Set(const std::initializer_list<U>& list);

        //! Set x and y with other point \p point.
        void Set(const Point& point);

        //! Set both x and y to zero.
        void SetZero();

        //! Computes this + (v,v).
        Point Add(T v) const;

        //! Compute this + (v.x , v.y).
        Point Add(const Point& v) const;

        //! Computes this - (v,v).
        Point Sub(T v) const;

        //! Compute this - (v.x , v.y).
        Point Sub(const Point& v) const;

        //! Computes this * (v,v).
        Point Mul(T v) const;

        //! Compute this * (v.x , v.y).
        Point Mul(const Point& v) const;

        //! Computes this / (v,v).
        Point Div(T v) const;

        //! Compute this / (v.x , v.y).
        Point Div(const Point& v) const;

        //! Computes (v,v) - this.
        Point RSub(T v) const;

        //! Compute (v.x, v.y) - this.
        Point RSub(const Point& v) const;

        //! Computes (v,v) / this.
        Point RDiv(T v) const;

        //! Computes (v.x, v.y)/this.
        Point RDiv(const Point& v) const;

        //! Computes this += (v,v)
        void IAdd(T v);

        //! Compute this += (v.x, v.y)
        void IAdd(const Point& v);

        //! Computes this -= (v,v)
        void ISub(T v);

        //! Compute this -= (v.x, v.y)
        void ISub(const Point& v);

        //! Computes this *= (v,v)
        void IMul(T v);

        //! Compute this *= (v.x, v.y)
        void IMul(const Point& v);

        //! Computes this /= (v,v)
        void IDiv(T v);

        //! Compute this /= (v.x, v.y)
        void IDiv(const Point& v);


        //! Returns the const reference to the \p i-th element of the point.
        const T& At(size_t i) const;

        //! Returns the reference to the \p i-th of the point.
        T& At(size_t i);

        //! Returns the sum of all the components.
        T Sum() const;

        //! Returns the minimnum value among x and y.
        T Min() const;

        //! Returns the maximum value among x and y.
        T Max() const;

        //! Returns the absolute minimum value among x and y.
        T AbsMin() const;

        //! Returns the absolute maximum value among x and y.
        T AbsMax() const;

        //! Returns the index of the dominant axis.
        size_t DominantAxis() const;

        //! Returns the index of subminant axis.
        size_t SubminantAxis() const;

        //! Returns a point with different value type.
        template<typename U>
        Point<U,2> CastTo() const;

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

        //! Computes this+= (v,v).
        Point& operator+=(T v);

        //! Computes this += (v.x, v.y)
        Point& operator+=(const Point& v);

        //! Computes this-= (v,v).
        Point& operator-=(T v);

        //! Computes this -= (v.x, v.y)
        Point& operator-=(const Point& v);

        //! Computes this*= (v,v).
        Point& operator*=(T v);

        //! Computes this *= (v.x, v.y)
        Point& operator*=(const Point& v);
        
        //! Computes this /= (v,v).
        Point& operator/=(T v);

        //! Computes this /= (v.x, v.y)
        Point& operator/=(const Point& v);

        //! Returns true if \p other is the same as this point.
        bool operator==(const Point& v) const;

        //! Returns true if \p other is not the same as this point.
        bool operator!=(const Point& v) const;
        
    };

    //! Type alias for 2D point.
    template<typename T> using Point2 = Point<T,2>;

    //! Positive sign operator
    template<typename T>
    Point<T,2> operator+(const Point<T,2>& a);

    //! Negative sign operator
    template<typename T>
    Point2<T> operator-(const Point2<T>& a);

    //! Computes (a,a) + (b.x, b.y)
    template<typename T>
    Point2<T> operator+ (T a, const Point2<T>& b);

    //! Computes (a.x, a.y) + (b.x, b.y).
    template<typename T>
    Point2<T> operator+(const Point2<T>& a, const Point2<T>& b);

    //! Computes (a.x, a.y) - (b, b).
    template <typename T>
    Point2<T> operator-(const Point2<T>& a, T b);

    //! Computes (a, a) - (b.x, b.y).
    template <typename T>
    Point2<T> operator-(T a, const Point2<T>& b);

    //! Computes (a.x, a.y) - (b.x, b.y).
    template <typename T>
    Point2<T> operator-(const Point2<T>& a, const Point2<T>& b);

    //! Computes (a.x, a.y) * (b, b).
    template <typename T>
    Point2<T> operator*(const Point2<T>& a, T b);

    //! Computes (a, a) * (b.x, b.y).
    template <typename T>
    Point2<T> operator*(T a, const Point2<T>& b);

    //! Computes (a.x, a.y) * (b.x, b.y).
    template <typename T>
    Point2<T> operator*(const Point2<T>& a, const Point2<T>& b);

    //! Computes (a.x, a.y) / (b, b).
    template <typename T>
    Point2<T> operator/(const Point2<T>& a, T b);

    //! Computes (a, a) / (b.x, b.y).
    template <typename T>
    Point2<T> operator/(T a, const Point2<T>& b);

    //! Computes (a.x, a.y) / (b.x, b.y).
    template <typename T>
    Point2<T> operator/(const Point2<T>& a, const Point2<T>& b);

    //! Returns element-wise min vector: (min(a.x, b.x), min(a.y, b.y)).
    template <typename T>
    Point2<T> Min(const Point2<T>& a, const Point2<T>& b);

    //! Returns element-wise max vector: (max(a.x, b.x), max(a.y, b.y)).
    template <typename T>
    Point2<T> Max(const Point2<T>& a, const Point2<T>& b);

    //! Returns element-wise clamped vector.
    template <typename T>
    Point2<T> Clamp(
        const Point2<T>& v, const Point2<T>& low, const Point2<T>& high);

    //! Returns element-wise ceiled vector.
    template <typename T>
    Point2<T> Ceil(const Point2<T>& a);

    //! Returns element-wise floored vector.
    template <typename T>
    Point2<T> Floor(const Point2<T>& a);

    //! Float-type 2D point.
    typedef Point2<float> Point2F;

    //! Double-type 2D point.
    typedef Point2<double> Point2D;

    //! Integer-type 2D point.
    typedef Point2<ssize_t> Point2I;

    //! Unsigned integer-type 2D point.
    typedef Point2<size_t> Point2UI;




    /*
    
    * IMPLEMENTATIONS OF ABOVE METHODS
    
    */
   template<typename T>
   Point<T,2>::Point() :
    x(0), y(0)
    {

    }

    template<typename T>
    Point<T,2>::Point(T newX, T newY)
    : x(newX), y(newY){}

    template<typename T>
    template<typename U>
    Point<T,2>::Point(const std::initializer_list<U>& list)
    {
        Set(list);
    }

    template<typename T>
    Point<T,2>::Point(const Point& v)
    : x(v.x), y(v.y){}

    template<typename T>
    void Point<T,2>::Set(T s)
    {
        x = s;
        y = s;
    }
    
    template<typename T>
    void Point<T,2>::Set(T newX, T newY)
    {
        x = newX;
        y = newY;
    }

    template<typename T>
    template<typename U>
    void Point<T,2>::Set(const std::initializer_list<U>& list)
    {
        JET_ASSERT(list.Size() >= 2);

        auto InputElem = list.begin();
        x = static_cast<T>(*InputElem);
        y = static_cast<T>(*(++InputElem));
    }

    template<typename T>
    void Point<T,2>::Set(const Point& v)
    {
        x = v.x;
        y = v.y;
    }

    template<typename T>
    void Point<T,2>::SetZero()
    {
        x = y = 0;
    }


    template<typename T>
    Point<T,2> Point<T,2>::Add(T v) const
    {
        return Point(x+v, y+v);
    }

    template<typename T>
    Point<T,2> Point<T,2>::Add(const Point& v) const
    {
        return Point(x + v.x, y + v.y);
    }

    template<typename T>
    Point<T,2> Point<T,2>::Sub(T v) const
    {
        return Point(x - v, y - v);
    }

    template<typename T>
    Point<T,2> Point<T,2>::Sub(const Point& v) const
    {
        return Point(x - v.x, y - v.y);
    }

    template<typename T>
    Point<T,2> Point<T,2>::Mul(T v) const
    {
        return Point(x * v, y * v);
    }

    template<typename T>
    Point<T,2> Point<T,2>::Mul(const Point& v) const
    {
        return Point(x * v.x, y * v.y);
    }

    template<typename T>
    Point<T,2> Point<T,2>::Div(T v) const
    {
        return Point(x / v, y / v);
    }

    template<typename T>
    Point<T,2> Point<T,2>::Div(const Point& v) const
    {
        return Point(x / v.x, y / v.y);
    }

    template<typename T>
    Point<T,2> Point<T,2>::RSub(T v) const
    {
        return Point(v - x, v - y);
    }

    template<typename T>
    Point<T,2> Point<T,2>::RSub(const Point& v) const
    {
        return Point(v.x - x, v.y - y);
    }

    template<typename T>
    Point<T,2> Point<T,2>::RDiv(T v) const
    {
        return Point(v / x, v / y);
    }

    template<typename T>
    Point<T,2> Point<T,2>::RDiv(const Point& v) const
    {
        return Point(v.x / x, v.y / y);
    }

    template<typename T>
    void Point<T,2>::IAdd(T v)
    {
        x += v;
        y += v;
    }

    template<typename T>
    void Point<T,2>::IAdd(const Point& v)
    {
        x += v.x;
        y += v.y;
    }

    template<typename T>
    void Point<T,2>::ISub(T v)
    {
        x -= v;
        y -= v;
    }

    template<typename T>
    void Point<T,2>::ISub(const Point& v)
    {
        x -= v.x;
        y -= v.y;
    }

    template<typename T>
    void Point<T,2>::IMul(T v)
    {
        x *= v;
        y *= v;
    }

    template<typename T>
    void Point<T,2>::IMul(const Point& v)
    {
        x *= v.x;
        y *= v.y;
    }

    template<typename T>
    void Point<T,2>::IDiv(T v)
    {
        x /= v;
        y /= v;
    }

    template<typename T>
    void Point<T,2>::IDiv(const Point& v)
    {
        x /= v.x;
        y /= v.y;
    }

    template<typename T>
    const T& Point<T,2>::At(size_t i) const
    {
        assert(i<2);
        return (&x)[i];
    }

    template<typename T>
    T& Point<T,2>::At(size_t i)
    {
        assert(i<2);
        return (&x)[i];
    }

    template<typename T>
    T Point<T,2>::Sum() const
    {
        return x + y;
    }

    template<typename T>
    T Point<T,2>::Min() const
    {
        return std::min(x,y);
    }

    template<typename T>
    T Point<T,2>::Max() const
    {
        return std::max(x,y);
    }

    template<typename T>
    T Point<T,2>::AbsMin() const
    {
        return jet::AbsMin(x,y);
    }

    template<typename T>
    T Point<T,2>::AbsMax() const
    {
        return jet::AbsMax(x,y);
    }

    template<typename T>
    size_t Point<T,2>::DominantAxis() const
    {
        return (std::fabs(x) > std::fabs(y)) ? 0 : 1;
    }

    template<typename T>
    size_t Point<T,2>::SubminantAxis() const
    {
        return (std::fabs(x) < std::fabs(y)) ? 0 : 1;
    }

    template<typename T>
    template<typename U>
    Point2<U> Point<T,2>::CastTo() const
    {
        return Point2<U>(static_cast<U>(x), static_cast<U>(y));
    }

    template<typename T>
    bool Point<T,2>::IsEqual(const Point& other) const
    {
        return (x== other.x && y == other.y);
    }

    template<typename T>
    T& Point<T,2>::operator[](size_t i)
    {
        assert(i<2);
        return (&x)[i];
    }

    template<typename T>
    const T& Point<T,2>::operator[](size_t i) const
    {
        assert(i<2);
        return (&x)[i];
    }

    template<typename T>
    Point<T,2>& Point<T,2>::operator=(const std::initializer_list<T>& list)
    {
        Set(list);
        return (*this);
    }

    template<typename T>
    Point<T,2>& Point<T,2>::operator=(const Point& v)
    {
        Set(v);
        return (*this);
    }

    template<typename T>
    Point<T,2>& Point<T,2>::operator+=(T v)
    {
        IAdd(v);
        return (*this);
    }

    template<typename T>
    Point<T,2>& Point<T,2>::operator+=(const Point& v)
    {
        IAdd(v);
        return (*this);
    }

    template<typename T>
    Point<T,2>& Point<T,2>::operator-=(T v)
    {
        ISub(v);
        return (*this);
    }

    template<typename T>
    Point<T,2>& Point<T,2>::operator-=(const Point& v)
    {
        ISub(v);
        return (*this);
    }

    template<typename T>
    Point<T,2>& Point<T,2>::operator*=(T v)
    {
        IMul(v);
        return (*this);
    }

    template<typename T>
    Point<T,2>& Point<T,2>::operator*=(const Point& v)
    {
        IMul(v);
        return (*this);
    }

    template<typename T>
    Point<T,2>& Point<T,2>::operator/=(T v)
    {
        IDiv(v);
        return (*this);
    }

    template<typename T>
    Point<T,2>& Point<T,2>::operator/=(const Point& v)
    {
        IDiv(v);
        return (*this);
    }

    template<typename T>
    bool Point<T,2>::operator==(const Point& v) const
    {
        return IsEqual(v);
    }

    template<typename T>
    bool Point<T,2>::operator!=(const Point& v) const
    {
        return !IsEqual(v);
    }

    template<typename T>
    Point<T,2> operator+(const Point<T,2>& a)
    {
        return a;
    }
    
    template<typename T>
    Point<T,2> operator-(const Point<T,2>& a)
    {
        return Point<T,2>(-a.x, -a.y);
    }

    template<typename T>
    Point<T,2> operator+(const Point<T,2>& a, T b)
    {
        return a.Add(b);
    }

    template<typename T>
    Point<T,2> operator+(T a, const Point<T,2>& b)
    {
        return b.Add(a);
    }

    template<typename T>
    Point<T,2> operator+(const Point<T,2>& a, const Point<T,2>& b)
    {
        return a.Add(b);
    }

    template<typename T>
    Point<T,2> operator-(const Point<T,2>& a, T b)
    {
        return a.Sub(b);
    }

    template<typename T>
    Point<T,2> operator-(T a, const Point<T,2>& b)
    {
        return b.RSub(a);
    }

    template<typename T>
    Point<T,2> operator-(const Point<T,2>& a, const Point<T,2>& b)
    {
        return a.Sub(b);
    }

    template<typename T>
    Point<T,2> operator*(const Point<T,2>& a, T b)
    {
        return a.Mul(b);
    }

    template<typename T>
    Point<T,2> operator*(T a, const Point<T,2>& b)
    {
        return b.Mul(a);
    }

    template<typename T>
    Point<T,2> operator*(const Point<T,2>& a, const Point<T,2>& b)
    {
        return a.Mul(b);
    }

    template<typename T>
    Point<T,2> operator/(const Point<T,2>& a, T b)
    {
        return a.Div(b);
    }

    template<typename T>
    Point<T,2> operator/(T a, const Point<T,2>& b)
    {
        return b.RDiv(a);
    }

    template<typename T>
    Point<T,2> operator/(const Point<T,2>& a, const Point<T,2>& b)
    {
        return a.Div(b);
    }

    template<typename T>
    Point<T,2> Min(const Point<T,2>& a, const Point<T,2>& b)
    {
        return Point<T,2>(std::min(a.x, b.x), std::min(a.y, b.y));
    }

    template<typename T>
    Point<T,2> Max(const Point<T,2>& a, const Point<T,2>& b)
    {
        return Point<T,2>(std::max(a.x, b.x), std::max(a.y, b.y));
    }

    template<typename T>
    Point<T,2> Clamp( const Point<T,2>& v, const Point<T,2>& low, const Point<T,2>& high)
    {
        return Point<T,2>(Clamp(v.x, low.x, high.x), Clamp(v.y, low.y, high.y));
    }

    template<typename T>
    Point<T,2> Ceil(const Point<T,2>& a)
    {
        return Point<T,2>(std::ceil(a.x), std::ceil(a.y));
    }

    template<typename T>
    Point<T,2> Floor(const Point<T,2>& a)
    {
        return Point<T,2>(std::floor(a.x), std::floor(a.y));
    }

}