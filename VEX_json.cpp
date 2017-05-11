#include <iostream>

#include "UT/UT_DSOVersion.h"
#include "UT/UT_JSONValue.h"
#include "UT/UT_String.h"
#include "UT/UT_Map.h"
#include "UT/UT_Lock.h"
#include "UT/UT_WorkBuffer.h"
#include "UT/UT_IndexedHashMap.h"
#include "UT/UT_ConcurrentHashMap.h"

#include "VEX/VEX_VexOp.h"
#include "VEX/VEX_PodTypes.h"

#include "VEX_jsonmapkeys.h"
#include "VEX_jsonvalue.h"

//struct JSONFile
//{
//	JSONFile(const char* file)
//		: m_loaded(false)
//	{
//		m_loaded = m_file.loadFromFile(file);
//	}

//	bool m_loaded;
//	UT_JSONValue m_file;
//};

//static UT_ConcurrentHashMap<const char*, JSONFile> jsonFileTable;

struct InitData
{

};

static void *json_Init()
{
	return new InitData();
}


static void json_Evaluate(int argc, VEX_VexOpArg argv[], void *initData)
{
	// output, filepath
	VEXint *result = reinterpret_cast<VEXint*>(argv[0].myArg);
	const char *filepath = reinterpret_cast<const char*>(argv[1].myArg);

	// json table
	//auto jsonTable = reinterpret_cast<UT_ConcurrentHashMap<const char*, JSONFile>*>(data);

	// find in the table
	const UT_JSONValue *value = nullptr;

	UT_JSONValue jsonFile;
	jsonFile.loadFromFile(filepath);
//	{
//		// UT_AutoLock lock(jsonFileTableLock);
//		// hash map accessor
//		std::cout << filepath << std::endl;
//	}

	// file hsan't been loaded
	if(!value)
	{
		VEX_VexOp::stringFree(reinterpret_cast<const char*>(argv[2].myArg));
		argv[2].myArg = const_cast<char*>(VEX_VexOp::stringAlloc("File is not a JSON file or can not be found."));
		*result = 0;
		return;
	}

	// 0 result, 1 errmsg, 2 output, 3+ variadic
	// variadic: int, string
	for(int i=3; i<argc; ++i)
	{
		//std::cout << argv[i].myType << std::endl;
	}

	// success
	*result = 1;
}

static void json_Cleanup(void *initData)
{
	if(initData)
	{
		delete reinterpret_cast<InitData*>(initData);
	}
}


//template<typename T>
//void newVexOp(const char *nameAndArgs)
//{
//	new VEX_VexOp(nameAndArgs, T::evaluate, VEX_ALL_CONTEXT, T::init, T::cleanup, VEX_OPTIMIZE_2, true);
//}


PUB_SYM void newVEXOp(void *data)
{
//	int json(string json, string &errmsg, string &output, ...);
//	0			1			2				3			4
//	new VEX_VexOp("json@&IS&S&S+", json_Evaluate, VEX_ALL_CONTEXT, json_Init, json_Cleanup, VEX_OPTIMIZE_2, true);

//	returns an array of keys based on provided path to the map
//	newVexOp<VEX_jsonmapkeys>("jsonmapkeys@&IS&S&[S+");
	new VEX_VexOp("jsonmapkeys@&IS&S&[S+", VEX_jsonmapkeys::evaluate, VEX_ALL_CONTEXT, VEX_jsonmapkeys::init, VEX_jsonmapkeys::cleanup, VEX_OPTIMIZE_2, true);
	new VEX_VexOp("jsonvalue@&IS&S&S+", VEX_jsonvalue::evaluate, VEX_ALL_CONTEXT, VEX_jsonvalue::init, VEX_jsonvalue::cleanup, VEX_OPTIMIZE_2, true);
}
