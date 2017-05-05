#include "VEX_jsonkeys.h"

#include "VEX_convert.h"

#include "VEX/VEX_VexOp.h"
#include "UT/UT_JSONValue.h"

void VEX_jsonkeys::evaluate(int argc, VEX_VexOpArg argv[], void* data)
{
	VEX_VexOpArg* status = &argv[0]; // output status
	VEX_VexOpArg* infile = &argv[1]; // input file
	VEX_VexOpArg* oerror = &argv[2]; // output error message
	VEX_VexOpArg* output = &argv[3]; // output value

	// some variables
	VEXint* statusValue = reinterpret_cast<VEXint*>(status->myArg);
	const char* infileValue = reinterpret_cast<const char*>(infile->myArg);
	VEX_JSONInstanceStorage* storage = reinterpret_cast<VEX_JSONInstanceStorage*>(data);

	*statusValue = VEX_STATUS_SUCCESS;

	// check if file exists and it's possible to parse
	const UT_JSONValue* jsonFile = storage->getJSON(infileValue);

	// Fail to parse file or file not found.
	if(!jsonFile)
	{
		statusValue =VEX_STATUS_FAILURE;
		oerror->myArg = VEX_SetString(*oerror, std::string("File:") + "not found or is not a JSON file.");
		return;
	}

	// Search for value, returns nullptr if fails to find the value at given path
	const UT_JSONValue* value = VEX_findJSONValue(jsonFile, argc, argv);

	// Fail to find a value at given path.
	if(!value)
	{
		statusValue = VEX_STATUS_FAILURE;
		oerror->myArg = VEX_SetString(*oerror, "Value has not been found!");
		return;
	}

	// output string[]
	if(value && value->getType() == UT_JSONValue::JSON_MAP)
	{
		UT_StringArray mapKeys;
		value->getMap()->getKeys(mapKeys);

		auto keysArray = reinterpret_cast<UT_ValArray<const char *>*>(output->myArg);
		to_vex_value<UT_ValArray<const char*>, UT_StringArray>::copy(keysArray, mapKeys);
		*statusValue = VEX_STATUS_SUCCESS;
	}
	else
	{
		oerror->myArg = VEX_SetString(*oerror, "Map hasn't been found.");
		*statusValue = VEX_STATUS_FAILURE;
	}
}
