#ifndef __brf__ExampleBase__
#define __brf__ExampleBase__

#include <cstdlib>
#include <cstdint>

#include "com/tastenkunst/cpp/brf/nxt/BRFManager.hpp"
#include "com/tastenkunst/cpp/brf/nxt/geom/Rectangle.hpp"

/**
 * ExampleBase initializes and updates Beyond Reality Face.
 * Just input the camera image with and height and whether BRF should
 * take care of gray scaling the image of if you want to do it on your
 * own, which most likely will be slower ;).
 */
class ExampleBase {

	public: brf::BitmapData	_bmd;
	public: brf::Rectangle	_brfRoi;
	public: brf::BRFManager	_brfManager;
	
	public: ExampleBase(int width, int height, bool grayScale) :
			_bmd(width, height, grayScale),
			_brfRoi(0, 0, width, height),
			_brfManager() {
	}
	
	public: ~ExampleBase() {
	}
	
	public: void init(bool showTraces = false) {
		
		// pass in true to see some traces (optional)
		// set it to false again in onReadyBRF to avoid the state change traces
		
		_brfManager.debug(showTraces);
		
		// Init BRF with InputData and a region of interest
		
		_brfManager.init(&_bmd, &_brfRoi);
	}
	
	public: virtual void onReadyBRF() {
	}
	
	public: virtual void update(uint8_t* data) {
		_bmd.updateData(data);
		_brfManager.update();
	}
};

#endif /* __brf__ExampleBase__ */