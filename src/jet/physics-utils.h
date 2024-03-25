#pragma once

#include <constants.h>
#include <Vector/vector3.h>
#include <algorithm>

namespace jet
{
    inline Vector2D ComputeDragForce(double dragCoeff, double radius, const Vector2D& velocity)
    {
        //! Stoke's drag law assuming that Reynold's number is very low.
        return -6.0 * kPiD * dragCoeff * radius * velocity;
    }

    inline Vector3D ComputeDragForce(double dragCoeff, double radius, const Vector3D& velocity)
    {
        //! Stoke's drag law assuming that Reynold's number is very low.
        return -6.0 * kPiD * dragCoeff * radius * velocity;
    }

    template<size_t N>
    inline Vector<double, N> ProjectAndApplyFriction(const Vector<double, N>& vel,
                            const Vector<double, N>& normal, double frictionCoeff)
    {
        Vector<double, N> velt = vel.Projected(normal);
        if (velt.LengthSquared() > 0)
        {
            double veln = std::max(-vel.Dot(normal), 0.0);
            velt *= std::max(1.0 - frictionCoeff * veln/ velt.Length(), 0.0);
        }

        return velt;
    }

    inline double ComputePressureFromEOS(double density, double targetDensity,
                        double EOSScale, double EOSExponent, double negativePressureScale)
    {
        double p = EOSScale / EOSExponent * (std::pow((density/targetDensity),EOSExponent) - 1.0);

        // Negative pressure scaling
        if (p < 0)
            p *= negativePressureScale;
        
        return p;
    }

}