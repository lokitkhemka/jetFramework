#pragma once

// Copyright (c) 1998-2014, Matt Pharr and Greg Humphreys.
// All rights reserved.

// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:

// Redistributions of source code must retain the above copyright notice, this
// list of conditions and the following disclaimer.
// Redistributions in binary form must reproduce the above copyright notice,
// this list of conditions and the following disclaimer in the documentation
// and/or other materials provided with the distribution.
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#include <Vector/vector2.h>
#include <Vector/vector3.h>

#include <constants.h>
#include <math-utils.h>
#include <algorithm>

namespace jet
{
    //! \brief Returns a randomly sampled direction within a cone.
    //!
    //! For a given cone, defined by axis and angle, this function returns a
    //! sampled direction within the cone.
    //!
    //! \param[in] u1 First Random Sample.
    //! \param[in] u2 Second Random Sample.
    //! \param[in] axis The axis of the cone.
    //! \param[in] angle The angle of the cone.
    //!
    //! \tparam T Real Number Type
    //!
    //! \return Sampled Direction Vector.
    template<typename T> inline Vector3<T> UniformSampleCone(T u1, T u2, const Vector3<T>& axis,
                T angle);

    //! \brief Returns a randomly sampled point within a unit hemisphere.
    //!
    //! For a given unit hemisphere, defined by center normal vector, this function returns a
    //! point within the hemisphere.
    //!
    //! \param[in] u1 First Random Sample.
    //! \param[in] u2 Second Random Sample.
    //! \param[in] normal The center normal of the hemisphere.
    //!
    //! \tparam T Real Number Type
    //!
    //! \return Sampled Point.
    template<typename T> inline Vector3<T> UniformSampleHemisphere(T u1, T u2, const Vector3<T>& normal);

    //! \brief Returns a weighted sampled point within a unit hemisphere.
    //!
    //! For a given unit hemisphere, defined by center normal vector, this function returns a
    //! point within the hemisphere.
    //!
    //! \param[in] u1 First Random Sample.
    //! \param[in] u2 Second Random Sample.
    //! \param[in] normal The center normal of the hemisphere.
    //!
    //! \tparam T Real Number Type
    //!
    //! \return Sampled Point.
    template<typename T> inline Vector3<T> CosineWeightedSampleHemisphere(T u1, T u2, const Vector3<T>& normal);


    //! \brief Returns a randomly sampled point on a sphere.
    //!
    //! For a given sphere, defined by center normal vector, this function returns a
    //! point on the sphere.
    //!
    //! \param[in] u1 First Random Sample.
    //! \param[in] u2 Second Random Sample.
    //!
    //! \tparam T Real Number Type
    //!
    //! \return Sampled Point.
    template<typename T> inline Vector3<T> UniformSampleSphere(T u1, T u2);

    //! \brief Returns a randomly sampled point on a disk.
    //!
    //! For a given disk, defined by center normal vector, this function returns a
    //! point on the disk.
    //!
    //! \param[in] u1 First Random Sample.
    //! \param[in] u2 Second Random Sample.
    //!
    //! \tparam T Real Number Type
    //!
    //! \return Sampled Point.
    template<typename T> inline Vector2<T> UniformSampleDisk(T u1, T u2);

    template<typename T> inline Vector3<T> UniformSampleCone(T u1, T u2, const Vector3<T>& axis,
                T angle)
    {
        T CosHalfAngle = std::cos(angle/2);
        T y = 1 - (1- CosHalfAngle) * u1;
        T r = std::sqrt(std::max<T>(0, 1 - y*y));
        T phi = TwoPi<T>() * u2;
        T x = r * std::cos(phi);
        T z = r * std::sin(phi);
        auto ts = axis.Tangential();
        return std::get<0>(ts) * x + axis * y + std::get<1>(ts) * z;
    }

    template<typename T> inline Vector3<T> UniformSampleHemisphere(T u1, T u2, const Vector3<T>& normal)
    {
        T y = u1;
        T r = std::sqrt(std::max<T>(0, 1 - y*y));
        T phi = TwoPi<T>() * u2;
        T x = r * std::cos(phi);
        T z = r * std::sin(phi);
        auto ts = normal.Tangential();
        return std::get<0>(ts) * x + normal * y + std::get<1>(ts) * z;
    }

    template<typename T> inline Vector3<T> CosineWeightedSampleHemisphere(T u1, T u2, 
            const Vector3<T>& normal)
    {
        T phi = TwoPi<T>() * u1;
        T y = std::sqrt(u2);
        T theta = std::acos(y);
        T x = std::cos(phi) * std::sin(theta);
        T z = std::sin(phi) * std::sin(theta);
        Vector3<T> t = Tangential(normal);
        auto ts = normal.Tangential();
        return std::get<0>(ts) * x + normal * y + std::get<1>(ts) * z;
    }

    template<typename T> inline Vector3<T> UniformSampleSphere(T u1, T u2)
    {
        T y = 1 - 2 * u1;
        T r = std::sqrt(std::max<T>(0, 1- y * y));
        T phi = TwoPi<T>() * u2;
        T x = r * std::cos(phi);
        T z = r * std::sin(phi);
        return Vector3<T>(x, y, z);
    }

    template<typename T> inline Vector2<T> UniformSampleDisk(T u1, T u2)
    {
        T r = std::sqrt(u1);
        T theta = TwoPi<T>() * u2;

        return Vector2<T>(r * std::cos(theta), r * std::sin(theta));
    }

}