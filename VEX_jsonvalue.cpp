#include "VEX_jsonvalue.h"

#include "VEX/VEX_VexOp.h"
#include "UT/UT_JSONValue.h"
#include "UT/UT_JSONValueArray.h"
#include "UT/UT_JSONValueMap.h"


void* VEX_jsonvalue::init()
{
	return nullptr;
}


void VEX_jsonvalue::evaluate(int argc, VEX_VexOpArg argv[], void*)
{
	VEX_VexOpArg* status = &argv[0];
	VEX_VexOpArg* inFile = &argv[1];
	VEX_VexOpArg* oerror = &argv[2];
	VEX_VexOpArg* output = &argv[3];

	auto statusValue = reinterpret_cast<VEXint*>(status->myArg);
	auto inFileValue = reinterpret_cast<const char*>(inFile->myArg);

	UT_JSONValue jsonFile;
	if(!jsonFile.loadFromFile(inFileValue))
	{
		oerror->myArg = VEX_SetString(*oerror, "File not found");
		*statusValue = VEX_STATUS_FAILURE;
		return;
	}

	const UT_JSONValue* value = &jsonFile;

	// iterate
	int index = 4;
	while(value && index<argc)
	{
		if(argv[index].myType == VEX_TYPE_INTEGER && value->getType() == UT_JSONValue::JSON_ARRAY)
		{
			auto idValue = reinterpret_cast<VEXint*>(argv[index].myArg);
			UT_JSONValueArray* array = value->getArray();
			value = array->get(*idValue);
		}
		else if(argv[index].myType == VEX_TYPE_STRING && value->getType() == UT_JSONValue::JSON_MAP)
		{
			auto keyValue = reinterpret_cast<const char*>(argv[index].myArg);
			UT_JSONValueMap* map = value->getMap();
			value = map->get(keyValue);
		}

		index++;
	}

	// output string
	if(value && value->getType() == UT_JSONValue::JSON_STRING)
	{
		output->myArg = VEX_SetString(*output, value->getS());
		*statusValue = VEX_STATUS_SUCCESS;
	}
	else
	{
		oerror->myArg = VEX_SetString(*oerror, "Map hasn't been found.");
		*statusValue = VEX_STATUS_FAILURE;
	}

}


void VEX_jsonvalue::cleanup(void*)
{
}
