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

#include <memory>

#include "SYS/SYS_Types.h"
#include "UT/UT_ConcurrentHashMap.h"
#include "UT/UT_JSONValue.h"

#include "tbb/concurrent_unordered_set.h"

class UT_String;
class UT_JSONValue;
struct VEX_VexOpArg;

///
/// \brief The VEX_JSONRefCounter struct
///
struct VEX_JSONValueRefCounter
{
	VEX_JSONValueRefCounter(std::unique_ptr<UT_JSONValue> ptr)
		: m_count(0), m_value(std::move(ptr))
	{
	}

	inline const UT_JSONValue* get() const
	{
		return m_value.get();
	}

	uint32 m_count;
	std::unique_ptr<UT_JSONValue> m_value;
};


using VEX_JSONStorage = UT_ConcurrentHashMap<uint32, VEX_JSONValueRefCounter>;

///
/// \brief The VEX_JSONLocalStorage struct
/// Stores data locally per function init.
///
struct VEX_JSONInstanceStorage
{
	static VEX_JSONInstanceStorage* create(VEX_JSONStorage* storage)
	{
		return new VEX_JSONInstanceStorage(storage);
	}

	~VEX_JSONInstanceStorage();

	const UT_JSONValue* getJSON(const UT_String& path);


	VEX_JSONStorage* m_storage;
	tbb::concurrent_unordered_set<uint32> m_hashSet;

private:
	VEX_JSONInstanceStorage(VEX_JSONStorage* storage);
};

///
/// \brief VEX_InitJSONStorage
/// VEX init function per function instance.
/// Returned value is instance storage.
///
void* VEX_InitJSONStorage();

///
/// \brief VEX_CleanupJSONStorage
///
void VEX_CleanupJSONStorage(void*);

///
/// \brief VEX_SetString
///
void* VEX_SetString(VEX_VexOpArg& arg, const char *value);

///
/// \brief VEX_GetJSONValue Iterative
///
const UT_JSONValue* VEX_GetJSONValue(const UT_JSONValue& value, const int& argc, const VEX_VexOpArg argv[], const int& firstIndex = 4);

#endif // DEFS_H
