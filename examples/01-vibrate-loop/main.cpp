#include <latero/tactiledisplay.h>
#include <chrono>
#include <cmath>


/**
 * Compute the position of a pin. The index of the pin is specified by x (column) and y (row), starting
 * from the upper-left corner (with the connector on the left of the Latero). The current time is
 * specified by t (in seconds). The function returns a value between -1.0 (left) and +1.0 (right). 
 *
 * This function makes the pins vibrate at 1 Hz, with adjacent rows having an opposite phase.
 */
double ComputePinPosition(unsigned int x, unsigned int y, double t)
{
	// Specify the vibration frequency. Currently 1 Hz.
	const double frequency = 1.0; // in Hertz

	// Compute the vibration as a sine, based on the current time.
	double v = sin(2*M_PI*t*frequency);

	// Invert the output for odd rows.
	if (y%2==1)
		v *= -1;

	return v;
}


int main(int argc, char* argv[])
{
	// Create a latero::TactileDisplay object that will be used to communicate with the Latero.
	// The connection is closed automatically when dev goes out of scope at the end of main().
	latero::TactileDisplay dev;
	
	// Note the start time using std::chrono.
	std::chrono::steady_clock::time_point startTime = std::chrono::steady_clock::now();

	// Note the time elapsed since start time.
	std::chrono::steady_clock::duration currentTime = std::chrono::steady_clock::now() - startTime;

	// Run for one minute.
	while (currentTime < std::chrono::minutes(1))
	{
		// Query the device for the size of a frame (number of pins).
		uint sx = dev.GetFrameSizeX();
		uint sy = dev.GetFrameSizeY();
		
		// Create a tactile image that will specify the position of each pin. The RangeImg
		// uses a value between -1.0 (left) and +1.0 (right) to specify the position of a pin.
		// See tactileimg.h in Latero for other possible encodings.
		latero::RangeImg img(sx, sy);
		
		// For each pin, call a function that will compute the position of the pin based on the
		// pin (x, y) and the current time.
		double currentTimeSeconds = std::chrono::duration<double>(currentTime).count();
		for (uint y=0; y<sy; ++y)
		{
			for (uint x=0; x<sx; ++x)
			{
				double v = ComputePinPosition(x, y, currentTimeSeconds);
				img.Set(x,y,v);
			}
		}
		
		// Send the tactile image to the device.
		dev.WriteFrame(img);

		// Update the current time.
		currentTime = std::chrono::steady_clock::now() - startTime;
	}

	return 0;
};
