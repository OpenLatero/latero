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

#ifndef LATERORT_TACTILEIMG_H
#define LATERORT_TACTILEIMG_H

#include "assert.h"
#include <vector>

namespace latero {

/**
 * The TactileImg class defines the base class for the specification of
 * tactile images to be displayed on the STReSS. A tactile image is any
 * representation of the placement of the array of skin contactors. Some 
 * possible representations: an array of pin positions, an array of local 
 * skin stretch/compression values, etc. For our purpose, a TactileImage
 * is composed of an array of taxel values.
 */
class TactileImg
{
public:

	/** constructor */
	TactileImg() {};

	/** destructor */
	virtual ~TactileImg() {};

	/** clone object */
	virtual TactileImg* Clone() const = 0;
};

/**
 * The ActuatorImg class restricts the TactileImg to representations
 * in which taxels correspond to actuators. In other words, these types
 * of images specify an array of taxel values, each of which controls
 * the position of an actuator.
 *
 * @todo should there be a shared base class instead of a template?
 */
template<class TTaxel>
class ActuatorImg : public TactileImg
{
public:

	/** 
	 * constructor
	 *
	 * @param sx horizontal size of image
	 * @param sy vertical size of image
	 */
	ActuatorImg(unsigned int sx, unsigned int sy) :
		sx_(sx), sy_(sy)
	{
		img_ = new TTaxel[sx*sy];
	};

	/** 
	 * constructor
	 *
	 * @param sx horizontal size of image
	 * @param sy vertical size of image

	 */
	ActuatorImg(unsigned int sx, unsigned int sy, TTaxel v) :
		sx_(sx), sy_(sy)
	{
		img_ = new TTaxel[sx*sy];
		Set(v);
	};

	/**
	 * Copy constructor
	 */
	ActuatorImg(const ActuatorImg& p) :
		sx_(p.sx_),
		sy_(p.sy_)	
	{
		img_ = new TTaxel[sx_*sy_];
		for (unsigned int i=0; i<sx_*sy_; ++i)
			img_[i] = p.img_[i];
	};

	/** 
	 * destructor 
	 */
	virtual ~ActuatorImg()
	{
		delete[] img_;
	};

	/**
	 * Assignment operator
	 */
	ActuatorImg operator= (const ActuatorImg& s)
	{ 
		if (&s == this) return *this;
		if (Size() != s.Size())
		{
			delete[] img_;
			img_ = new TTaxel[s.sx_*s.sy_];
		}
		sx_ = s.sx_;
		sy_ = s.sy_;
		for (unsigned int i=0; i<sx_*sy_; ++i)
			img_[i] = s.img_[i];
		return *this; 
	};

	TactileImg* Clone() const
	{
		return new ActuatorImg<TTaxel>(*this);
	};

	/**
	 * Set the value of the taxel (x,y) to v.
	 *
	 * @param x position in x
	 * @param y position in y
	 * @param v new taxel value
	 */
	inline void Set(unsigned int x, unsigned int y, TTaxel v)
	{
		Set(GetIndex(x,y), v);
	};

	/**
	 * Set all taxel values to v.
	 *
	 * @param v new taxel value
	 */
	inline void Set(TTaxel v)
	{
		for (unsigned int i=0; i<Size(); ++i)
			Set(i, v);
	};


	/**
	 * Get the value of the taxel (x,y).
	 *
	 * @param x position in x
	 * @param y position in y
	 * @return taxel value
	*/
	inline TTaxel Get(unsigned int x, unsigned int y) const 
	{
		return Get(GetIndex(x,y));
	};


	/**
	 * Set the value of the taxel (i) to v.
	 *
	 * @param i linear position
	 * @param v new taxel value
	 */
	inline void Set(unsigned int i, TTaxel v)
	{
	 	img_[i] = v;
	};

	/**
	 * Get the value of the taxel (i).
	 *
	 * @param i linear position
	 * @return taxel value
	 */
	inline TTaxel Get(unsigned int i) const
	{
		return img_[i];
	};

	inline unsigned int Size() const
	{
		return sx_*sy_;
	};

	inline unsigned int SizeX() const
	{
		return sx_;
	};

	inline unsigned int SizeY() const
	{
		return sy_;
	};

	inline std::vector<TTaxel> Vector() const
	{
		std::vector<TTaxel> v;
		for (unsigned int i=0; i<Size(); ++i)
			v.push_back(Get(i));
		return v;
	}

	inline void Scale(TTaxel v)
	{
		for (unsigned int i=0; i<Size(); ++i)
			Set(i, v*Get(i));
	}

	ActuatorImg<TTaxel> Mult(TTaxel v) const
	{
		ActuatorImg<TTaxel> rv = *this;
		rv.Scale(v);
		return rv;
	}


protected:

	/**
	 * @return linear index corresponding to position
	 * @param x position in x
	 * @param y position in y
	 */
	inline unsigned int GetIndex(unsigned int x, unsigned int y) const
	{
  		assert(x<sx_);
  		assert(y<sy_);
  		return y*sx_ + x;
	};

	/** 
	 * buffer holding the taxel values 
	 */
	TTaxel* img_;

	/**
	 * size of image
	 */
	unsigned int sx_, sy_;

};


class DoubleActuatorImg  : public ActuatorImg<double>
{
public:
	DoubleActuatorImg(unsigned int sx, unsigned int sy) : ActuatorImg<double>(sx,sy) {};

	DoubleActuatorImg(unsigned int sx, unsigned int sy, double v) : ActuatorImg<double>(sx,sy,v) {};

	DoubleActuatorImg* Clone() const
	{
		return new DoubleActuatorImg(*this);
	};

	/** 
	 * destructor 
	 */  
	virtual ~DoubleActuatorImg() {};

	/** TODO: copied from ActuatorImg */
	DoubleActuatorImg Mult(double v) const
	{
		DoubleActuatorImg rv = *this;
		rv.Scale(v);
		return rv;
	}

 	DoubleActuatorImg& operator+=(const DoubleActuatorImg &p)
 	{
		int n=sx_*sy_;
		for (int i=0; i<n; ++i)
			img_[i] += p.img_[i];
		return *this;
	}

 	DoubleActuatorImg& operator-=(const DoubleActuatorImg &p)
 	{
		int n=sx_*sy_;
		for (int i=0; i<n; ++i)
			img_[i] -= p.img_[i];
		return *this;
	}

 	DoubleActuatorImg& operator-=(double v)
 	{
		int n=sx_*sy_;
		for (int i=0; i<n; ++i)
			img_[i] -= v;
		return *this;
	}
};


/**
 * The RangeImg class specifies the the physical position of the actuators relative
 * to the range of values possible with the device. More precisely, the taxels are interpreted 
 * as values between -1.0 and 1.0 representing the entire range of motion of actuators, 
 * from left to right.
 */
class RangeImg  : public DoubleActuatorImg
{
public:
	RangeImg(unsigned int sx, unsigned int sy) : DoubleActuatorImg(sx,sy) {};

	RangeImg(unsigned int sx, unsigned int sy, double v) : DoubleActuatorImg(sx,sy,v) {};

	RangeImg* Clone() const
	{
		return new RangeImg(*this);
	};

	virtual ~RangeImg() {};
};

/**
 * The BiasedImg is similar to RangeImg but the range is from 0 to 1, representing the range of
 * motion from right to left.
 */
class BiasedImg  : public DoubleActuatorImg
{
public:
	BiasedImg(unsigned int sx, unsigned int sy) : DoubleActuatorImg(sx,sy) {};

	BiasedImg(unsigned int sx, unsigned int sy, double v) : DoubleActuatorImg(sx,sy,v) {};

	BiasedImg* Clone() const
	{
		return new BiasedImg(*this);
	};

	virtual ~BiasedImg() {};

	void ConvertToRange(RangeImg &dest) const
	{	
		for (unsigned int i=0; i<sx_*sy_; ++i)
			dest.Set(i, 1.0 - 2.0*img_[i]);
	};

	void ConvertFromRange(const RangeImg &src)
	{	
		for (unsigned int i=0; i<sx_*sy_; ++i)
			img_[i] = 0.5 * (1.0 - src.Get(i));
	};
};


};

#endif
