#pragma once

#include "tactileimg.h"
#include "point.h"
#include <boost/date_time/posix_time/posix_time_types.hpp>
#include "tl-latero/latero.h"
#include "buttondebouncer.h"
#include <stdint.h>
#include <chrono>


namespace latero {

class TactileDisplay
{
public:
	TactileDisplay();
	virtual ~TactileDisplay();
	int WriteFrame(const RangeImg &normFrame);
	void SetFadeDuration(int ms);
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
	double CheckUpdateRate(int seconds = 60);

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

	std::chrono::system_clock::time_point fadeStart_;
	std::chrono::milliseconds fadeDuration_;
	RangeImg displayedImg_; // unless fading...
    ButtonDebouncer button0_, button1_;
};


}; // latero
