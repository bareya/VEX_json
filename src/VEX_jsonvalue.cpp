#include "VEX_jsonvalue.h"
#include "VEX_import.h"

#include "UT/UT_JSONValueMap.h"

void VEX_jsonvalue::evaluate(int argc, VEX_VexOpArg argv[], void* data)
{
	VEX_VexOpArg* status = &argv[0]; // output status
	VEX_VexOpArg* infile = &argv[1]; // input file
	VEX_VexOpArg* oerror = &argv[2]; // output error message
	VEX_VexOpArg* output = &argv[3]; // output value

	// some variables
	auto statusValue = reinterpret_cast<VEXint<VEX_64>*>(status->myArg);
	auto infileValue = reinterpret_cast<const char*>(infile->myArg);
	auto storage = reinterpret_cast<VEX_JSONInstanceStorage*>(data);

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

	// output type defines data conversion, note that import might fail, because wrong data access
	switch(output->myType)
	{
		case VEX_TYPE_INTEGER:
		{
			*statusValue = json_to_vex<VEXint<VEX_64>,int64>::copy(*output, *value);
			if(!*statusValue){
				oerror->myArg = VEX_SetString(*oerror, "Importing int value failed");
			}
			return;
		}
		case VEX_TYPE_FLOAT:
		{
			*statusValue = json_to_vex<VEXfloat<VEX_64>,fpreal64>::copy(*output, *value);
			if(!*statusValue){
				oerror->myArg = VEX_SetString(*oerror, "Importing float value failed");
			}
			return;
		}
		case VEX_TYPE_STRING:
		{
			*statusValue = json_to_vex<const char*,UT_StringHolder>::copy(*output, *value);
			if(!*statusValue){
				oerror->myArg = VEX_SetString(*oerror, "Importing string value failed");
			}
			return;
		}
		default:
		{
			*statusValue = false;
			return;
		}
	}
}
