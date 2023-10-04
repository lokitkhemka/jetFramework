#pragma once

#include<Animation/animation.h>

namespace jet
{
    //! \brief Abstract Base class for physics-based animation.

    //! This class represents physics based animation by adding time-integration
    //! specific functions to Animation class.
    class PhysicsAnimation : public Animation
    {
    public:
        //! Default Constructor
        PhysicsAnimation();

        //! Destructor
        virtual ~PhysicsAnimation();

        //! \brief Returns true if fixed sub-timestepping is used.
        //! When performing a time-integration, it is often required to take
        //! sub-timestepping for better results. The sub-stepping can be either
        //! fixed or adaptive and this function returns which feature is currently
        //! selected.
        //! \return true if using fixed sub-time steps, false otherwise
        bool IsUsingFixedSubTimeSteps() const;

        
        
        //! \brief Sets true if fixed sub-timestepping is used.
        //!
        //! \param[in] IsUsing true to enable fixed sub-stepping
        void SetIsUsingFixedSubTimeSteps(bool IsUsing);



        //! \brief Returns the number of fixed sub-timesteps.
        //!
        //! \return The number of fixed sub-timesteps.
        unsigned int NumberOfFixedSubTimeSteps() const;


        //! \brief Sets the number of fixed sub-timesteps.
        //!
        //! \param[in] NumberOfSteps The number of fixed sub-timesteps.
        void SetNumberOfFixedSubTimeSteps(unsigned int NumberOfSteps);

        //! Advances a single frame.
        void AdvanceSingleFrame();

        //! \brief Returns the current frame.
        Frame CurrentFrame() const;

        //! \brief Sets current frame cursor, without invoking Update() method
        void SetCurrentFrame(const Frame& frame);

        //! \brief Returns current time in seconds.
        //!
        //! This function returns the current time which is calculated by adding
        //! current frame time + sub-timesteps it passed.
        double CurrentTimeInSeconds() const;



    protected:
        //! \brief Called when a single time-step should be advanced.
        //!
        //! When Animation::Update function is called, this class will internally
        //! subdivide a frame into sub-steps if needed. Each substep is then taken
        //! to move forward in time. This function is called for each substep, and
        //! a subclass that inherits PhysicsAnimation class should implement this function
        //! for its own physics model.
        //!
        //! \param[in] TimeIntervalInSeconds The time interval in seconds
        virtual void OnAdvanceSubTimeStep(double TimeIntervalInSeconds) = 0;



        //! \brief Returns the required number of sub-steps for given time interval.
        //!
        //! The number of sub-steps can be different depending on the physics model.
        //! This function should be overrriden to implement own logic for model specific
        //! sub-stepping for given time interval.
        //!
        //! \param[in] TimeIntervalInSeconds The time interval in seconds.
        //!
        //! \return Required number of sub-steps.
        virtual unsigned int NumberOfSubTimeSteps(double TimeIntervalInSeconds) const;


        //! \brief Called at frame 0 to intialize the physics state.
        //!
        //! Inheriting classes can override this function to setup initial conditions
        //! for the simulation.
        virtual void OnInitialize();


    private:
        Frame _CurrentFrame;
        bool _IsUsingFixedSubTimeSteps = true;
        unsigned int _NumberOfFixedSubTimeSteps = 1;
        bool _HasInitialized = false;
        double  _CurrentTime = 0.0;

        void OnUpdate(const Frame& frame) final;
        void AdvanceTimeStep(double TimeIntervalInSeconds);
        void Initialize();
    };

    typedef std::shared_ptr<PhysicsAnimation> PhysicsAnimationPtr;
}
