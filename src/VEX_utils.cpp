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


VEX_JSONValueRefCounter::VEX_JSONValueRefCounter(const VEX_JSONValueRefCounter& other)
	: m_count(other.m_count), m_value(other.m_value)
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
			json.m_value = nullptr;
		}

#if SYS_VERSION_MAJOR_INT < 16
		m_gStorage->insert(a, std::pair<uint32, VEX_JSONValueRefCounter>(hash, std::move(json)));
#else
		m_gStorage->emplace(a, hash, std::move(json));
#endif
	}

	if(m_hashSet.count(hash) == 0)
	{
#if SYS_VERSION_MAJOR_INT < 16
		m_hashSet.insert(hash);
#else
		m_hashSet.emplace(hash);
#endif
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


void* VEX_SetString(VEX_VexOpArg &arg, const std::string& value)
{
	VEX_VexOp::stringFree(reinterpret_cast<const char*>(arg.myArg));
	return reinterpret_cast<void*>(const_cast<char*>(VEX_VexOp::stringAlloc(value.c_str())));
}


// ***************************** JSON FILE ITERATOR ***************************** //
const UT_JSONValue* VEX_FindJSONValue(int argc, VEX_VexOpArg argv[], void* data)
{
	// map default values
	VEX_VexOpArg* status = &argv[0]; // output status
	VEX_VexOpArg* inFile = &argv[1]; // input json file
	VEX_VexOpArg* oerror = &argv[2]; // output error message
	// VEX_VexOpArg* output = &argv[3]; // not used output value

	auto statusValue = reinterpret_cast<VEXint*>(status->myArg);
	auto inFileValue = reinterpret_cast<const char*>(inFile->myArg);

	VEX_JSONInstanceStorage* storage = reinterpret_cast<VEX_JSONInstanceStorage*>(data);
	const UT_JSONValue* jsonCache = storage->getJSON(inFileValue);
	if(!jsonCache)
	{
		*statusValue = VEX_STATUS_FAILURE;
		oerror->myArg = VEX_SetString(*oerror, "File not found");
		return nullptr;
	}

	const UT_JSONValue* value = jsonCache;

	int argIndex = 4;
	while(value && argIndex<argc)
	{
		const VEX_VexOpArg& arg = argv[argIndex];
		if(arg.myType == VEX_TYPE_INTEGER && value->getType() == UT_JSONValue::JSON_ARRAY)
		{
			auto idValue = reinterpret_cast<VEXint*>(arg.myArg);
			value = value->getArray()->get(*idValue);
		}
		else if(arg.myType == VEX_TYPE_STRING && value->getType() == UT_JSONValue::JSON_MAP)
		{
			auto keyValue = reinterpret_cast<const char*>(arg.myArg);
			value = value->getMap()->get(keyValue);
		}
		else
		{
			*statusValue = VEX_STATUS_FAILURE;
			oerror->myArg = VEX_SetString(*oerror, std::string("Can't access JSON type:") + VEX_jsonTypeAsString(value) + " using: " + VEX_vexTypeAsString(arg));
			return nullptr;
		}

		argIndex++;
	}

	return value;
}


//
const char* VEX_jsonTypeAsString(const UT_JSONValue* value)
{
	switch(value->getType())
	{
		case UT_JSONValue::JSON_NULL:
		{
			return "null";
		}
		case UT_JSONValue::JSON_BOOL:
		{
			return "bool";
		}
		case UT_JSONValue::JSON_INT:
		{
			return "int";
		}
		case UT_JSONValue::JSON_REAL:
		{
			return "float";
		}
		case UT_JSONValue::JSON_STRING:
		{
			return "string";
		}
		case UT_JSONValue::JSON_KEY:
		{
			return "key";
		}
		case UT_JSONValue::JSON_ARRAY:
		{
			return "array";
		}
		case UT_JSONValue::JSON_MAP:
		{
			return "map";
		}
		default:
		{
			return "unknown";
		}
	}
}


const char* VEX_vexTypeAsString(const VEX_VexOpArg& arg)
{
	switch(arg.myType)
	{
		case VEX_TYPE_INTEGER:
		{
			return "int";
		}
		case VEX_TYPE_FLOAT:
		{
			return "float";
		}
		case VEX_TYPE_STRING:
		{
			return "string";
		}
	}
}
