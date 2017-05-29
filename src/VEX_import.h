#ifndef VEX_IMPORT_H
#define VEX_IMPORT_H

#include "VEX_convert.h"

#include "VEX/VEX_VexOp.h"
#include "UT/UT_JSONValue.h"
#include "UT/UT_JSONValueArray.h"

// T and F are same types, data improted directly to VEX
template<typename T,typename F,bool=true> struct json_to_vex_same
{
	static bool copy(VEX_VexOpArg& to, const UT_JSONValue& from)
	{
		if(!to.myArray){
			return from.import(*reinterpret_cast<T*>(to.myArg));
		}else{
			return from.import(*reinterpret_cast<UT_ValArray<T>*>(to.myArg));
		}
	}
};

// T and F are different types, data has to be casted and copied to storage type.
template<typename T,typename F> struct json_to_vex_same<T,F,false>
{
	static bool copy(VEX_VexOpArg& to, const UT_JSONValue& from)
	{
		if(!to.myArray){
			return json_to_vex_same<T,F,false>::import(to, from);
		}else{
			return json_to_vex_same<UT_ValArray<T>,UT_ValArray<F>,false>::import(to, from);
		}
	}

	/// generic data import
	static bool import(VEX_VexOpArg& to, const UT_JSONValue& from)
	{
		F fromValue;
		if(from.import(fromValue)){
			T* toValue = reinterpret_cast<T*>(to.myArg);
			to_vex_value<T,F>::copy(*toValue, fromValue);
			return true;
		}
		return false;
	}
};

/// Wrapper with same type probing
template<typename T, typename F> struct json_to_vex :
		json_to_vex_same<T,F,std::is_same<T,F>::value>
{};

#endif // VEX_IMPORT_H
