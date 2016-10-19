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

#ifndef LATERO_TACTILE_DISPLAY_H
#define LATERO_TACTILE_DISPLAY_H

#include "tactileimg.h"
#include "point.h"
#include <boost/date_time/posix_time/posix_time_types.hpp>
#include "tl-latero/latero.h"
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
    
    
class TactileDisplay
{
public:
	TactileDisplay();
	virtual ~TactileDisplay();
	int WriteFrame(const RangeImg &normFrame);
	void SetFadeDuration(boost::posix_time::time_duration duration);
	void BeginFade();

	/** @return total number of actuators */
	inline uint GetNbActuators() const { return nbActuators_; }

	/** @return number of actuators in X */
	inline uint GetFrameSizeX() const { return sx_; }

	/** @return number of actuators in Y */
	inline uint GetFrameSizeY() const { return sy_; }

	/** @return distance between actuator centers in X (mm) */
	inline double GetPitchX() const { return pitchX_; }

	/** @return distance between actuator centers in Y (mm) */
	inline double GetPitchY() const { return pitchY_; }

	/** @return width of contactors */
	inline double GetContactorSizeX() const { return contactorSizeX_; }

	/** @return height of contactors */
	inline double GetContactorSizeY() const { return contactorSizeY_; }

	/** @return width of the tactile display, including the span of the actuators */
	inline double GetWidth() const { return width_; }

	/** @return height of the tactile display, including the span of the actuators */
	inline double GetHeight() const { return height_; }

	/** @return radius of bounding circle that encloses the tactile display, including the span of the actuators */
	inline double GetRadius() const { return radius_; }

	/** @return Offset of actuator (i,j) with respect to the center of the display */
	inline Point GetActuatorOffset(uint i, uint j) const { return offset_.Get(i,j); }

	/** @return Offset of all actuators with respect to the center of the display */
	ActuatorImg<Point> GetOffset() const { return offset_; }

	/** compute update rate over a certain period of time */
	double CheckUpdateRate(boost::posix_time::time_duration duration);

    inline bool GetButton0(bool &upEvent, bool &downEvent) const {
        upEvent = button0_.UpEvent();
        downEvent = button0_.DownEvent();
        return button0_.IsDown();
    }

    inline bool GetButton1(bool &upEvent, bool &downEvent) const {
        upEvent = button1_.UpEvent();
        downEvent = button1_.DownEvent();
        return button1_.IsDown();
    }    
    
    void MonitorButtons(boost::posix_time::time_duration duration);
    void MonitorButtonsState(boost::posix_time::time_duration duration);
    
protected:
	void Precompute();
	int WriteFrame_(const RangeImg &normFrame);
	int WriteFrame_(double *arr, unsigned int size);

	latero_t *handle_;
	double x_, y_, theta_;
	
private:
    
	// config
	const unsigned int sx_, sy_; // frame size
	const float pitchX_, pitchY_; // in mm
	const float contactorSizeX_, contactorSizeY_; // in mm
    static const boost::posix_time::time_duration debouncing_time;

	// precomputed information
	double width_;
	double height_;
	double radius_;
	ActuatorImg<Point> offset_;
	int nbActuators_;

	boost::posix_time::ptime fadeStart_;
	boost::posix_time::time_duration fadeDuration_; // ms
	RangeImg displayedImg_; // unless fading...
    ButtonDebouncer button0_, button1_;
};


}; // latero

#endif
