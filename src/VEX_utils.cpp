#include "VEX_utils.h"

#include "VEX/VEX_VexOp.h"
#include "UT/UT_JSONValue.h"
#include "UT/UT_JSONValueMap.h"
#include "UT/UT_String.h"


// ***************************** VEX_JSONStorage ***************************** //
static VEX_JSONStorage vexJsonStorageNew;


// ***************************** VEX_JSONValueRefCounter ***************************** //
VEX_JSONValueRefCounter::VEX_JSONValueRefCounter(UT_JSONValue* value)
	: m_count(0), m_value(value)
{
}


VEX_JSONValueRefCounter::VEX_JSONValueRefCounter(VEX_JSONValueRefCounter&& other)
	: m_count(other.m_count), m_value(std::move(other.m_value))
{
}


VEX_JSONValueRefCounter::~VEX_JSONValueRefCounter()
{
}


VEX_JSONValueRefCounter& VEX_JSONValueRefCounter::operator=(VEX_JSONValueRefCounter&& other)
{
	m_count = other.m_count;
	m_value = std::move(other.m_value);
	return *this;
}


// ***************************** VEX_JSONInstanceStorage ***************************** //
VEX_JSONInstanceStorage* VEX_JSONInstanceStorage::create()
{
	return new VEX_JSONInstanceStorage(&vexJsonStorageNew);
}


VEX_JSONInstanceStorage::VEX_JSONInstanceStorage(VEX_JSONStorage* storage)
	: m_gStorage(storage)
{
}


VEX_JSONInstanceStorage::~VEX_JSONInstanceStorage()
{
	// decrement count based on local set
	{
		VEX_JSONStorage::accessor a;
		for(auto it = m_hashSet.begin(); it!=m_hashSet.end(); ++it)
		{
			if(m_gStorage->find(a, *it) && a->second.m_count > 0)
			{
				a->second.m_count--;
			}
		}
	}

	// clean up zero count storage map
	for(auto it = m_gStorage->begin(); it!=m_gStorage->end(); ++it)
	{
		uint32 key = it->first;
		if(it->second.m_count == 0)
		{
			m_gStorage->erase(key);
		}
	}
}


const UT_JSONValue* VEX_JSONInstanceStorage::getJSON(const UT_String& path)
{
	uint32 hash = path.hash();
	VEX_JSONStorage::accessor a;

	if(!m_gStorage->find(a, hash))
	{
		VEX_JSONValueRefCounter json(new UT_JSONValue());
		if(!json->loadFromFile(path))
		{
			json.m_value.reset(nullptr); // deleter will be invoked
		}

		m_gStorage->emplace(a, hash, std::move(json));
	}

	if(m_hashSet.count(hash) == 0)
	{
		m_hashSet.emplace(hash);
		a->second.m_count++;
	}

	return a->second.m_value.get();
}


// ***************************** INIT AND CLEAN FUNCTIONS ***************************** //
void* VEX_InitJSONStorage()
{
	return VEX_JSONInstanceStorage::create();
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
