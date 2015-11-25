#ifndef __brf_Point__
#define __brf_Point__

#include <cmath>

#include "com/tastenkunst/cpp/brf/nxt/utils/StringUtils.hpp"

namespace brf {

	/**
	 * Defines a point with x, y.
	 */
	class Point {
		
		/**
		 * the x of the point.
		 */
		public: double x;
		
		/**
		 * the y of the point.
		 */
		public: double y;
		
		/**
		 * Instantiates a brf::Point.
		 *
		 * @param x - optional, the x of the point.
		 * @param y - optional, the y of the point.
		 */
		public: Point(double x = 0.0, double y = 0.0) :
			x(x), y(y) {
		}
		
		/**
		 * @param x0 the x value of the first point.
		 * @param y0 the y value of the first point.
		 * @param x1 the x value of the second point.
		 * @param y1 the y value of the second point.
		 * @return the distance between to points.
		 */
		public: static double distance(double x0, double y0, double x1, double y1) {
			return sqrt((x1 - x0) * (x1 - x0) + (y1 - y0) * (y1 - y0));
		}
		
		/**
		 * Interpolates a point position from 2 other points and a factor.
		 * A factor of 0.5 with be half way between the two points.
		 *
		 * @param x0 the x value of the first point.
		 * @param y0 the y value of the first point.
		 * @param x1 the x value of the second point.
		 * @param y1 the y value of the second point.
		 * @param f the factor.
		 * @param p a Point reference.
		 * @return the inputted Point reference.
		 */
		public: static Point& interpolate(double x0, double y0, double x1, double y1, double f, Point& p) {
			p.x = (x1 - x0) * f + x0;
			p.y = (y1 - y0) * f + y0;
			return p;
		}
		
		/**
		 * @return a formatted string representation of a Point.
		 */
		public: std::string toString() {
			return "(" + brf::to_string(x) + ", " + brf::to_string(y) + ")";
		}
	};
}

#endif /* __brf_Point__ */
