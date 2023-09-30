#include <jet.h>
#include "animation.h"
#include <timer.h>

namespace jet
{
    Frame::Frame()
    {}

    Frame::Frame(unsigned int newIndex, double newTimeIntervalInSeconds)
                : Index(newIndex), TimeIntervalInSeconds(newTimeIntervalInSeconds)
    {}

    double Frame::TimeInSeconds() const
    {
        return Index * TimeIntervalInSeconds;
    }

    void Frame::Advance()
    {
        ++Index;
    }

    void Frame::Advance(unsigned int delta)
    {
        Index += delta;
    }

    Frame& Frame::operator++()
    {
        Advance();
        return *this;
    }

    Frame Frame::operator++(int i)
    {
        UNUSED_VARAIBLE(i);

        Frame result = *this;
        Advance();
        return result;
    }

    Animation::Animation()
    {}

    Animation::~Animation()
    {}

    void Animation::Update(const Frame& frame)
    {
        Timer timer;

        JET_INFO << "Begin Updating Frame: " << frame.Index 
                    << "TimeIntervalInSeconds: " << frame.TimeIntervalInSeconds
                    << "(1/" << 1.0 / frame.TimeIntervalInSeconds
                    <<") seconds";
        OnUpdate(frame);

        JET_INFO << "End Updating Frame (took " << timer.DurationInSeconds()
                    << " seconds)";
    }
}