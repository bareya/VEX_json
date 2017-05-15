#include "VEX_utils.h"

#include "VEX/VEX_VexOp.h"
#include "UT/UT_JSONValue.h"
#include "UT/UT_JSONValueMap.h"
#include "UT/UT_String.h"


// ***************************** VEX_JSONStorage ***************************** //
static VEX_JSONStorage vexJsonStorageNew;


// ***************************** VEX_JSONInstanceStorage ***************************** //
VEX_JSONInstanceStorage::VEX_JSONInstanceStorage(VEX_JSONStorage* storage)
	: m_storage(storage)
{
}


VEX_JSONInstanceStorage::~VEX_JSONInstanceStorage()
{
	// iterate over local elements and decrease reference count
}

const UT_JSONValue* VEX_JSONInstanceStorage::getJSON(const UT_String& path)
{
	uint32 hash = path.hash();
	VEX_JSONStorage::accessor a;

	if(!m_storage->find(a, hash))
	{
		std::unique_ptr<UT_JSONValue> json(new UT_JSONValue());
		if(!json->loadFromFile(path))
		{
			json.reset(nullptr); // deleter will be invoked
		}

		m_storage->emplace(a, hash, std::move(json));
	}

	return a->second.get();
}

// ***************************** INIT AND CLEAN FUNCTIONS ***************************** //
void* VEX_InitJSONStorage()
{
	return VEX_JSONInstanceStorage::create(&vexJsonStorageNew);
}


void VEX_CleanupJSONStorage(void* data)
{
	delete reinterpret_cast<VEX_JSONInstanceStorage*>(data);
}


// ***************************** SETTERS AND GETTERS ***************************** //
void* VEX_SetString(VEX_VexOpArg& arg, const char *value)
{
	VEX_VexOp::stringFree(reinterpret_cast<const char*>(arg.myArg));
	return reinterpret_cast<void*>(const_cast<char*>(VEX_VexOp::stringAlloc(value)));
}


const UT_JSONValue* VEX_GetJSONValue(const UT_JSONValue& jsonValue, const int& argc, const VEX_VexOpArg argv[], const int& firstIndex)
{
	// begining value
	const UT_JSONValue* value = &jsonValue;

	int argIndex = firstIndex;
	while(value && argIndex<argc)
	{
		const VEX_VexOpArg& arg = argv[argIndex];
		if(arg.myType == VEX_TYPE_INTEGER && value->getType() == UT_JSONValue::JSON_ARRAY)
		{
			auto idValue = reinterpret_cast<VEXint*>(arg.myArg);
			UT_JSONValueArray* array = value->getArray();
			value = array->get(*idValue);
		}
		else if(arg.myType == VEX_TYPE_STRING && value->getType() == UT_JSONValue::JSON_MAP)
		{
			auto keyValue = reinterpret_cast<const char*>(arg.myArg);
			UT_JSONValueMap* map = value->getMap();
			value = map->get(keyValue);
		}

		argIndex++;
	}

	return value;
}
