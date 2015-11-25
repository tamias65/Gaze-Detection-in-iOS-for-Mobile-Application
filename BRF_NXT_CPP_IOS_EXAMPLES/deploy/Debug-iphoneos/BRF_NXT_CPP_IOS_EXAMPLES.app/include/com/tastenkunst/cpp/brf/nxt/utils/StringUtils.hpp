#ifndef __brf_StringUtils__
#define __brf_StringUtils__

#include <string>
#include <iostream>
#include <sstream>

namespace brf {

	/**
	 * Writes a message to the console.
	 *
	 * @param the message
	 */
	void trace(std::string msg) {
		std::cout << msg << std::endl;
	}

	/**
	 * Writes an error message to the console.
	 *
 	 * @param the message
	 */
	void Err(std::string msg) {
		std::cout << "Error: " << msg << std::endl;
	}

	/**
	 * Converts all kind of types into strings for use in trace.
	 *
	 * @param the value to convert
	 * @see trace
	 */
	template < typename T >
	std::string to_string(const T& n) {
		std::ostringstream strStream;
		strStream << n;
		return strStream.str();
	}
	
	/**
	 * Tests whether a pointer is NULL and returns 1 (true) or 0 (false)
	 *
	 * @param the pointer to test
	 */
	template < typename T >
	std::string isNULL(T* ptr) {
		if(ptr == NULL) {
			return brf::to_string(1);
		} else {
			return brf::to_string(0);
		}
	}
}

#endif /* __brf_StringUtils__ */
