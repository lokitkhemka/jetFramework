#pragma once

#include <macros.h>
#include <cmath>
#include <limits>

//! Float-type epsilon.
constexpr float kEpsilonF = std::numeric_limits<float>::epsilon();

//! Double-type epsilon.
constexpr double kEpsilonD = std::numeric_limits<double>::epsilon();

// MARK: Max

//! Max size_t.
constexpr size_t kMaxSize = std::numeric_limits<size_t>::max();

//! Max ssize_t.
constexpr ssize_t kMaxSSize = std::numeric_limits<ssize_t>::max();

//! Max float.
constexpr float kMaxF = std::numeric_limits<float>::max();

//! Max double.
constexpr double kMaxD = std::numeric_limits<double>::max();

//! Float-type pi.
constexpr float kPiF = 3.14159265358979323846264338327950288f;

//! Double-type pi.
constexpr double kPiD = 3.14159265358979323846264338327950288;

//! Pi for type T.
template <typename T>
constexpr T Pi() {
    return static_cast<T>(kPiD);
}

//! Pi for float.
template <>
constexpr float Pi<float>() {
    return kPiF;
}

//! Pi for double.
template <>
constexpr double Pi<double>() {
    return kPiD;
}

//! Float-type pi/2.
constexpr float kHalfPiF = 1.57079632679489661923132169163975144f;

//! Double-type pi/2.
constexpr double kHalfPiD = 1.57079632679489661923132169163975144;

//! Pi/2 for type T.
template <typename T>
constexpr T HalfPi() {
    return static_cast<T>(kHalfPiD);
}

//! Pi/2 for float.
template <>
constexpr float HalfPi<float>() {
    return kHalfPiF;
}

//! Pi/2 for double.
template <>
constexpr double HalfPi<double>() {
    return kHalfPiD;
}


//! Float-type pi/4.
constexpr float kQuarterPiF = 0.785398163397448309615660845819875721f;

//! Double-type pi/4.
constexpr double kQuarterPiD = 0.785398163397448309615660845819875721;

//! Pi/4 for type T.
template <typename T>
constexpr T QuarterPi() {
    return static_cast<T>(kQuarterPiD);
}

//! Pi/2 for float.
template <>
constexpr float QuarterPi<float>() {
    return kQuarterPiF;
}

//! Pi/2 for double.
template <>
constexpr double QuarterPi<double>() {
    return kQuarterPiD;
}


//! Float-type 2*pi.
constexpr float kTwoPiF = static_cast<float>(2.0 * kPiD);

//! Double-type 2*pi.
constexpr double kTwoPiD = 2.0 * kPiD;

//! 2*pi for type T.
template <typename T>
constexpr T TwoPi() {
    return static_cast<T>(kTwoPiD);
}

//! 2*pi for float.
template <>
constexpr float TwoPi<float>() {
    return kTwoPiF;
}

//! 2*pi for double.
template <>
constexpr double TwoPi<double>() {
    return kTwoPiD;
}


//! Float-type 1/pi.
constexpr float kInvPiF = static_cast<float>(1.0 / kPiD);

//! Double-type 1/pi.
constexpr double kInvPiD = 1.0 / kPiD;

//! 1/pi for type T.
template <typename T>
constexpr T InvPi() {
    return static_cast<T>(kInvPiD);
}

//! 1/pi for float.
template <>
constexpr float InvPi<float>() {
    return kInvPiF;
}

//! 1/pi for double.
template <>
constexpr double InvPi<double>() {
    return kInvPiD;
}

//! Float-type 1/2*pi.
constexpr float kInvTwoPiF = static_cast<float>(0.5 / kPiD);

//! Double-type 1/2*pi.
constexpr double kInvTwoPiD = 0.5 / kPiD;

//! 1/2*pi for type T.
template <typename T>
constexpr T InvTwoPi() {
    return static_cast<T>(kInvTwoPiD);
}

//! 1/2*pi for float.
template <>
constexpr float InvTwoPi<float>() {
    return kInvTwoPiF;
}

//! 1/2*pi for double.
template <>
constexpr double InvTwoPi<double>() {
    return kInvTwoPiD;
}