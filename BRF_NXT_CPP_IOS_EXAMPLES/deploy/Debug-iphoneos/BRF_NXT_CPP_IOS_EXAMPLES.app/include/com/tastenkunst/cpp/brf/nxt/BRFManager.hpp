#ifndef __brf_BRFManager__
#define __brf_BRFManager__

#define API __attribute__((used))

#include <cstdlib>
#include <cstdint>
#include <vector>
#include <memory>
#include <string>
#include <functional>

#include "com/tastenkunst/cpp/brf/nxt/utils/AllocUtils.hpp"

#include "com/tastenkunst/cpp/brf/nxt/image/InputData.hpp"

#include "com/tastenkunst/cpp/brf/nxt/geom/Point.hpp"
#include "com/tastenkunst/cpp/brf/nxt/geom/Rectangle.hpp"

#include "com/tastenkunst/cpp/brf/nxt/BRFFaceShape.hpp"

/**
 * Beyond Reality Face is a general face tracker.
 * It detects a single face in a webcam stream and follows it
 * by morphing a 68 facial point shape.
 *
 * @author Marcel Klammer, <a href="http://www.tastenkunst.com" target="_blank">Tastenkunst GmbH</a>, 2015
 */
namespace brf {

	/**
	 * InputData class for BRF. It will process your color pixel pointer
	 * and convert it to a grayscale pixel pointer. Or just copy your
	 * grayscale pixel pointer, depending on the doGrayScale param.
	 */
	class BitmapData: public brf::InputData {

		public: uint8_t* _grayData;
		public:	int _size;
		public: bool _doGrayScale;

		/**
		 * @param width - mandatory, the width of the image data
		 * @param height - mandatory, the height of the image data
		 * @param doGrayScale - optional (default: true), do the grayscaling yourself (false), or let BRF do that for you (true).
		 */
		public: BitmapData(int width, int height, bool doGrayScale = true) : InputData(width, height),
				_grayData((uint8_t*)brf::fastMalloc(width * height * sizeof(uint8_t))),
				_size(width * height),
				_doGrayScale(doGrayScale) {
		}
		
		/**
		 * <p>Updates the grayscale pixel array. Input your color pixel array or grayscale pixel
		 * array and the data will be processed or copied into the _grayData pointer of this BitmapData.
		 *
		 * @param data - mandatory, the pixel array/buffer/pointer, gray (doGrayScale == false) or color/bgra (doGrayScale == true)
		 */
		public: void updateData(uint8_t* data);
		
		/**
		 * Returns its internal grayscale pixel pointer for usage in BRF.
		 *
		 * @return the grayscale 8bit pointer
		 */
		public: virtual uint8_t* getData() {
			return _grayData;
		}
	}; // class BitmapData
	
	/**
	 * <p>BRFManager handles the startup of the SDK and all tracking processes. You just need to instantiate it with 
	 * a BitmapData instance you want to analyse and the region of interest (roi, Rectangle region in that BitmapData).</p>
	 * 
	 * <p>Don't call ANY tracking related method before READY was dispatched.</p>
	 * 
	 * <p>BRFManager::READY will be dispatched, when BRF is ready to use. You can set up BRF for your needs with
	 * the methods setPointTrackingVars, setFaceDetectionVars and setFaceTrackingVars. Calling these methods is
	 * completely optional, since all BRF components use default values, that do a great job.</p>
	 * 
	 * <p>There are three different modes that BRF can operate in:</p>
	 * <p>BRFMode::POINT_TRACKING - Tracks points in a video/webcam stream.</p>
	 * <p>BRFMode::FACE_DETECTION - Finds faces in images/webcam streams and marks them with rectangles.</p>
	 * <p>BRFMode::FACE_TRACKING  - Detects a single face and analyses it to find detailed face features.</p>
	 * 
	 * <p>Please see the SDK's examples. Implementations for all tracking modes are included.</p>
	 * 
	 * <p>Author: Marcel Klammer, <a href="http://www.tastenkunst.com" target="_blank">Tastenkunst GmbH</a>, 2015</p>
	 */
	class BRFManager {

		/**
		 * Once BRF is ready to start the tracking, READY is dispatched (called).
		 */
		public: static const std::function< void() > READY;
		
		public: brf::Rectangle* _roi;
		public: brf::InputData* _src;
		public: bool _debug;
		
		public: BRFManager() :
			_roi(NULL),
			_src(NULL),
			_debug(false) {
		}

		public: ~BRFManager() {
			_roi = NULL;
			_src = NULL;
		}
		
		/**
		 * <p>Initializes the SDK and sets up BRF automatically.
		 * Once BRF is ready to start tracking, BRFManager::READY is dispatched.
		 * Just listen to that event.</p>
		 * 
		 * <p>For questions about the implementation, please refer to the examples, that are included in the SDK download.</p>
		 * 
		 * @param src - mandatory, the BitmapData BRF is working on (default size should be 640x480)
		 * @param roi - mandatory, define the region of interest in your src BitmapData, that should be analysed (should definitly NOT exceed 800x600, default should be 640x480)
		 */
		public: void init(brf::InputData* src, brf::Rectangle* roi);
		
		/**
		 * Sets whether to trace out debug information.
		 *
		 * @param debug - mandatory, true for showing traces, false otherwise
		 */
		public: void debug(bool debug);
		
		/**
		 * traces BRF notes, if debug is set to true.
		 *
		 * @param msg - mandatory, the msg to trace
		 */
		public: void log(std::string msg);
		
		/**
		 * <p>Updates the tracking process of BRF. Usually on every camera frame.</p>
		 *
		 * @return current BRFState (same as BRFManager.state)
		 */
		public: std::string& update();
		
		/**
		 * Not implemented yet.
		 *
		 * <p>(since BRF v3.0.10, reintroduced: was part of BRF v2.3 before)</p>
		 * 
		 * <p>Single image face tracking was not working as expected. There was a 50% chance to get a good result,
		 * but it was more like a lucky shot.</p>
		 * 
		 * <p>This method lets you specify the left and right eye position. It skips the face detection part and
		 * uses the eye positions to align the face tracking shape. This way there is a much better chance to get
		 * a good result out of a single image. (Works only with mode == BRFMode::FACE_TRACKING.)</p>
		 *
		 * @param leftEye - mandatory, the left eye position (on the BitmapData, that BRF will work on!).
		 * @param rightEye - mandatory, the right eye position (on the BitmapData, that BRF will work on!).
		 * @return numUpdates - optional, number of morthing tries of the shape.
		 */
		public: std::string& updateByEyes(brf::Point& leftEye, brf::Point& rightEye, int numUpdates = 25);
		
		/** 
		 * <p>Resets the tracking process of BRF. Usually when you want to pause BRF tracking or restart with FACE_DETECTION.
		 * Also removes all pointsToTrack, when mode == BRFMode::POINT_TRACKING.</p>
		 */
		public: void reset();
		
		/**
		 * Returns the used BRFMode (BRFMode::POINT_TRACKING, BRFMode::FACE_DETECTION, BRFMode::FACE_TRACKING)
		 *
		 * @return current BRFMode
		 * @see BRFMode
		 */
		public: std::string& mode();
		
		/**
		 * Sets the BRFMode (BRFMode::POINT_TRACKING, BRFMode::FACE_DETECTION, BRFMode::FACE_TRACKING)
		 *
		 * @param mode - mandatory
		 * @see BRFMode
		 */
		public: void mode(std::string mode);
		
		/** 
		 * <p>lastState is set to the BRFState, that was set in the end of the last update call.</p>
		 * 
		 * @return lastState
		 * @see BRFState
		 */
		public: std::string& lastState();
		
		/** 
		 * <p>state is set to the BRFState, that was set in the end of the current update call.</p>
		 * 
		 * @return state
		 * @see BRFState
		 */
		public: std::string& state();
		
		/** 
		 * <p>nextState is set to the BRFState, that will be the state of the next update cycle.</p>
		 * 
		 * @return nextState
		 * @see BRFState
		 */
		public: std::string& nextState();
		
		///////////////
		// Optical Flow
		///////////////
		
		/**
		 * <p>Sets all necessary parameters for mode == BRFMode::POINT_TRACKING.</p>
		 * 
		 * @param patchSize - optional, the size of a point patch. Can be any odd value between 11 and 41. A larger patchSize means better results but lower performance.
		 * @param numLevels - optional, the number of pyramid levels optical flow will work on. Can be any value between 1 and 4. More levels mean better results but lower performance.
		 * @param numIterations - optional, the number of iterations optical flow will do before it stops improving results. Can be any value between 5 and 100. More iterations mean better results but lower performance.
		 * @param error - optional, an accuracy value. Usually a value between 0.0001 and 0.1. Smaller values mean better results but lower performance.
		 */
		public: void setPointTrackingVars(int patchSize = 21, int numLevels = 4, int numIterations = 25, double error = 0.0006);
		
		/**
		 * <p>Updated when: mode == BRFMode::POINT_TRACKING &amp;&amp; state == BRFState::POINT_TRACKING.
		 * Includes all tracked points, whether valid or invalid (get their states from pointStates)</p>
		 * 
		 * @return pointsToTrack - a vector of points
		 */
		public: std::vector< std::shared_ptr<brf::Point> >& pointsToTrack();
		
		/** 
		 * <p>Updated when: mode == BRFMode::POINT_TRACKING &amp;&amp; state == BRFState::POINT_TRACKING.
		 * Includes the states for all corresponding pointsToTrack.</p>
		 * 
		 * <p>If true: point is valid. If false: point is not valid (and will be removed from pointsToTrack automatically 
		 * if checkPointsValidBeforeTracking == true)</p>
		 * 
		 * @return pointStates -  a vector of states
		 */
		public: std::vector< bool >& pointStates();
		
		/**
		 * Returns whether BRF should check for invalid/lost points before an update. If true, invalid points will be removed from pointsToTrack.
		 * 
		 * @return whether BRF checks for invalid points before an update
		 */
		public: bool checkPointsValidBeforeTracking();

		/**
		 * Sets whether BRF should check for invalid/lost points before an update. If true, invalid points will be removed from pointsToTrack.
		 *
		 * @param checkPointsValidBeforeTracking - mandatory
		 */
		public: void checkPointsValidBeforeTracking(bool checkPointsValidBeforeTracking);
		
		/**
		 * Adds points that should be tracked. Please call this method right before _brfManager.update();
		 *
		 * @param pointsToAdd - mandatory, points to add
		 */
		public: void addPointsToTrack(std::vector< std::shared_ptr<brf::Point> >& pointsToAdd);
		
		///////////////
		// ViolaJones
		///////////////
		
		/**
		 * <p>Sets all necessary parameters for mode == BRFMode::FACE_DETECTION.</p>
		 * 
		 * @param baseScale - optional, start scale value. Must be smaller than maxScale. Smaller values mean scan for smaller faces, but much(!) lower performance.
		 * @param scaleIncrement - optional, increment scale value. Lower values mean more layers of face sizes, but much lower performance.
		 * @param maxScale - optional, end scale value. Higher values mean larger faces, but (not so much) lower performance.
		 * @param rectIncrement - optional, accuracy value for one face size layer. Lower values mean more scans in one face size layer, but much(!) lower performance.
		 * @param minRectsToFind - optional, the number of faces BRF needs to find to stop the scan. If minRectsToFind faces are within one region of the image, lastDetectedFace will hold the resulting face rectangle.
		 * @param detectMultipleFaces - optional, if true, BRF will not stop scanning for faces after one face was found. Draw lastDetectedFaces (with s) to view all found faces.
		 */
		public: void setFaceDetectionVars(double baseScale = 4.0,
				double scaleIncrement = 0.5, double maxScale = 12.0, double rectIncrement = 0.04,
				int minRectsToFind = 12, bool detectMultipleFaces = false);
		
		/**
		 * <p>Sets the region of interest (roi) for BRFState::FACE_DETECTION.
		 * In most cases you can set a smaller roi for face detection in the center of the source image. 
		 * This will result in better performance and a smoother tracking start, because the tracking start in the image center.</p>
		 *
		 * @param x - mandatory
 		 * @param y - mandatory
 		 * @param width - mandatory
 		 * @param height - mandatory
		 */
		public: void setFaceDetectionROI(double x, double y, double width, double height);
		
		/** 
		 * <p>Updated when: (mode == BRFMode::FACE_DETECTION || mode == BRFMode::FACE_TRACKING) &amp;&amp; state == BRFState::FACE_DETECTION.
		 * If no result is avalable, x, y, width and height are set to 0. Otherwise it's filled with the found face region.</p>
		 * 
		 * @return lastDetectedFace
		 */
		public: brf::Rectangle& lastDetectedFace();
		
		/** 
		 * <p>Updated when: (mode == BRFMode::FACE_DETECTION || mode == BRFMode::FACE_TRACKING) &amp;&amp; state == BRFState::FACE_DETECTION.
		 * Includes every single found face region of that update cycle.</p>
		 * 
		 * @return lastDetectedFaces - vecto of rectangels
		 */
		public: std::vector< std::shared_ptr<brf::Rectangle> >& lastDetectedFaces();
		
		///////////////
		// ASM
		///////////////
		
		/**
		 * <p>Sets all necessary parameters for mode == BRFMode::FACE_TRACKING.</p>
		 * 
		 * @param minShapeDimension - optional, BRF will reset if the faceShape is smaller than that value.
		 * @param maxShapeDimension - optional, BRF will reset if the faceShape is larger than that value.
		 * @param resetMode - optional, 1 is a more strict reset behaviour, 0 is the old not that strict one.
		 */
		public: void setFaceTrackingVars(double minShapeDimension = 50.0,
				double maxShapeDimension = 500.0, int resetMode = 1);
		
		/** 
		 * <p>Updated when: mode == BRFMode::FACE_TRACKING &amp;&amp; (state == BRFState::FACE_TRACKING_START || state == BRFState::FACE_TRACKING).
		 * Includes 68 points/vertices of the face shape, also its triangles and its bounds.</p>
		 * 
		 * <p>FaceShape_points.jpg shows all indices of the points.</p>
		 * 
		 * <p>If candideEnabled == true it will also update candideVertices, scale, translationX, translationY, rotationX, rotationY and rotationZ. 
		 * These values are used to update 3D engines. This Candide 3 implementation also includes 6 ActionUnits (activate them by setting candideActionUnitsEnabled == true).</p>
		 *
		 * @return faceShape
		 */
		public: brf::BRFFaceShape& faceShape();
		
		/**
		 * Returns candideEnabled. Only enable the Candide 3 model update, when you need it (eg. for 3D engine updates).
		 *
		 * @return candideEnabled
		 */
		public: bool candideEnabled();
		
		/**
		 * Sets candideEnabled. Only enable the Candide 3 model update, when you need it (eg. for 3D engine updates).
		 *
		 * @param candideEnabled - mandatory
		 */
		public: void candideEnabled(bool candideEnabled);

		/**
		 * Returns candideActionUnitsEnabled. Only enable the Candide 3 model update and ActionUnit update, when you need it (eg. for 3D engine updates).
		 *
		 * @return candideActionUnitsEnabled
		 */
		public: bool candideActionUnitsEnabled();
		
		/**
		 * Sets candideActionUnitsEnabled. Only enable the Candide 3 model update and ActionUnit update, when you need it (eg. for 3D engine updates).
		 *
		 * @param candideActionUnitsEnabled - mandatory
		 */
		public: void candideActionUnitsEnabled(bool candideActionUnitsEnabled);
		
	}; // class BRFManager
	
} // namespace brf

#endif // #define __brf_BRFManager__
