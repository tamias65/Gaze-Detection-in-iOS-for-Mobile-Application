#ifndef __brf_BRFState__
#define __brf_BRFState__

#include <string>

namespace brf {

	/**
	 * <p>BRF can be in different BRFStates depending on BRFMode and tracking results. These are the combinations:</p>
	 *
	 * <p>BRFMode::POINT_TRACKING - possible states: BRFState::POINT_TRACKING or BRFState::RESET (RESET only after a reset() call).</p>
	 * <p>BRFMode::FACE_DETECTION - possible states: BRFState::FACE_DETECTION or BRFState::RESET (RESET only after a reset() call).</p>
	 * <p>BRFMode::FACE_TRACKING - possible states: BRFState::FACE_DETECTION, BRFState::FACE_TRACKING_START, BRFState::FACE_TRACKING or BRFState::RESET (RESET only after a reset() call).</p>
	 * 
	 * <p>In BRFState::RESET nothing is updated.</p>
	 * <p>In BRFState::POINT_TRACKING BRFManager.pointsToTrack and BRFManager.pointStates are updated.</p>
	 * <p>In BRFState::FACE_DETECTION BRFManager.lastDetectedFace and BRFManager.lastDetectedFaces are updated.</p>
	 * <p>In BRFState::FACE_TRACKING_START BRFManager.faceShape is updated (but not its candide values).</p>
	 * <p>In BRFState::FACE_TRACKING BRFManager.faceShape is updated (including its candide values).</p>
	 * 
	 * @see BRFMode
	 * @see BRFManager
	 */
	class BRFState {
		
		/**
		 * <p>When mode == BRFMode::FACE_DETECTION || mode == BRFMode::FACE_TRACKING</p>
		 * <p>In BRFState::FACE_DETECTION BRFManager.lastDetectedFace and BRFManager.lastDetectedFaces are updated.</p>
		 */
        public: static const std::string FACE_DETECTION;
		
		/**
		 * <p>When mode == BRFMode::FACE_TRACKING</p>
		 * <p>In BRFState::FACE_TRACKING_START BRFManager.faceShape is updated (but not its candide values).</p>
		 */
		public: static const std::string FACE_TRACKING_START;
		
		/**
		 * <p>When mode == BRFMode::FACE_TRACKING</p>
		 * <p>In BRFState::FACE_TRACKING BRFManager.faceShape is updated (including its candide values).</p>
		 */
		public: static const std::string FACE_TRACKING;
		
		/**
		 * <p>When mode == BRFMode::POINT_TRACKING</p>
		 * <p>In BRFState::POINT_TRACKING BRFManager.pointsToTrack and BRFManager.pointStates are updated.</p>
		 */
		public: static const std::string POINT_TRACKING;
		
		/**
		 * <p>When mode == BRFMode::POINT_TRACKING || mode == BRFMode::FACE_DETECTION || mode == BRFMode::FACE_TRACKING</p>
		 * <p>In BRFState::RESET nothing is updated. This state will be set only after a BRFManager.reset() call.</p>
		 */
		public: static const std::string RESET;
	};

	const std::string BRFState::FACE_DETECTION("state_face_detection");
	const std::string BRFState::FACE_TRACKING_START("state_face_tracking_start");
	const std::string BRFState::FACE_TRACKING("state_face_tracking");
	const std::string BRFState::POINT_TRACKING("state_point_tracking");
	const std::string BRFState::RESET("state_reset");
}

#endif /* __brf_BRFState__ */
