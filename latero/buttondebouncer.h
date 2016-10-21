// -----------------------------------------------------------
//
// Copyright (c) 2015 by Vincent Levesque. All Rights Reserved.
//
// This file is part of latero.
//
//    latero is free software: you can redistribute it and/or modify
//    it under the terms of the GNU Lesser General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    latero is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU Lesser General Public License for more details.
//
//    You should have received a copy of the GNU Lesser General Public License
//    along with latero.  If not, see <http://www.gnu.org/licenses/>.
//
// -----------------------------------------------------------

#ifndef LATERO_BUTTON_DEBOUNCER_H
#define LATERO_BUTTON_DEBOUNCER_H

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

#endif

