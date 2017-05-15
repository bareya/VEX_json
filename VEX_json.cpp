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


struct OutputData
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

	// TODO FOR TESTING READ EVERY EXECUTION
	UT_JSONValue jsonFile;
	if(!jsonFile.loadFromFile(filepath))
	{
		VEX_VexOp::stringFree(reinterpret_cast<const char*>(argv[2].myArg));
		argv[2].myArg = const_cast<char*>(VEX_VexOp::stringAlloc("File is not a JSON file or can not be found."));
		*result = 0;
		return;
	}


	const UT_JSONValue *value = &jsonFile;
	// 0 result, 1 path, 2 errmsg, 3 output, 4+ variadic
	// variadic: int, string
	for(int i=4; i<argc; ++i)
	{
		// check if is an array

		// check for correct type
		switch(argv[i].myType)
		{
			case VEX_TYPE_STRING:
			{
				if(value->getType()==UT_JSONValue::JSON_MAP)
				{
					const char* varArg = reinterpret_cast<const char*>(argv[i].myArg);
					UT_JSONValueMap *valueMap = value->getMap();;
					value = valueMap->get(varArg);
					if( value == nullptr )
					{
						VEX_VexOp::stringFree(reinterpret_cast<const char*>(argv[2].myArg));
						argv[2].myArg = const_cast<char*>(VEX_VexOp::stringAlloc("Key can not be found"));
						*result = 0;
						return;
					}

					break;
				}
				else
				{
					VEX_VexOp::stringFree(reinterpret_cast<const char*>(argv[2].myArg));
					argv[2].myArg = const_cast<char*>(VEX_VexOp::stringAlloc("Variadic argument syntax error, required string."));
					*result = 0;
					return;
				}
			}
			default:
			{
				VEX_VexOp::stringFree(reinterpret_cast<const char*>(argv[2].myArg));
				argv[2].myArg = const_cast<char*>(VEX_VexOp::stringAlloc("Variadic Stirng or Integer arguments only."));
				*result = 0;
				return;
			}
		}
	}

	// get value
	if( value->getType() == UT_JSONValue::JSON_STRING && argv[3].myType == VEX_TYPE_STRING )
	{
		VEX_VexOp::stringFree(reinterpret_cast<const char*>(argv[3].myArg));
		argv[3].myArg = const_cast<char*>(VEX_VexOp::stringAlloc(value->getS()));
	}
	else
	{
		VEX_VexOp::stringFree(reinterpret_cast<const char*>(argv[2].myArg));
		argv[2].myArg = const_cast<char*>(VEX_VexOp::stringAlloc("Value type is not string"));
		*result = 0;
		return;
	}

	// success
	*result = 1;
}

//https://www.originpc.com/gaming/laptops/eon17-slx/
static void json_Cleanup(void *initData)
{
	if(initData)
	{
		delete reinterpret_cast<InitData*>(initData);
	}
}


void newVEXOp(void *data)
{
	// int json(string json, string &errmsg, string &output, ...);
	new VEX_VexOp("json@&IS&S&S+", json_Evaluate, VEX_ALL_CONTEXT, json_Init, json_Cleanup, VEX_OPTIMIZE_2, true);
//	new VEX_VexOp("json@&IS&S&S+", json_Evaluate, VEX_ALL_CONTEXT, json_Init, json_Cleanup, VEX_OPTIMIZE_2, true);
}
