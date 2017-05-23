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
#include "SYS/SYS_Version.h"
#include "UT/UT_ConcurrentHashMap.h"
#include "tbb/concurrent_unordered_set.h"

// Houdini 16 has definition of VEX_PodTypes, previous versions don't, it's taken from VEX/VEX_PodTypes.h
#if SYS_VERSION_MAJOR_INT < 16
	#include <VM/VM_Math.h>
	#include <UT/UT_Vector2.h>
	#include <UT/UT_Vector3.h>
	#include <UT/UT_Vector4.h>
	#include <UT/UT_Matrix2.h>
	#include <UT/UT_Matrix3.h>
	#include <UT/UT_Matrix4.h>

	using VEXfloat = fpreal32;
	using VEXint = int32;
	using VEXuint = uint32;
	using VEXvec2 = UT_Vector2;
	using VEXvec3 = UT_Vector3;
	using VEXvec4 = UT_Vector4;
	using VEXmat2 = UT_Matrix2;
	using VEXmat3 = UT_Matrix3;
	using VEXmat4 = UT_Matrix4;
	using VEXquaternion = UT_Quaternion;
#endif

class UT_String;
class UT_JSONValue;
struct VEX_VexOpArg;

struct VEX_JSONValueRefCounter;
struct VEX_JSONInstanceStorage;

///
/// \brief The VEX_JSONRefCounter struct
/// Place holder for UT_JSONValue object. This class is designed to be moved around only.
/// Because of legacy of STL copy constructor must be implemented.
/// Also internally it holds reference count. Referece counting is managed by VEX_JSONInstanceStorage.
///
struct VEX_JSONValueRefCounter
{
	VEX_JSONValueRefCounter(VEX_JSONValueRefCounter&& other);

	// Copy constructor must be present for maps and sets
	VEX_JSONValueRefCounter(const VEX_JSONValueRefCounter& other);
	~VEX_JSONValueRefCounter();

	VEX_JSONValueRefCounter& operator=(VEX_JSONValueRefCounter&& other);
	inline UT_JSONValue* operator->() const
	{
		return m_value.get();
	}

	friend struct VEX_JSONInstanceStorage;
private:
	VEX_JSONValueRefCounter(UT_JSONValue* value);
	VEX_JSONValueRefCounter& operator=(const VEX_JSONValueRefCounter&) = delete;

	uint32 m_count;

	// m_value must be shared because of copy constructor
	std::shared_ptr<UT_JSONValue> m_value;
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
/// VEX init function per function instance. Returned value is instance storage.
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
/// \brief VEX_GetJSONValue2
///
const UT_JSONValue* VEX_FindJSONValue(int argc, VEX_VexOpArg argv[], void* data);

///
/// \brief jsonTypeAsString
///
const char* VEX_jsonTypeAsString(const UT_JSONValue* value);

///
/// \brief vexTypeAsString
///
const char* VEX_vexTypeAsString(const VEX_VexOpArg* arg);

#endif // VEX_UTILS_H

