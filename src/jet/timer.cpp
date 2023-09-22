#include<jet.h>
#include"timer.h"

namespace jet
{
    Timer::Timer()
    {
        _startingPoint = _clock.now();
    }

    double Timer::DurationInSeconds() const
    {
        auto end = std::chrono::steady_clock::now();

        auto count = std::chrono::duration_cast<std::chrono::microseconds>(end - _startingPoint).count();
        return count/1000000.0;
    }

    void Timer::Reset()
    {
        _startingPoint = _clock.now();
    }
}