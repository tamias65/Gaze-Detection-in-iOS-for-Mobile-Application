#ifndef __brf_Rectangle__
#define __brf_Rectangle__

#include "com/tastenkunst/cpp/brf/nxt/utils/StringUtils.hpp"

namespace brf {
	
	/**
	 * Defines a rectangle with x, y, width and height.
	 */
	class Rectangle {
		
		/**
		 * the x of the rectangle position.
		 */
		public: double x;

		/**
		 * the y of the rectangle position.
		 */
		public: double y;
		
		/**
		 * the width of the rectangle.
		 */
		public: double width;
		
		/**
		 * the radius of this ... just kidding ... the height of this rectangle.
		 */
		public: double height;

		/**
		 * Instantiates a brf::Rectangle.
		 *
		 * @param x - optional, the x of the rectangle position.
		 * @param y - optional, the y of the rectangle position.
		 * @param width - optional, the width of the rectangle.
		 * @param height - optional, the height of this rectangle.
		 */
		public: Rectangle(double x = 0.0, double y = 0.0, double width = 0.0, double height = 0.0) :
			x(x), y(y), width(width), height(height) {
		}

		/**
		 * Sets the values of an existing Rectangle all at once.
		 *
		 * @param x - optional, the x of the rectangle position.
		 * @param y - optional, the y of the rectangle position.
		 * @param width - optional, the width of the rectangle.
		 * @param height - optional, the height of this rectangle.
		 */
		public: void setTo(double _x = 0.0, double _y = 0.0, double _width = 0.0, double _height = 0.0) {
			x = _x;
			y = _y;
			width = _width;
			height = _height;
		}
		
		/**
		 * Returns whether a rectangle includes the params position.
		 *
		 * @param _x - mandatory, the x to check.
		 * @param _y - mandatory, the y to check.
		 * @return whether the position lies in this rectangle.
		 */
		public: bool contains(double _x, double _y) {
			if(_x >= x && _x <= x + width && _y >= y && _y <= y + height) {
				return true;
			}
			return false;
		}
		
		/**
		 * @return a formatted string representation of a Rectangle.
		 */
		public: std::string toString() {
			return "(" + brf::to_string(x) + ", " + brf::to_string(y) + ", " + brf::to_string(width) + ", " + brf::to_string(height) + ")";
		}
	};
}

#endif /* __brf_Rectangle__ */
