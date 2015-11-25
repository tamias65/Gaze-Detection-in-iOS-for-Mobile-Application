#ifndef __brf_Matrix__
#define __brf_Matrix__

#include <cmath>

#include "com/tastenkunst/cpp/brf/nxt/utils/StringUtils.hpp"

namespace brf {
	
	/**
	 * A transform matrix struct, that is not really used outside of BRF internals.
	 * It's a part of the geom package anyway.
	 */
	class Matrix {

		public: double a;
		public: double b;
		public: double c;
		public: double d;
		public: double tx;
		public: double ty;

		public: Matrix(double a = 1.0, double b = 0.0, double c = 0.0, double d = 1.0, double tx = 0.0, double ty = 0.0) :
			a(a), b(b), c(c), d(d), tx(tx), ty(ty) {
		}

		public: void translate(double _tx, double _ty) {
			tx += _tx;
			ty += _ty;
		}

		public: void scale(double _sx, double _sy) {
			a *= _sx;
			b *= _sy;
			c *= _sx;
			d *= _sy;
			tx *= _sx;
			ty *= _sy;
		}

		public: void rotate(double _r) {
			double _sin = sin(_r);
			double _cos = cos(_r);
			double _a = a;
			double _b = b;
			double _c = c;
			double _d = d;
			double _tx = tx;
			double _ty = ty;

			a  = _a  * _cos - _b  * _sin;
			b  = _a  * _sin + _b  * _cos;
			c  = _c  * _cos - _d  * _sin;
			d  = _c  * _sin + _d  * _cos;
			tx = _tx * _cos - _ty * _sin;
			ty = _tx * _sin + _ty * _cos;
		}

		public: void identity() {
			a = 1.0;
			b = 0.0;
			c = 0.0;
			d = 1.0;
			tx = 0.0;
			ty = 0.0;
		}

		public: std::string toString() {
			return "(" +
					brf::to_string(a) + ", " + brf::to_string(b) + ", " +
					brf::to_string(c) + ", " + brf::to_string(d) + ", " +
					brf::to_string(tx) + ", " + brf::to_string(ty) + ")";
		}
	};
}

#endif /* __brf_Matrix__ */
