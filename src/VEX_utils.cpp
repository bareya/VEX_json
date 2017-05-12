#include "VEX_utils.h"

#include "VEX/VEX_VexOp.h"
#include "UT/UT_JSONValue.h"
#include "UT/UT_JSONValueMap.h"
#include "UT/UT_String.h"


static VEX_JSONStorage vexJsonStorage;


const UT_JSONValue* VEX_JSONStorage::getJSON(const UT_String& path)
{
	uint32 hash = path.hash();
	tbb::concurrent_hash_map<uint32, std::unique_ptr<UT_JSONValue>>::accessor a;

	if(!m_storage.find(a, hash))
	{
		std::unique_ptr<UT_JSONValue> json(new UT_JSONValue());
		if(!json->loadFromFile(path))
		{
			json.reset(nullptr);
		}

		m_storage.emplace(a, hash, std::move(json));
	}

	return a->second.get();
}


VEX_JSONStorage* VEX_GetJSONStorage()
{
	return &vexJsonStorage;
}


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


const UT_JSONValue* VEX_GetJSONMapValue(const UT_JSONValue& value, const VEX_VexOpArg& key)
{
	const char* keyValue = reinterpret_cast<const char*>(key.myArg);

	UT_JSONValueMap *map = value.getMap();
	const UT_JSONValue* newValue = map->get(keyValue);
	if(!newValue)
	{
		return nullptr;
	}

	return newValue;
}
