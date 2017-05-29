#ifndef VEX_IMPORT_H
#define VEX_IMPORT_H

#include "VEX_convert.h"

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

	/// generic data import
	static bool import(VEX_VexOpArg& to, const UT_JSONValue& from)
	{
		F fromValue;
		std::cout << fromValue << std::endl;
		if(from.import(fromValue)){
			T* toValue = reinterpret_cast<T*>(to.myArg);
			to_vex_value<T,F>::copy(toValue, fromValue);
			return true;
		}
		return false;
	}
};

// T and F are different types, data has to be casted and copied to storage type.
template<typename T,typename F> struct json_to_vex_same<T,F,false>
{
	static bool copy(VEX_VexOpArg& to, const UT_JSONValue& from)
	{
		if(!to.myArray){
			return json_to_vex_same<T,F,true>::import(to, from);
		}else{
			return json_to_vex_same<UT_ValArray<T>,UT_ValArray<F>,true>::import(to, from);
		}
	}
};

// String conversion requiures full specialization
template<> struct json_to_vex_same<const char*,UT_StringHolder,false>
{
	static bool copy(VEX_VexOpArg& to, const UT_JSONValue& from)
	{
		if(!to.myArray){
			return json_to_vex_same<const char,UT_StringHolder>::import(to, from);
		}else{
			return json_to_vex_same<UT_ValArray<const char*>,UT_StringArray,true>::import(to, from);
		}
	}
};

/// Wrapper with same type probing
template<typename T, typename F> struct json_to_vex :
		json_to_vex_same<T,F,std::is_same<T,F>::value>
{};

#endif // VEX_IMPORT_H
