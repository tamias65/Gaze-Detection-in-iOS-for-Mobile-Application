#ifndef __brf_InputData__
#define __brf_InputData__

#include <cstdlib>
#include <cstdint>

namespace brf {

	/**
	 * The base class for our BitmapData implementations.
	 * The only thing BRF needs is the grayscaled data pointer
	 * from getData().
	 */
	class InputData {
		
		/**
		 * The width of the input data image.
		 */
		public: unsigned int width;
		
		/**
		 * The height of the input data image.
		 */
		public: unsigned int height;

		/**
		 * Instantiates this base class.
		 *
		 * @param width of the input data image.
		 * @param height of the input data image.
		 */
		public: InputData(unsigned int width, unsigned int height) :
				width(width),
				height(height) {
		}

		public: virtual ~InputData() {
		}
		
		/**
		 * Overwrite this method in your implementation to return
		 * the grayscaled data pointer.
		 */
		public: virtual uint8_t* getData() {
			return NULL;
		}
	};
}

#endif /* __brf_InputData__ */
