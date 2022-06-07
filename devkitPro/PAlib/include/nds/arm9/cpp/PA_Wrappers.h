// C++ wrappers for some PAlib functionality

#pragma once

/*! \file PA_Wrappers.h
    \brief Simple C++ wrappers
*/

/*! \addtogroup cxx
	@{
*/

// Replacements for new and delete because the standard ones use exceptions.

/// Lightweight new operator
inline void* operator new(size_t size){
	return malloc(size);
}

/// Lightweight new operator
inline void* operator new[](size_t size){
	return malloc(size);
}

/// Lightweight delete operator
inline void operator delete(void* p){
	free(p);
}

/// Lightweight delete operator
inline void operator delete[](void* p){
	free(p);
}

namespace PA{
	/// Handle provider, use it to get dynamic sprite numbers for example
	template <int NHANDLES>
	class HandleProvider{
		int stack[NHANDLES];
		int sp;
	public:
		/// Constructor.
		inline HandleProvider(){
			for(int i = 0; i < NHANDLES; i ++)
				stack[i] = i;
			sp = NHANDLES;
		}

		/// Get a new handle.
		inline int newhandle(){
			PA_Assert((sp - 1) >= 0, "Out of free handles");
			return stack[--sp];
		}

		/// Delete a handle.
		inline void deletehandle(int handle){
			stack[sp++] = handle;
			PA_Assert(sp <= NHANDLES, "Too many handles");
		}
	};
};

/*! @} */
