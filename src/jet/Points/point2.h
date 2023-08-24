#pragma once

#include <Points/point.h>
#include <algorithm>

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
        const T& At*(size_t i) const;

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

   

}