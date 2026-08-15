#pragma once

#include <queue>
#include <chrono>

namespace latero {

// TODO: Updated to std::chrono but could not be tested without a Tactograph.

class ButtonDebouncer
{
public:
    /**
     * Debounces button readings and maintains state.
     * @param debouncingTime Time during which reading must be stable to be accepted.
     */
    ButtonDebouncer(std::chrono::milliseconds debouncingTime) :
        state_(false), upEvent_(false), downEvent_(false),
        reading_(false),
        timeLastToggle_(std::chrono::system_clock::now()),
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
            timeLastToggle_ = std::chrono::system_clock::now();
            reading_ = v;
        }
        else if (reading_ != state_)
        {
            if ((std::chrono::system_clock::now()-timeLastToggle_) > debouncing_time)
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
    std::chrono::system_clock::time_point timeLastToggle_; // time at which reading last changed
    const std::chrono::milliseconds debouncing_time; // time during which reading must be stable
};
    
    
}; // latero


