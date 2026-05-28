#pragma once

#include <queue>

namespace latero {

class ButtonDebouncer
{
public:
    /**
     * Debounces button readings and maintains state.
     * @param debouncingTime Time during which reading must be stable to be accepted.
     */
    ButtonDebouncer(boost::posix_time::time_duration debouncingTime) :
        state_(false), upEvent_(false), downEvent_(false),
        reading_(false),
        timeLastToggle_(boost::posix_time::microsec_clock::universal_time()),
        debouncing_time(debouncingTime)
    {
    }

    /**
     * Update button with current reading.
     * @param v true if button is read as down (pressed)
     */
    void UpdateState(bool v)
    {
        upEvent_ = downEvent_ = false;
        if (reading_ != v)
        {
            timeLastToggle_ = boost::posix_time::microsec_clock::universal_time();
            reading_ = v;
        }
        else if (reading_ != state_)
        {
            if ((boost::posix_time::microsec_clock::universal_time()-timeLastToggle_) > debouncing_time)
            {
                state_ = reading_;
                downEvent_ = state_;
                upEvent_ = !state_;
            }
        }
    }

    inline bool IsDown() const { return state_; }
    inline bool UpEvent() const { return upEvent_; }
    inline bool DownEvent() const { return downEvent_; }

protected:
    bool state_; // current state (true if pressed)
    bool upEvent_, downEvent_; // instantaneous events when button comes up or down

    bool reading_; // last button reading
    boost::posix_time::ptime timeLastToggle_; // time at which reading last changed
    const boost::posix_time::time_duration debouncing_time; // time during which reading must be stable
};
    
    
}; // latero


