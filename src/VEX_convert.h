#ifndef VEX_CONVERT_H
#define VEX_CONVERT_H

#include "UT/UT_ValArray.h"
#include "UT/UT_StringHolder.h"
#include "UT/UT_StringArray.h"
#include "VEX/VEX_VexOp.h"

// Base template for copy operation.
template<typename T, typename F, bool=true> struct to_vex_value_simple
{
	static void copy(T* to, const F& from)
	{
		*to = static_cast<T>(from);
	}
};

// S
template<typename T, typename F> struct to_vex_value_simple<T,F,false>
{
	static void copy(T* to, const F& from)
	{
		to->clear();
		to->setCapacity(from.entries());
		for(auto it = from.begin(); !it.atEnd(); ++it)
		{
			to->append(*it); // cast missing
		}
	}
};

// is_pod for char ptr returns false
template<> struct to_vex_value_simple<const char, UT_StringHolder, false>
{
	static void copy(const char* to, const UT_StringHolder& from)
	{
		VEX_VexOp::stringFree(to);
		to = VEX_VexOp::stringAlloc(from.c_str());
	}
};

//
template<> struct to_vex_value_simple<UT_ValArray<const char*>, UT_StringArray, false>
{
	static void copy(UT_ValArray<const char*>* to, const UT_StringArray& from)
	{
		for(exint i=0; i<from.entries(); ++i)
		{
			to->append(VEX_VexOp::stringAlloc(from(i).c_str()));
		}
	}
};

// Wrapper for pod types
template<typename T, typename F> struct to_vex_value :
		to_vex_value_simple<T,F, std::is_pod<T>::value && std::is_pod<F>::value>
{};

#endif // VEX_CONVERT_H
