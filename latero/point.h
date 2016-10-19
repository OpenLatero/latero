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

#ifndef LATERO1DOF_POINT_H
#define LATERO1DOF_POINT_H

#include <math.h>

namespace latero {

// TODO: this is no longer a pair, more like a vector...

template<class T>
class pair_T
{
public:
	pair_T(T xx=0, T yy=0) :
		x(xx), y(yy)
	{
	}

	pair_T(T p[2]) :
		x(p[0]), y(p[1])
	{
	}

	inline void Clip(T min, T max)
	{
		if (x < min) x = min;
		if (x > max) x = max;
		if (y < min) y = min;
		if (y > max) y = max;

	}

	inline void Set(T xx, T yy)
	{
		x = xx;
		y = yy;
	}

	inline void SetPolar(double norm, double rad)
	{
		x = norm*cos(rad);
		y = norm*sin(rad);
	}

	/** change the norm wihtout changing the orientation */
	inline void SetNorm(double norm)
	{
		SetPolar(norm, AngleRad());
	}

	/** changes the angle without modifying the norm */
	inline void SetAngleRad(double rad)
	{
		SetPolar(Norm(), rad);
	}

	inline void SetAngleDegree(double deg)
	{
		SetAngleRad(deg*M_PI/180.0);
	}

	/** @return angle in radians (0-2Pi) */
	inline double AngleRad() const
	{
		double angle = atan2(y,x);
		if (angle<0) angle += 2*M_PI;
		return angle;
	}

	/** @return angle in degrees (0-360) */
	inline double AngleDegree() const
	{
		return AngleRad() * 180.0 / M_PI;
	}

	inline double NormSquared() const
	{
		return x*x + y*y;
	}

	inline double Norm() const
	{
		return sqrt(NormSquared());
	}

	pair_T<T> operator*(int v) const
	{
		pair_T<T> rv = *this;
		rv.x *= v;
		rv.y *= v;
		return rv;
	}

	pair_T<T> operator*(double v) const
	{
		pair_T<T> rv = *this;
		rv.x *= v;
		rv.y *= v;
		return rv;
	}

 	pair_T<T>& operator*=(double v)
 	{
		*this = *this * v;
		return *this;
	}

 	pair_T<T>& operator*=(int v)
 	{
		*this = *this * v;
		return *this;
	}

 	pair_T<T>& operator/=(int v)
 	{
		*this = *this / v;
		return *this;
	}

 	pair_T<T>& operator/=(double v)
 	{
		*this = *this / v;
		return *this;
	}

 	pair_T<T>& operator+=(const pair_T<T> &p)
 	{
		*this = *this + p;
		return *this;
	}

 	pair_T<T>& operator-=(const pair_T<T> &p)
 	{
		*this = *this - p;
		return *this;
	}

	pair_T<T> operator/(double v) const
	{
		pair_T<T> rv = *this;
		rv.x /= v;
		rv.y /= v;
		return rv;
	}

	pair_T<T> operator/(int v) const
	{
		pair_T<T> rv = *this;
		rv.x /= v;
		rv.y /= v;
		return rv;
	}


	friend pair_T<T> operator-(const pair_T<T>& p1, const pair_T<T>& p2)
	{
   		pair_T<T> rv;
   		rv.x = p1.x - p2.x;
   		rv.y = p1.y - p2.y;
   		return rv;
	}

	friend pair_T<T> operator+(const pair_T<T>& p1, const pair_T<T>& p2)
	{
   		pair_T<T> rv;
   		rv.x = p1.x + p2.x;
   		rv.y = p1.y + p2.y;
   		return rv;
	}

	friend bool operator==(const pair_T<T>& p1, const pair_T<T>& p2)
	{
		return ((p1.x == p2.x) && (p1.y == p2.y));
	}

	friend bool operator!=(const pair_T<T>& p1, const pair_T<T>& p2)
	{
		return ((p1.x != p2.x) || (p1.y != p2.y));
	}


	T x;
	T y;
};

typedef pair_T<double> Point;

} // namespace

#endif
