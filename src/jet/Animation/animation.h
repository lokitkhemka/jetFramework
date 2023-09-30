#pragma once

#include<macros.h>
#include<memory>

namespace jet
{
    //! \brief Rerpesentation of an animation frame.
    //! This struct holds current animation frame index and frame interval in seconds.

    struct Frame final
    {
        //! Frame Index
        unsigned int Index = 0;

        //! Time Interval between two adjacent frames.
        double TimeIntervalInSeconds = 1.0/ 60.0;

        //! Constructs Frame instance with 1/60 seconds time interval.
        Frame();

        //! Constructs Frame instance with given time interval.
        Frame(unsigned int newIndex, double newTimeIntervalInSeconds);

        //! Returns the elapsed time in seconds.
        double TimeInSeconds() const;

        //! Advances a single frame
        void Advance();

        //! Advances multiple frames.
        //! \param delta Number of frames to advance.
        void Advance(unsigned int delta);

        //! Advances a single frame (prefix)
        Frame& operator++();

        //! Advances a single frame (post fix)
        Frame operator++(int);
    };


    //! \brief Abstract base class for animation.
    //!
    //! This class represents the interface for animation.
    //! Because animation is a function of time and/or its previous state, this class provides
    //! a virtual function Update() which can be overriden by its sub-classes to implement
    //! their own state update logic.
    class Animation
    {
    public:
        Animation();
        
        virtual ~Animation();

        //! \brief Updates animation state for a given \p frame.
        //!
        //! This function updates animation state by calling Animation::OnUpdate
        //! function.
        void Update(const Frame& frame);
    protected:
        //! \brief The implementation of this function should update the animation state for given Frame instance \p frame.
        //!
        //! This funciton is called from Animation::Update when state of this class instance needs to be
        //! updated. The inherited class should override this funciton.
        virtual void  OnUpdate(const Frame& frame) = 0;

    };

    typedef std::shared_ptr<Animation> AnimationPtr;

}