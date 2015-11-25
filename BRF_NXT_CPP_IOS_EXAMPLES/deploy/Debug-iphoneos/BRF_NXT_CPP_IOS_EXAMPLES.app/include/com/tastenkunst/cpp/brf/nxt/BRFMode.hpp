#ifndef __brf_BRFMode__
#define __brf_BRFMode__

#include <string>

namespace brf {
	
	/**
	 * <p>BRFMode includes the three tracking modes, that BRF can operate in.</p>
	 * <p>Please see the SDK examples. Implementations for all tracking modes are included.</p>
	 * 
	 * @see BRFState
	 * @see BRFManager
	 */
	class BRFMode {
	
		/**
		 * Set mode to BRFMode::POINT_TRACKING if you want to track points in a webcam stream.
		 */
		public: static const std::string POINT_TRACKING;
		
		/**
		 * Set mode to BRFMode::FACE_DETECTION if you want to detect and track face rectangles in images or webcam streams.
		 */
		public: static const std::string FACE_DETECTION;

		/**
		 * Set mode to BRFMode::FACE_TRACKING if you want to detect and track face rectangles and analyse their face features in images or webcam streams.
		 */
		public: static const std::string FACE_TRACKING;
	};

	const std::string BRFMode::POINT_TRACKING("mode_point_tracking");
	const std::string BRFMode::FACE_DETECTION("mode_face_detection");
	const std::string BRFMode::FACE_TRACKING("mode_face_tracking");
}

#endif /* __brf_BRFMode__ */
