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

#include "tactograph.h"
#include <math.h>
#include <iostream>

namespace latero {

Tactograph::Tactograph() :
	workspaceWidth_(WORKSPACE_WIDTH),
	workspaceHeight_(WORKSPACE_HEIGHT),
    emPos_(0,0), emOrientation_(0)
{
	Precompute();
	emulated_ = !CheckDevice();
	
	if (handle_)
	{
	  // reset position...
	  latero_pkt_t response;
	  latero_write(handle_, &response);
	  if ((response.hdr.type == PKT_TYPE_FULLR0) || (response.hdr.type == PKT_TYPE_FULLR1))
	  {
	      if (response.fullr.iostatus == 0x0000) 
		 std::cout << "Warning !! The LateroIO status is invalid.\n The Latero I/O interface\nis likely to be unplugged or not powered on.\n";
	      else 
                latero_reset_position(handle_, response.fullr.quad);
	  }
	}
}

Tactograph::~Tactograph()
{
}

bool Tactograph::CheckDevice()
{
  // todo: not sure why this is here and not in TactileDisplay
  return (handle_!=NULL);
}

void Tactograph::ReadCarrierState(Point &position, double &orientation)
{
	if (emulated_)
	{
        position = emPos_;
        orientation = emOrientation_;
	}
	else
	{
		double pos[2]; // position within pantograph workspace
		pos[0] = x_;
		pos[1] = y_;
		orientation = theta_;

		// clip to workspace (todo)
		pos[0] = fmin(fmax(pos[0],0.0),workspaceWidth_);
		pos[1] = fmin(fmax(pos[1],0.0),workspaceHeight_);

		// shift to piezo space...
		pos[0] += GetWidth() / 2.0;
		pos[1] += GetHeight() / 2.0;

		position.Set(pos[0], pos[1]);
	}
}


ActuatorImg<Point> Tactograph::GetActuatorPositions(Point centerPos, double angle) const
{
	// TODO: cleanup
	// note rotation is weird because y is inverted!
	ActuatorImg<Point> map = GetOffset();

	double costheta = cos(angle);
	double sintheta = sin(angle);
	for (uint i=0; i<GetFrameSizeX(); i++)
	{ 
		for (uint j=0; j<GetFrameSizeY(); j++)
		{ 
			Point p = GetActuatorOffset(i,j);
			Point p_rotated;
			p_rotated.x = p.x*costheta + p.y*sintheta;
			p_rotated.y = p.y*costheta - p.x*sintheta;

			Point piezoPos = centerPos + p_rotated;
			map.Set(i,j,piezoPos);
		}
	}
	return map;
}

void Tactograph::Precompute()
{
	surfaceWidth_ = workspaceWidth_ + GetWidth();
	surfaceHeight_ = workspaceHeight_ + GetHeight();
	surfaceSize_ = Point(surfaceWidth_, surfaceHeight_);
	surfaceCenter_ = surfaceSize_/2.0;
}
    
void Tactograph::SetEmulatedState(const Point &pos, double orientation) const
{ 
    if (!emulated_) return;
    emPos_ = pos; 
    emOrientation_ = orientation; 
} 

} // namespace


