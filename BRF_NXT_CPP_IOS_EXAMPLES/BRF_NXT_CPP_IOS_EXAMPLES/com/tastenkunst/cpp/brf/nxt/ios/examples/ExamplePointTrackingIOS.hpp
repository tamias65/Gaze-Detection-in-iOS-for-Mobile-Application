#ifndef __brf__ios__ExamplePointTrackingIOS__
#define __brf__ios__ExamplePointTrackingIOS__

#include <cstdlib>
#include <cstdint>

#include "com/tastenkunst/cpp/brf/nxt/BRFMode.hpp"
#include "com/tastenkunst/cpp/brf/nxt/BRFState.hpp"

#include "com/tastenkunst/cpp/brf/nxt/geom/Rectangle.hpp"

#include "com/tastenkunst/cpp/brf/nxt/ios/utils/DrawingUtils.hpp"

#include "ExampleBase.hpp"

/**
 * Point tracking class. Touch on your device to add some points.
 * (make sure to add the touch event handler in the ViewController.mm)
 */
class ExamplePointTrackingIOS : public ExampleBase {

	public: std::vector< std::shared_ptr<brf::Point> > _pointsToAdd;
	public: int _lastNumPoints;
	
	public: ExamplePointTrackingIOS(int width, int height) :
			ExampleBase(width, height, true),
			
			_pointsToAdd(0),
			_lastNumPoints(0) {
	}
	
	public: ~ExamplePointTrackingIOS() {
	}
	
	public: void onReadyBRF() {
		brf::trace("ExamplePointTrackingIOS::onReadyBRF");
		
		_brfManager.setPointTrackingVars(21, 4, 25, 0.0006);
		_brfManager.checkPointsValidBeforeTracking(true);

		_brfManager.mode(brf::BRFMode::POINT_TRACKING);
		
		// and maybe turn off debug traces
		
		_brfManager.debug(false);
	}
	
	/**
	 * We overwrite the original update method to be able
	 * to add the points befor the next tracking.
	 */
	public: virtual void update(uint8_t* data) {
		_bmd.updateData(data);
		
		if(_pointsToAdd.size() > 0) {
		
			// Update point right before a tracking update to make sure, that
			// it is not done within a tracking update cycle.
		
			_brfManager.addPointsToTrack(_pointsToAdd);
			_pointsToAdd.clear();
		}
		
		_brfManager.update();
	}
	
	/**
	 * We update the sampled buffer with the BRF rois and results.
	 */
	public: void updateGUI(CGContextRef context) {
		
		std::string& state = _brfManager.state();
	
		// this time: point tracking
		if(state == brf::BRFState::POINT_TRACKING) {
			
			// Get all the points, that were tracked.
			
			std::vector< std::shared_ptr<brf::Point> > points = _brfManager.pointsToTrack();
			
			// Get all their states, either valid or invalid.
			
			std::vector< bool > pointStates = _brfManager.pointStates();

			int i = 0;
			int l = (int)points.size();

			while(i < l) {
			
				// Draw all points, invalid will be red (and will be removed instantly),
				// valid points are green.
			
				if(pointStates[i]) {
					DrawingUtils::drawPoint(context, points[i], 2.0f, true, 0x00ff00, 1);
				} else {
					DrawingUtils::drawPoint(context, points[i], 2.0f, true, 0xff0000, 1);
				}
				
				++i;
			}
			
			// Trace out, if a points got added or lost.
			
			if(l != _lastNumPoints) {
				_lastNumPoints = l;
				
				brf::trace("Number of points: " + brf::to_string(l));
			}
		}
	}
	
	/**
	 * Adding a touch location on the image data.
	 * Choose to either add one or many points.
	 */
	public: void onClickedVideo(int x, int y) { //MouseEvent
		// Add 1 point:
//		_pointsToAdd.push_back(std::shared_ptr<brf::Point>(new brf::Point((double)x, (double)y)));

		//Add 100 points
		double w = 120.0;
		double step = 10.0;
		double xStart = x - w * 0.5;//event.localX - w * 0.5;
		double xEnd = x + w * 0.5; //event.localX + w * 0.5;
		double yStart = y - w * 0.5; //event.localY - w * 0.5;
		double yEnd = y + w * 0.5;//event.localY + w * 0.5;
		double dy = yStart;
		double dx = xStart;

		for(; dy < yEnd; dy += step) {
			for(dx = xStart; dx < xEnd; dx += step) {
				_pointsToAdd.push_back(std::shared_ptr<brf::Point>(new brf::Point(dx, dy)));
			}
		}
	}
};

#endif /* __brf__ios__ExamplePointTrackingIOS__ */