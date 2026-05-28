#pragma once

#include "tactiledisplay.h"
#include "point.h"

namespace latero {

/** 
 * Tactile display on a planar carrier.
 */
class Tactograph : public TactileDisplay
{
public:
	Tactograph();
	virtual ~Tactograph();

	/** 
	 * @param position Position (mm) of the center of the TD from the top-left corner of the
	 * virtual surface. The virtual surface consists of the reachable workspace
	 * of the 2D carrier, plus the area over which the tactile dipslay extends at the
	 * edges.
	 * @param orientation Orientation (rad) of the TD (0 when straight)
	 */
	void ReadCarrierState(Point &position, double &orientation);

	/** @returns Width of the virtual surface (mm). */
	inline double GetSurfaceWidth() const { return surfaceWidth_; }

	/** @returns Height of the virtual surface (mm). */
	inline double GetSurfaceHeight() const { return surfaceHeight_; }

	/** @return (width,height) of virtual surface */
	inline Point GetSurfaceSize() const { return surfaceSize_; }

	/** @return Center point of virtual surface */
	inline Point GetSurfaceCenter() const { return surfaceCenter_; }

	/**
	 * @param centerPos position of the center of the display on the virtual display
	 * @param angle orientation of the display
	 * @return A matrix of points indicating the position of the center of each actuator within the virtual surface 
	 * for a given position and orientation of the tactile display.
	 */
	ActuatorImg<Point> GetActuatorPositions(Point centerPos, double angle) const;

    /** @return true if there is no physical device connected to the system */
    inline bool IsEmulated() const { return emulated_; }
    
    /** 
     * set position to be used if emulated 
     * @todo: QUICK HACK: emPos_ and emOrientation_ were declared mutable so that this function could be declared
     * const and used in the GUI thread of LATEROR. This should be safe but it is not quite right 
     * and should be modified in the future. 
     */
    void SetEmulatedState(const Point &pos, double orientation = 0) const;
    
private:

	/** @return true if the kernel module detects the IO card */
	bool CheckDevice();

	/** precompute some constants based on configuration */
	void Precompute();

private:
	/** precomputed width and height of the virtual surface */
	double surfaceWidth_, surfaceHeight_;

	/** workspace of the carrier */
	const double workspaceWidth_, workspaceHeight_;

	/** precomputed size of the virtual surface */
	Point surfaceSize_;

	/** precomputed center of the virtual surface */
	Point surfaceCenter_;

	/** true if there is no device connected to the system */
	bool emulated_;
    
    /** emulated position */
    mutable Point emPos_;
    
    /** emulated orientation */
    mutable double emOrientation_;
};

} // namespace
