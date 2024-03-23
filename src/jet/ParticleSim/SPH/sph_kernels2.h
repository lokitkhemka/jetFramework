#pragma once

#include <constants.h>
#include <Vector/vector2.h>

namespace jet
{
    //! \brief Standar 2D SPH kernel function object
    struct SPHStdKernel2
    {
        //! Kernel Radius
        double h;

        //! Square of the kernel radius
        double h2;

        //! Cube of the kernel radius
        double h3;

        //! Fourth-power of the kernel radius
        double h4;

        //! Constructs a Kernel object with zero radius.
        SPHStdKernel2();

        //! Constructs a Kernel object with given radius
        explicit SPHStdKernel2(double radius);

        //! Copy Constructor
        SPHStdKernel2(const SPHStdKernel2& other);

        //! Returns kernel function value at given distance.
        double operator()(double distance) const;

        //! Returns the first derivative at given distance.
        double FirstDerivative(double distance) const;

        //! Returns the gradient at a point.
        Vector2D Gradient(const Vector2D& point) const;

        //! Returns the graident at a point defined by distance and direction.
        Vector2D Gradient(double distance, const Vector2D& direction) const;

        //! Returns the second derivative at a given distance.
        double SecondDerivative(double distance) const;
    };

    //! Spiky 2D SPH kernel function object.

    struct SPHSpikyKernel2
    {
        //! Kernel Radius
        double h;

        //! Square of the kernel radius
        double h2;

        //! Cube of the kernel radius
        double h3;

        //! Fourth-power of the kernel radius
        double h4;

        //! Fifth-power of the kernel radius
        double h5;

        //! Constructs a Kernel object with zero radius.
        SPHSpikyKernel2();

        //! Constructs a Kernel object with given radius
        explicit SPHSpikyKernel2(double radius);

        //! Copy Constructor
        SPHSpikyKernel2(const SPHSpikyKernel2& other);

        //! Returns kernel function value at given distance.
        double operator()(double distance) const;

        //! Returns the first derivative at given distance.
        double FirstDerivative(double distance) const;

        //! Returns the gradient at a point.
        Vector2D Gradient(const Vector2D& point) const;

        //! Returns the graident at a point defined by distance and direction.
        Vector2D Gradient(double distance, const Vector2D& direction) const;

        //! Returns the second derivative at a given distance.
        double SecondDerivative(double distance) const;
    };


    inline SPHStdKernel2::SPHStdKernel2()
        : h(0), h2(0), h3(0), h4(0)
    {}

    inline SPHStdKernel2::SPHStdKernel2(double h_)
        : h(h_), h2(h*h), h3(h2 * h), h4(h2 * h2)
    {}

    inline SPHStdKernel2::SPHStdKernel2(const SPHStdKernel2& other)
        :h(other.h), h2(other.h2), h3(other.h3), h4(other.h4)
    {}

    inline double SPHStdKernel2::operator()(double distance) const
    {
        double distanceSq = distance * distance;

        if (distanceSq >= h2)
        {
            return 0.0;
        }
        else
        {
            double x = 1.0 - distanceSq / h2;
            return 4.0 / (kPiD * h2) * x * x * x;
        }
    }

    inline double SPHStdKernel2::FirstDerivative(double distance) const
    {
        if (distance >= h)
        {
            return 0.0;
        }
        else
        {
            double x = 1.0- distance * distance / h2;
            return -24.0 * distance / (kPiD * h4) * x * x;
        }
    }

    inline Vector2D SPHStdKernel2::Gradient(const Vector2D& point) const
    {
        double dist = point.Length();
        if (dist > 0.0)
        {
            return Gradient(dist, point/dist);
        }
        else
        {
            return Vector2D(0,0);
        }
    }

    inline Vector2D SPHStdKernel2::Gradient(double distance, const Vector2D& directionToCenter) const
    {
        return -FirstDerivative(distance) * directionToCenter;
    }

    inline double SPHStdKernel2::SecondDerivative(double distance) const
    {
        double distanceSq = distance * distance;

        if (distanceSq >= h2)
        {
            return 0.0;
        }
        else
        {
            double x = distanceSq / h2;
            return 24.0 / (kPiD * h4) * (1 - x) * (5 * x - 1);
        }
    }

    inline SPHSpikyKernel2::SPHSpikyKernel2()
        : h(0), h2(0), h3(0), h4(0), h5(0)
    {}

    inline SPHSpikyKernel2::SPHSpikyKernel2(double h_)
        : h(h_), h2(h * h), h3(h2 * h), h4(h2 * h2), h5(h3 * h2)
    {}

    inline SPHSpikyKernel2::SPHSpikyKernel2(const SPHSpikyKernel2& other)
        : h(other.h), h2(other.h2), h3(other.h3), h4(other.h4), h5(other.h5)
    {}

    inline double SPHSpikyKernel2::operator()(double distance) const
    {
        if (distance >= h)
        {
            return 0.0;
        }
        else
        {
            double x = 1.0 - distance / h;
            return 10.0 / (kPiD * h2) * x * x * x;
        }
    }

    inline double SPHSpikyKernel2::FirstDerivative(double distance) const
    {
        if (distance >= h)
        {
            return 0.0;
        }
        else
        {
            double x = 1.0 - distance / h;
            return -30.0 / (kPiD * h3) * x * x;
        }
    }

    inline Vector2D SPHSpikyKernel2::Gradient(const Vector2D& point) const
    {
        double dist = point.Length();
        if (dist > 0.0)
        {
            return Gradient(dist, point/dist);
        }
        else
            return Vector2D(0,0);
    }

    inline Vector2D SPHSpikyKernel2::Gradient(double distance, const Vector2D& directionToCenter) const
    {
        return -FirstDerivative(distance) * directionToCenter;
    }

    inline double SPHSpikyKernel2::SecondDerivative(double distance) const
    {
        if (distance >= h)
            return 0.0;
        else
        {
            double x = 1.0 - distance / h;
            return 60.0 / (kPiD * h4) * x;
        }
    }
}