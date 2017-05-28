#ifndef VEX_JSONUTILS_H
#define VEX_JSONUTILS_H

#include "json.h"

#include <memory>
#include <string>

#include "SYS/SYS_Types.h"
#include "SYS/SYS_Version.h"

#include "UT/UT_ConcurrentHashMap.h"
#include "tbb/concurrent_unordered_set.h"

// Taken from Houdini 16's VEX/VEX_PodTypes.h
#if SYS_VERSION_MAJOR_INT < 16
	using VEXfloat = fpreal32;
	using VEXint = int32;
#endif

// mentioned for compiler
class UT_String;
class UT_JSONValue;
struct VEX_VexOpArg;

struct VEX_JSONValueRefCounter;
struct VEX_JSONInstanceStorage;

/// UT_JSONValue is a place holder for JSON data. As this document is written and as far as my knowledge goes
/// Place holder for UT_JSONValue object. This class is designed to be moved around only.
/// Because of legacy of STL copy constructor must be implemented.
/// Also internally it holds reference count. Referece counting is managed by VEX_JSONInstanceStorage.
struct VEX_JSONValueRefCounter final
{
	/// @{ move
	VEX_JSONValueRefCounter(VEX_JSONValueRefCounter&& other);
	VEX_JSONValueRefCounter& operator=(VEX_JSONValueRefCounter&& other);
	/// @}

	/// @{ copy, for map and set
	VEX_JSONValueRefCounter(const VEX_JSONValueRefCounter& other);
	/// @}

	~VEX_JSONValueRefCounter();

	friend struct VEX_JSONInstanceStorage;

private:
	/// Free creation is forbidden
	explicit VEX_JSONValueRefCounter(UT_JSONValue* value);

	/// Assigment is not allowed.
	VEX_JSONValueRefCounter& operator=(const VEX_JSONValueRefCounter&) = delete;

	/// non const helper operator extracting pointer
	inline UT_JSONValue* operator->() const
	{
		return m_value.get();
	}

	uint32 m_count; ///< count usages of value
	std::shared_ptr<UT_JSONValue> m_value; ///< Stores JSON file, must be shared because of copy constructor.
};

/// Typedef of hash map for storage type.
using VEX_JSONStorage = UT_ConcurrentHashMap<uint32, VEX_JSONValueRefCounter>;

/// Instance is created in VEX init, passed to evaluate and destroyed in cleanup.
struct VEX_JSONInstanceStorage final
{
	/// Instance must be created on the heap only.
	static VEX_JSONInstanceStorage* create();

	~VEX_JSONInstanceStorage();

	///
	const UT_JSONValue* getJSON(const char* path);

	friend struct VEX_JSONValueRefCounter;

private:
	///
	explicit VEX_JSONInstanceStorage(VEX_JSONStorage* storage);

	/// @{ noncopyable
	VEX_JSONInstanceStorage(const VEX_JSONInstanceStorage&) = delete;
	VEX_JSONInstanceStorage& operator=(const VEX_JSONInstanceStorage&) = delete;
	/// @}

	VEX_JSONStorage* m_gStorage; ///< Cached pointer to global storage
	tbb::concurrent_unordered_set<uint32> m_hashSet; ///<
};

/// VEX init function per function instance. Returned value is instance storage
void* VEX_InitJSONStorage();
void VEX_CleanupJSONStorage(void*);

/// Helper functions to convert C++ strings to VEX String
void* VEX_SetString(VEX_VexOpArg& arg, const char *value);
void* VEX_SetString(VEX_VexOpArg& arg, const std::string& value);

/// Type of data in JSONValue, UT_JSONValue performs conversion.
enum class JSONDataType
{
	Numeric,
	String,
	Compound,
	Unknown
};

JSONDataType VEX_getJSONDataType(const UT_JSONValue* value);

/// Returns string representation of JSON or VEX value
const char* VEX_jsonTypeAsString(const UT_JSONValue* value);
const char* VEX_vexTypeAsString(const VEX_VexOpArg& arg);

/// Search for value, returns nullptr if fails to find the value at given path
const UT_JSONValue* VEX_findJSONValue(int argc, VEX_VexOpArg argv[], void* data);
const UT_JSONValue* VEX_findJSONValue(const UT_JSONValue* inputValue, int argc, VEX_VexOpArg argv[]);

#endif//VEX_JSONUTILS_H

