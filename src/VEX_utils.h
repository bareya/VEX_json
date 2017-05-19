#ifndef VEX_UTILS_H
#define VEX_UTILS_H

#ifdef __GNUC__
	#define PRV_SYM __attribute__ ((visibility ("hidden")))
	#define PUB_SYM __attribute__ ((visibility ("default")))
#else
	#define PRV_SYM
	#define PUB_SYM
#endif

#include "vexjson.h"

#include <memory>

#include "SYS/SYS_Types.h"
#include "UT/UT_ConcurrentHashMap.h"
#include "tbb/concurrent_unordered_set.h"

class UT_String;
class UT_JSONValue;
struct VEX_VexOpArg;

struct VEX_JSONValueRefCounter;
struct VEX_JSONInstanceStorage;

///
/// \brief The VEX_JSONRefCounter struct
/// Place holder for UT_JSONValue object. This class is designed to be moved around only, no copying is allowed.
/// Also internally it holds reference count. Referece counting is managed by VEX_JSONInstanceStorage.
///
struct VEX_JSONValueRefCounter
{
	VEX_JSONValueRefCounter(VEX_JSONValueRefCounter&& other);
	~VEX_JSONValueRefCounter();

	VEX_JSONValueRefCounter& operator=(VEX_JSONValueRefCounter&& other);
	inline UT_JSONValue* operator->() const
	{
		return m_value.get();
	}

	friend struct VEX_JSONInstanceStorage;
private:
	VEX_JSONValueRefCounter(UT_JSONValue* value);
	VEX_JSONValueRefCounter(const VEX_JSONValueRefCounter&) = delete;
	VEX_JSONValueRefCounter& operator=(const VEX_JSONValueRefCounter&) = delete;

	uint32 m_count;
	std::unique_ptr<UT_JSONValue> m_value;
};

///
/// \brief VEX_JSONStorage
/// Global storage for all JSON files
///
using VEX_JSONStorage = UT_ConcurrentHashMap<uint32, VEX_JSONValueRefCounter>;

///
/// \brief The VEX_JSONLocalStorage struct
/// This class is designed to be passed between init, evaluate and clean VEX functions.
/// It holds a pointer to JSON global storage.
///
struct VEX_JSONInstanceStorage
{
	static VEX_JSONInstanceStorage* create();
	~VEX_JSONInstanceStorage();

	const UT_JSONValue* getJSON(const UT_String& path);

	friend struct VEX_JSONValueRefCounter;
private:
	VEX_JSONInstanceStorage(VEX_JSONStorage* storage);

	VEX_JSONStorage* m_gStorage;
	tbb::concurrent_unordered_set<uint32> m_hashSet;
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



const UT_JSONValue* VEX_GetJSONValue2(int argc, VEX_VexOpArg argv[], void* data);

#endif // VEX_UTILS_H

