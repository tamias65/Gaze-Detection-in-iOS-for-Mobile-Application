#ifndef __brf_AllocUtils__
#define __brf_AllocUtils__

#include <cstdlib>

#include "com/tastenkunst/cpp/brf/nxt/utils/StringUtils.hpp"

namespace brf {

	/**
	 * Utility functions to alloc, free and align pointers.
	 * Borrowed from OpenCV memory alignment:
	 *
	 * Aligns pointer by the certain number of bytes.
	 *
	 * This small inline function aligns the pointer by the certian number of bytes by shifting
	 * it forward by 0 or a positive offset.
	 */
	template<typename _Tp>
	inline _Tp* alignPtr(_Tp* ptr, int n=(int)sizeof(_Tp)) {
		return (_Tp*)(((size_t)ptr + n-1) & -n);
	}

	/**
	 * Allocs and aligns a pointer
	 */
	inline void* fastMalloc(size_t size) {
		unsigned char* udata = (unsigned char*)malloc(size + sizeof(void*) + 16);
		if(!udata) {
			Err("Failed to allocate memory");
		}
		unsigned char** adata = brf::alignPtr((unsigned char**)udata + 1, 16);
		adata[-1] = udata;
		return adata;
	}
	
	/**
	 * Frees an aligned pointer.
	 */
	inline void fastFree(void* ptr) {
		if(ptr) {
			unsigned char* udata = ((unsigned char**)ptr)[-1];
			free(udata);
		}
	}
}

#endif /* __brf_AllocUtils__ */
