#include <jet.h>
#include <constants.h>
#include "physics_animation.h"
#include <timer.h>
#include <limits>

namespace jet
{
    PhysicsAnimation::PhysicsAnimation()
    {}

    PhysicsAnimation::~PhysicsAnimation()
    {}

    bool PhysicsAnimation::IsUsingFixedSubTimeSteps() const
    {
        return _IsUsingFixedSubTimeSteps;
    }

    void PhysicsAnimation::SetIsUsingFixedSubTimeSteps(bool IsUsing)
    {
        _IsUsingFixedSubTimeSteps = IsUsing;
    }

    unsigned int PhysicsAnimation::NumberOfFixedSubTimeSteps() const
    {
        return _NumberOfFixedSubTimeSteps;
    }

    void PhysicsAnimation::SetNumberOfFixedSubTimeSteps(unsigned int NumberOfSteps)
    {
        _NumberOfFixedSubTimeSteps = NumberOfSteps;
    }

    void PhysicsAnimation::AdvanceSingleFrame()
    {
        ++_CurrentFrame;
        Update(_CurrentFrame);
    }

    Frame PhysicsAnimation::CurrentFrame() const
    {
        return _CurrentFrame;
    }

    void PhysicsAnimation::SetCurrentFrame(const Frame& frame)
    {
        _CurrentFrame = frame;
    }

    double PhysicsAnimation::CurrentTimeInSeconds() const
    {
        return _CurrentTime;
    }

    unsigned int PhysicsAnimation::NumberOfSubTimeSteps(double TimeIntervalInSeconds) const
    {
        UNUSED_VARIABLE(TimeIntervalInSeconds);

        // Returns the number of fixed sub-timesteps by default
        return _NumberOfFixedSubTimeSteps;
    }

    void PhysicsAnimation::OnUpdate(const Frame& frame)
    {
        if(frame.Index > _CurrentFrame.Index)
        {
            unsigned int NumberOfFrames = frame.Index - _CurrentFrame.Index;

            for(unsigned int i = 0; i < NumberOfFrames; ++i)
                AdvanceTimeStep(frame.TimeIntervalInSeconds);

            _CurrentFrame = frame;
        }
        else if(frame.Index == 0 && !_HasInitialized)
        {
            Initialize();
        }
    }

    void PhysicsAnimation::AdvanceTimeStep( double TimeIntervalInSeconds)
    {
        _CurrentTime = _CurrentFrame.TimeInSeconds();

        if(_IsUsingFixedSubTimeSteps)
        {
            JET_INFO << "Using Fixed Sub-timesteps: " << _NumberOfFixedSubTimeSteps;

            // Performing Fixed Time-stepping
            const double ActualTimeInterval = TimeIntervalInSeconds / static_cast<double>(_NumberOfFixedSubTimeSteps);

            for(unsigned int i = 0; i < _NumberOfFixedSubTimeSteps; ++i)
            {
                JET_INFO << "Begin OnAdvanceSubTimeStep: " << ActualTimeInterval
                            << "(1/ " << 1.0/ActualTimeInterval << ") seconds";

                Timer timer;
                OnAdvanceSubTimeStep(ActualTimeInterval);

                JET_INFO << "End OnAdvanceSubTimeStep (took " << timer.DurationInSeconds() << " seconds)";

                _CurrentTime += ActualTimeInterval;
            }
        }
        else
        {
            JET_INFO << "Using Adaptive sub-timesteps.";

            //Performing Adaptive Time-stepping.
            double RemainingTime = TimeIntervalInSeconds;
            while (RemainingTime > kEpsilonD)
            {
                unsigned int NumSteps = NumberOfSubTimeSteps(RemainingTime);
                double ActualTimeInterval = RemainingTime / static_cast<double>(NumSteps);

                JET_INFO << "Number of remaining sub-timesteps: " << NumSteps;

                JET_INFO << "Being OnAdvanceSubTimeStep: " << ActualTimeInterval
                            << "(1/" << 1.0 / ActualTimeInterval << ") seconds";
                
                Timer timer;
                OnAdvanceSubTimeStep(ActualTimeInterval);

                JET_INFO << "End OnAdvanceSubTimeStep (took " << timer.DurationInSeconds()
                                << " seconds)";

                RemainingTime -= ActualTimeInterval;
                _CurrentTime += ActualTimeInterval;
            }
        }
    }


    void PhysicsAnimation::Initialize()
    {
        OnInitialize();
        _HasInitialized = true;
    }

    void PhysicsAnimation::OnInitialize()
    {}
}