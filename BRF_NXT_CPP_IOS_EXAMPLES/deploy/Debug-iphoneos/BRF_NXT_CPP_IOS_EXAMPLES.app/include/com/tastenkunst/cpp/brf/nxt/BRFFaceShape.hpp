#ifndef __brf_BRFFaceShape__
#define __brf_BRFFaceShape__

#include <vector>
#include <memory>
#include <algorithm>

#include "com/tastenkunst/cpp/brf/nxt/geom/Point.hpp"
#include "com/tastenkunst/cpp/brf/nxt/geom/Rectangle.hpp"

namespace brf {
	
	/**
	 * <p>BRFFaceShape includes all detailed face properties and features. Eg. points, bounds etc.</p>
	 * <p>See FaceShape_points.jpg (below) for index access of points (eg. left eye center (index: 31) or chin (index: 7)).</p>
	 * <p><img src="https://tastenkunst.s3.amazonaws.com/website/content/brf/FaceShape_points.jpg" /></p>
	 * <p>When _brfManager.candideEnabled == true a Candide 3 model, its vertices and scale, translationX, translationY, 
	 * rotationX, rotationY, rotationZ values, is updated as well. The candide vertices are 2D, but transformed by 
	 * these transform values (scale, translation, rotation).</p>
	 * <p>6 AcionsUnits for brows, mouth and jaw are also implemented (activate it by setting _brfManager.candideActionUnitsEnabled = true).</p>
	 */
	class BRFFaceShape {

		/**
		 * Holds the same values as points as a one dimensional vector (x,y,x,y...).
		 */
		public: std::vector< double > faceShapeVertices;
		
		/**
		 * Used to know, which triangles to draw.
		 */
		public: std::vector< int > faceShapeTriangles;

		/**
		 * 68 points of the face shape. See FaceShape_points.jpg for index access.
		 */
		public: std::vector< std::shared_ptr<Point> > points;
		
		/**
		 * Outline rectangle of the face shape. Used for easy access of x, y, width and height of the face shape.
		 */
		public: Rectangle bounds;
		
		/**
		 * Holds the vertices of the candide shape as a one dimensional vector (x,y,x,y...).
		 */
		public: std::vector< double > candideShapeVertices;
		
		/**
		 * Used to know, which triangles to draw.
		 */
		public: std::vector< int > candideShapeTriangles;

		/**
		 * The scale of the model multiplied by 180 to match the standard face width.
		 */
		public: double scale;
		
		/**
		 * The x translation of the model.
		 */
		public: double translationX;
		
		/**
		 * The y translation of the model.
		 */
		public: double translationY;
		
		/**
		 * The x rotation of the model.
		 */
		public: double rotationX;
		
		/**
		 * The y rotation of the model.
		 */
		public: double rotationY;
		
		/**
		 * The z rotation of the model.
		 */
		public: double rotationZ;

		/**
		 * @private
		 */
		public: BRFFaceShape() :
				faceShapeVertices(0),
				faceShapeTriangles(0),
				points(0),
				bounds(),

				candideShapeVertices(0),
				candideShapeTriangles(0),

				scale(1.0),
				translationX(0.0),
				translationY(0.0),
				rotationX(0.0),
				rotationY(0.0),
				rotationZ(0.0) {

			int triangles[] = {
					 0,  1, 27,  0, 21, 27,  1,  2, 39,  1, 27, 30, //12
					 1, 30, 39,  2,  3, 48,  2, 39, 48,  3,  4, 48,
					 4,  5, 48,  5,  6, 58,  5, 48, 59,  5, 58, 59,
					 6,  7, 57,  6, 57, 58,  7,  8, 56,  7, 56, 57,
					 8,  9, 55,  8, 55, 56,  9, 10, 54,  9, 54, 55,
					10, 11, 54, 11, 12, 54, 12, 13, 43, 12, 43, 54,
					13, 14, 32, 13, 32, 35, 13, 35, 43, 14, 15, 32,
					15, 16, 20, 15, 20, 32, 16, 17, 20, 17, 18, 19,
					17, 18, 23, 17, 19, 20, 18, 19, 34, 18, 23, 24,
					18, 24, 37, 18, 34, 45, 18, 37, 45, 19, 20, 33, //line 10
					19, 33, 34, 20, 32, 33, 21, 22, 26, 21, 26, 27,
					22, 23, 26, 23, 24, 25, 23, 25, 26, 24, 25, 29,
					24, 29, 37, 25, 26, 28, 25, 28, 29, 26, 27, 28,
					27, 28, 31, 27, 30, 31, 28, 29, 31, 29, 30, 31,
					29, 30, 38, 29, 37, 38, 30, 38, 39, 32, 33, 36,
					32, 35, 36, 33, 34, 36, 34, 35, 36, 34, 35, 44,
					34, 44, 45, 35, 43, 44, 37, 38, 45, 38, 39, 67,
					38, 44, 45, 38, 44, 67, 39, 40, 46, 39, 40, 48,
					39, 46, 67, 40, 41, 46, 40, 41, 50, 40, 48, 49,
					40, 49, 50, 41, 42, 47, 41, 42, 52, 41, 46, 67, //line 20
					41, 47, 67, 41, 50, 51, 41, 51, 52, 42, 43, 47,
					42, 43, 54, 42, 52, 53, 42, 53, 54, 43, 44, 47,
					44, 47, 67, 48, 49, 65, 48, 59, 60, 48, 60, 65,
					49, 50, 65, 50, 51, 64, 50, 64, 65, 51, 52, 64,
					52, 53, 63, 52, 63, 64, 53, 54, 63, 54, 55, 62,
					54, 62, 63, 55, 56, 62, 56, 57, 61, 56, 61, 62,
					57, 58, 61, 58, 59, 60, 58, 60, 61, 60, 61, 66,
					60, 65, 66, 61, 62, 66, 62, 63, 66, 63, 64, 66, //line 28
					64, 65, 66}; //length 339
			faceShapeTriangles.resize(339);
			std::copy( triangles, triangles+339, faceShapeTriangles.begin());
		}
	};
}

#endif /* __brf_BRFFaceShape__ */
