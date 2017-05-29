#ifndef VEX_CONVERT_H
#define VEX_CONVERT_H

#include "UT/UT_ValArray.h"

// Base template for copy operation.
template<typename T, typename F, bool=true> struct to_vex_value_simple
{
	static void copy(T& to, const F& from)
	{
		to = static_cast<T>(from);
	}
};

// S
template<typename T, typename F> struct to_vex_value_simple<T,F,false>
{
	static void copy(T& to, const F& from)
	{
		to.clear();
		to.setCapacity(from.entries());
		for(auto it = from.begin(); !it.atEnd(); ++it)
		{
			to.append(*it); // cast missing
		}
	}
};

// Wrapper for pod types
template<typename T, typename F> struct to_vex_value :
		to_vex_value_simple<T,F, std::is_pod<T>::value && std::is_pod<F>::value>
{};

#endif // VEX_CONVERT_H
