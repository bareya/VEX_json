#include "VEX_jsontype.h"

#include "VEX/VEX_VexOp.h"
#include "UT/UT_JSONValue.h"


void VEX_jsontype::evaluate(int argc, VEX_VexOpArg argv[], void* data)
{
	VEX_VexOpArg* status = &argv[0]; // output status
	VEX_VexOpArg* infile = &argv[1]; // input file
	VEX_VexOpArg* oerror = &argv[2]; // output error message
	VEX_VexOpArg* output = &argv[3]; // output value

	auto statusValue = reinterpret_cast<VEXint<VEX_64>*>(status->myArg);
	const char* infileValue = reinterpret_cast<const char*>(infile->myArg);
	VEX_JSONInstanceStorage* storage = reinterpret_cast<VEX_JSONInstanceStorage*>(data);

	const UT_JSONValue* jsonFile = storage->getJSON(infileValue);

	if(!jsonFile)
	{
		statusValue =VEX_STATUS_FAILURE;
		oerror->myArg = VEX_SetString(*oerror, std::string("File:") + "not found or is not a JSON file.");
		return;
	}

	const UT_JSONValue* value = VEX_findJSONValue(jsonFile, argc, argv);

	if(value)
	{
		output->myArg = VEX_SetString(*output, VEX_jsonTypeAsString(value));
		*statusValue = VEX_STATUS_SUCCESS;
	}
	else
	{
		oerror->myArg = VEX_SetString(*oerror, "Value path hasn't been found.");
		*statusValue = VEX_STATUS_FAILURE;
	}
}
