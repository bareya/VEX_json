#ifndef DEFS_H
#define DEFS_H

#ifdef __GNUC__
	#define PRV_SYM __attribute__ ((visibility ("hidden")))
	#define PUB_SYM __attribute__ ((visibility ("default")))
#else
	#define PRV_SYM
	#define PUB_SYM
#endif

#define VEX_STATUS_FAILURE 0
#define VEX_STATUS_SUCCESS 1
#define VEX_STATUS_WARNING 2

#include <memory>

#include "SYS/SYS_Types.h"
#include "UT/UT_ConcurrentHashMap.h"

class UT_String;
class UT_JSONValue;
struct VEX_VexOpArg;

struct VEX_JSONStorage
{
	const UT_JSONValue* getJSON(const UT_String& path);

private:
	UT_ConcurrentHashMap<uint32, std::unique_ptr<UT_JSONValue>> m_storage;
};

///
/// \brief VEX_GetStorage
///
VEX_JSONStorage* VEX_GetJSONStorage();

///
/// \brief VEX_SetString
///
void* VEX_SetString(VEX_VexOpArg& arg, const char *value);

///
/// \brief VEX_GetJSONValue Iterative
///
const UT_JSONValue* VEX_GetJSONValue(const UT_JSONValue& value, const int& argc, const VEX_VexOpArg argv[], const int& firstIndex = 4);

///
/// \brief VEX_GetJSONMapValue
///
const UT_JSONValue* VEX_GetJSONMapValue(const UT_JSONValue& jsonValue, const VEX_VexOpArg& key);


#endif // DEFS_H
