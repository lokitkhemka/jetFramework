#pragma once

#include <macros.h>
#include <constants.h>
#include <cstddef>
#include <limits>
#include <algorithm>
#include <cmath>


namespace jet
{
    //!
    //! \brief Returns true if \p x and \p y are similar.
    //!
    //! \param[in] x First value
    //! \param[in] y Second value
    //! \param[in] eps Tolerance
    //!
    //! \tparam T Value type
    //! \return true if similar
    //!

    template<typename T>
    inline bool Similar(T x, T y, T eps = std::numeric_limits<T>::epsilon())
    {
        return (std::abs(x-y) <= eps);
    }

    //!
    //! \brief Returns the sign of the value.
    //! \param[in] x Input value
    //! \tparam T Value type
    //!
    //! \return Sign
    //!
    template<typename T>
    inline T Sign(T x)
    {
        if (x >= 0)
            return 1;
        else
            return -1;
    }

    //! \brief Returns the minimum value among three inputs.
    //!
    //! \param[in] x First value
    //! \param[in] y Second value
    //! \param[in] z Third value
    //!
    //! \tparam T Value type
    //! \return Minimum value
    template<typename T> 
    inline T Min3(T x, T y, T z)
    {
        return std::min(std::min(x,y), z);
    }

    //! \brief Returns the maximum value among three inputs.
    //!
    //! \param[in] x First value
    //! \param[in] y Second value
    //! \param[in] z Third value
    //!
    //! \tparam T Value type
    //! \return Maximum value
    template<typename T> 
    inline T Max3(T x, T y, T z)
    {
        return std::max(std::max(x,y),z);
    }

    //! Returns minimum among \p n elements stored in \p x
    template<typename T>
    inline T MinN(const T* x, size_t n)
    {
        T m = x[0];
        for (size_t i = 1; i<n; ++i)
        {
            m = std::min(m, x[i]);
        }
        return m;
    }

    //! Returns the maximum among \p n element stored in \p x
    template<typename T>
    inline T MaxN(const T* x, size_t n)
    {
        T m = x[0];
        for (size_t i = 1; i<n; ++i)
        {
            m = std::max(m, x[i]);
        }
        return m;
    }

    //! \brief Returns the absolute minimum value among two inputs.
    //!
    //! \param[in] x First value
    //! \param[in] y Second value
    //!
    //! \tparam T Value type
    //! \return Absolute Minimum value
    template<typename T> 
    inline T AbsMin(T x, T y)
    {
        return (x*x < y*y) ? x : y;
    }

    //! \brief Returns the absolute maximum value among two inputs.
    //!
    //! \param[in] x First value
    //! \param[in] y Second value
    //!
    //! \tparam T Value type
    //! \return Absolute Maximum value
    template<typename T> 
    inline T AbsMax(T x, T y)
    {
        return (x*x > y*y) ? x : y;
    }

    //! Returns absolute minimum among \p n elements stored in \p x
    template<typename T>
    inline T AbsMinN(const T* x, size_t n)
    {
        T m = x[0];
        for (size_t i = 0; i<n; ++i)
        {
            m = AbsMin(m, x[i]);
        }
        return m;
    }

    //! Returns the absolute maximum among \p n element stored in \p x
    template<typename T>
    inline T AbsMaxN(const T* x, size_t n)
    {
        {
        T m = x[0];
        for (size_t i = 0; i<n; ++i)
        {
            m = AbsMax(m, x[i]);
        }
        return m;
    }
    }

    //! Returns the square of \p x.
    template<typename T>
    inline T Square(T x)
    {
        return x*x;
    }

    //! Returns the cube of \p x.
    template<typename T>
    inline T Cube(T x)
    {
        return x*x*x;
    }

    //! Returns the clamped value.
    template <typename T>
    inline T Clamp(T val, T low, T high)
    {
        if (val<low)
            return low;
        else if(val > high)
            return high;
        else
            return val;
    }

    //! Converts Degrees to Radians
    template<typename T>
    inline T DegreesToRadians(T AngleInDegrees)
    {
        return AngleInDegrees * Pi<T>() / 180;
    }

    //! Converts Radians to Degrees
    template<typename T>
    inline T RadiansToDegrees(T AngleInRadians)
    {
        return AngleInRadians * 180 * InvPi<T>();
    }

    //!
    //! \brief Computes the Barycentric coordinates.
    //! \param[in] x The Input Value
    //! \param[in] iLow  Lowest Index.
    //! \param[in] iHigh Highest Index.
    //! \param i The output index.
    //! \param t The offset from \p i.
    //!
    //! \tparam T Value Type.

    template<class T>
    inline void GetBarycentric(T x, ssize_t iLow, ssize_t iHigh, ssize_t* i, T* t)
    {
        T s = std::floor(x);
        *i = static_cast<ssize_t>(s);

        ssize_t offset = -iLow;
        iLow += offset;
        iHigh += offset;

        if (iLow == iHigh)
        {
            *i = iLow;
            *t = 0;
        } else if(*i < iLow)
        {
            *i = iLow;
            *t = 0;
        } else if(*i > iHigh - 1)
        {
            *i = iHigh - 1;
            *t = 1;
        } else {
            *t = static_cast<T>(x-s);
        }

        *i -= offset;
    }


    //! \brief Computes Linear Interpolation
    //!
    //! \param[in] f0 The first value
    //! \param[in] f1 The second value
    //! \param[in] t Relative offset [0,1] from the first value.
    //!
    //! \tparam S Input Value type.
    //! \tparam T Offset type
    //!
    //! \return The interpolated value.
    template<typename S, typename T>
    inline S Lerp(const S& f0, const S& f1, T t)
    {
        return (1-t)*f0 + t*f1;
    }

    //! Compute bilinear interpolation
    template<typename S, typename T>
    inline S Bilerp(
        const S& f00, const S& f10,
        const S& f01, const S& f11,
        T tx, T ty)
        {
            return Lerp(Lerp(f00, f10, tx), Lerp(f01, f11, tx), ty);
        }
    
    
    //! Compute trilinear interpolation
    template<typename S, typename T>
    inline S Trilerp(
        const S& f000, const S& f100,
        const S& f010, const S& f110,
        const S& f001, const S& f101,
        const S& f011, const S& f111,
        T tx, T ty, T tz)
        {
            return Lerp(Bilerp(f000, f100, f010, f110, tx, ty),
                        Bilerp(f001, f101, f011, f111, tx, ty), tz);
        }

    //! Compute Catmull-Rom Interpolation.
    template<typename S, typename T>
    inline S CatmullRom(const S& f0, const S& f1, 
                        const S& f2, const S& f3, T t)
                        {
                            S d1 = (f2 - f0)/2;
                            S d2 = (f3 - f1)/2;
                            S D1 = f2 - f1;

                            S a3 = d1 + d2 - 2 * D1;
                            S a2 = 3 * D1 - 2 * d1 - d2;
                            S a1 = d1;
                            S a0 = f1;

                            return a3 * Cube(t) + a2*Square(t) + a1*t + a0;
                        }


    //! Compute Monotonic Catmull-Rom Interpolation.
    template<typename T>
    inline T MonotonicCatmullRom(const T& f0, const T& f1, 
                        const T& f2, const T& f3, T t)
                        {
                            T d1 = (f2 - f0)/2;
                            T d2 = (f3 - f1)/2;
                            T D1 = f2 - f1;

                            if (std::fabs(D1) < kEpsilonD)
                                d1 = d2 = 0;
                            
                            if(Sign(D1) != Sign(d1))
                                d1 = 0;
                            
                            if (Sign(D1) != Sign(d2))
                                d2 = 0;

                            T a3 = d1 + d2 - 2 * D1;
                            T a2 = 3 * D1 - 2 * d1 - d2;
                            T a1 = d1;
                            T a0 = f1;

                            return a3 * Cube(t) + a2*Square(t) + a1*t + a0;

                        }



}