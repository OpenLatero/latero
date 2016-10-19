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

#include "tactiledisplay.h"
#include <iostream>
#include <stdio.h>

namespace latero {

// There doesn't seem to be any oscillations when looking at the button state every ms. A brief click seems to be in
// the order of 130 ms. We're setting the deboucing to 5 ms just in case there is occasionally oscillations, but
// it might not be necessary. More investigation needed.
const boost::posix_time::time_duration TactileDisplay::debouncing_time = boost::posix_time::milliseconds(5);    
    
TactileDisplay::TactileDisplay() :
    sx_(8), sy_(8),
    pitchX_(1.2), pitchY_(1.6125), // was 1.4 in McGill version
	contactorSizeX_(0.5), contactorSizeY_(1.4), // was 1.2 in McGill version
	offset_(sx_, sy_),
	fadeDuration_(boost::posix_time::milliseconds(500)),       
    displayedImg_(sx_, sy_),
    button0_(debouncing_time), button1_(debouncing_time) 
{
	Precompute();
	fadeStart_ = boost::posix_time::microsec_clock::universal_time();
	
	handle_ = new latero_t;
	int rv = latero_open(handle_, "192.168.1.220"); // TODO: find an elegant way to specify this
	if (rv < 0 )
	{
	    std::cout << "latero_open() failed\n";
	    handle_ = NULL;
	}
	else
	{
		// test connection
		bool failed = true;
		for (int i=0; i<10; ++i)
		{
			latero_pkt_t response;
			latero_write(handle_, &response);
   			if ((response.hdr.type == PKT_TYPE_FULLR0) || (response.hdr.type == PKT_TYPE_FULLR1))
			{
        		if (response.fullr.iostatus != 0x0000)
				{
					failed = false;
					break;
				}
			}
		}

		if (failed)
		{
		    std::cout << "cannot communicate with latero\n";
		    latero_close(handle_);
		    handle_ = NULL;
		}
	}
}

TactileDisplay::~TactileDisplay()
{
  if (handle_) 
  {
    latero_close(handle_);
    delete handle_;
  }
};


int TactileDisplay::WriteFrame(const RangeImg &normFrame)
{
    boost::posix_time::time_duration t = boost::posix_time::microsec_clock::universal_time() - fadeStart_; 
	if (t > fadeDuration_)
	{
		displayedImg_ = normFrame;
		return WriteFrame_(normFrame);
	}
	else
	{
		double ratio = (double)t.total_microseconds() / (double)fadeDuration_.total_microseconds();
		RangeImg img(sx_, sy_);
		for (uint i=0; i<img.Size(); ++i)
			img.Set(i, (1.0-ratio)*displayedImg_.Get(i) + ratio*normFrame.Get(i));
		return WriteFrame_(img);
	}
}


int TactileDisplay::WriteFrame_(const RangeImg &normFrame)
{
	double* mvFrame = new double[normFrame.Size()]; // not very good in realtime
	for (uint i=0; i<normFrame.Size(); ++i)
	{
		// invert so that -1 is to the left and +1 is to the right
		float norm = -1*normFrame.Get(i); // still necessary?
		mvFrame[i] = norm; 
	}
	int rv = WriteFrame_(mvFrame,normFrame.Size());
	delete[] mvFrame;
	return rv;
}

int TactileDisplay::WriteFrame_(double *arr, unsigned int size)
{
    if (!handle_) return 0;
  
    latero_pkt_t response;
    int rv = latero_write_pins(handle_, arr, &response);
    if ((response.hdr.type == PKT_TYPE_FULLR0) || (response.hdr.type == PKT_TYPE_FULLR1))
    {
        if (response.fullr.iostatus == 0x0000) 
        {
            std::cout << "Warning !! The LateroIO status is invalid.\n The Latero I/O interface\nis likely to be unplugged or not powered on.\n";
        } 
        else 
        {
            latero_compute_position(handle_, response.fullr.quad, &x_, &y_, &theta_);
            bool b0 = !(response.fullr.dio_in & LATERO_BUTTON0_MASK);
            bool b1 = !(response.fullr.dio_in & LATERO_BUTTON1_MASK);
            button0_.UpdateState(b0);
            button1_.UpdateState(b1);    
        }
    }
    return rv;
}

void TactileDisplay::SetFadeDuration(boost::posix_time::time_duration duration)
{
	fadeDuration_ = duration;
}

void TactileDisplay::BeginFade()
{
	// todo: handle repeated calls...?
	fadeStart_ = boost::posix_time::microsec_clock::universal_time();
}

void TactileDisplay::Precompute()
{
	width_ = (GetFrameSizeX()-1)*GetPitchX() + GetContactorSizeX();
	height_ = (GetFrameSizeY()-1)*GetPitchY() + GetContactorSizeY();
	radius_ = sqrt(width_*width_ + height_*height_) / 2;
	nbActuators_ = GetFrameSizeX() * GetFrameSizeY();

	// actuator offsets: The offset is used a large number of times within the realtime loop. The actuator
	// offset is therefore precomputed to save time. (This saves in the order of 30 us in some tests.)
	for (unsigned int i=0; i<GetFrameSizeX(); ++i)
	{
		for (unsigned int j=0; j<GetFrameSizeY(); ++j)
		{
			Point p;
			p.x = i*GetPitchX() - ((GetFrameSizeX()-1)*GetPitchX()/2.0);
			p.y = j*GetPitchY() - ((GetFrameSizeY()-1)*GetPitchY()/2.0);
			offset_.Set(i,j,p);
		}
	}	
}


double TactileDisplay::CheckUpdateRate(boost::posix_time::time_duration duration)
{
	if (!handle_) 
		return 0;
  
	long n = 0;
	latero_pkt_t response;	
    boost::posix_time::ptime t0 = boost::posix_time::microsec_clock::universal_time();
	while ((boost::posix_time::microsec_clock::universal_time() - t0) < duration)
	{
		for (int i=0; i<500; ++i)
		{
			latero_write(handle_, &response);
			n++;
		}
	}
	
	double rv = (double)n / (double)(boost::posix_time::microsec_clock::universal_time() - t0).total_seconds();
	std::cout << "Latero Update Rate: " << rv << " Hz\n";
	return rv;
}

void TactileDisplay::MonitorButtons(boost::posix_time::time_duration duration)
{
    if (!handle_) return;
	boost::posix_time::ptime start = boost::posix_time::microsec_clock::universal_time();
    boost::posix_time::time_duration t = boost::posix_time::microsec_clock::universal_time() - start;
    while (t < duration)
    {
        latero_pkt_t response;
        latero_write(handle_, &response);
        t = boost::posix_time::microsec_clock::universal_time() - start;
        if ((response.hdr.type == PKT_TYPE_FULLR0) || (response.hdr.type == PKT_TYPE_FULLR1))
        {
            if (response.fullr.iostatus == 0x0000) 
            {
                std::cout << "Warning !! The LateroIO status is invalid.\n The Latero I/O interface\nis likely to be unplugged or not powered on.\n";
            } 
            else 
            {
                bool b0 = !(response.fullr.dio_in & LATERO_BUTTON0_MASK);
                bool b1 = !(response.fullr.dio_in & LATERO_BUTTON1_MASK);
                std::cout << t.total_milliseconds() << " " << (b0?1:0) << " " << (b1?1:0) << "\n";
            }
        }
    }
}

void TactileDisplay::MonitorButtonsState(boost::posix_time::time_duration duration)
{
    if (!handle_) return;
    
    ButtonDebouncer button0(debouncing_time), button1(debouncing_time);
    boost::posix_time::ptime start = boost::posix_time::microsec_clock::universal_time();
    boost::posix_time::time_duration t = boost::posix_time::microsec_clock::universal_time() - start;
    while (t < duration)
    {
        latero_pkt_t response;
        latero_write(handle_, &response);
        t = boost::posix_time::microsec_clock::universal_time() - start;
        if ((response.hdr.type == PKT_TYPE_FULLR0) || (response.hdr.type == PKT_TYPE_FULLR1))
        {
            if (response.fullr.iostatus == 0x0000) 
            {
                std::cout << "Warning !! The LateroIO status is invalid.\n The Latero I/O interface\nis likely to be unplugged or not powered on.\n";
            } 
            else 
            {
                bool b0 = !(response.fullr.dio_in & LATERO_BUTTON0_MASK);
                bool b1 = !(response.fullr.dio_in & LATERO_BUTTON1_MASK);
                button0.UpdateState(b0);
                button1.UpdateState(b1);
                if (button0.UpEvent()) std::cout << "Button0-UP\n";
                if (button0.DownEvent()) std::cout << "Button0-DOWN\n";
                if (button1.UpEvent()) std::cout << "Button1-UP\n";
                if (button1.DownEvent()) std::cout << "Button1-DOWN\n";
            }
        }
    }
}    
    
}; // latero
