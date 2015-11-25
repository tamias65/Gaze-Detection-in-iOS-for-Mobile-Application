#ifndef __brf_Vector3D__
#define __brf_Vector3D__

#include "com/tastenkunst/cpp/brf/nxt/utils/StringUtils.hpp"

namespace brf {

	/**
	 * A 3D point/vector struct, that is not really used outside of BRF internals.
	 * It's a part of the geom package anyway.
	 */
	class Vector3D {
	
		public: double x;
		public: double y;
		public: double z;

		public: Vector3D(double x = 0.0, double y = 0.0, double z = 0.0) :
			x(x), y(y), z(z) {
		}

		public: std::string toString() {
			return "(" + brf::to_string(x) + ", " + brf::to_string(y) + ", " + brf::to_string(z) + ")";
		}
	};
}

#endif /* __brf_Vector3D__ */
