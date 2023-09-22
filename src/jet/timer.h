#pragma once

#include<chrono>

namespace jet
{
    //! Simple timer class.
    class Timer
    {
    public:
        //! Constructs a timer and start ticking.
        Timer();

        //! Returns the time duration since the creation of reset in Seconds.
        double DurationInSeconds() const;

        //! Resets the timer.
        void Reset();

    private:
        std::chrono::steady_clock _clock;
        std::chrono::steady_clock::time_point _startingPoint;
    };
}