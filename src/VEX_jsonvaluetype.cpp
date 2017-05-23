#include "VEX_jsonvaluetype.h"

#include "VEX/VEX_VexOp.h"
#include "UT/UT_JSONValue.h"


void VEX_jsonvaluetype::evaluate(int argc, VEX_VexOpArg argv[], void* data)
{
	VEX_VexOpArg* status = &argv[0]; // output status
	VEX_VexOpArg* oerror = &argv[2]; // output error message
	VEX_VexOpArg* output = &argv[3]; // output value
	auto statusValue = reinterpret_cast<VEXint*>(status->myArg);
	const UT_JSONValue* value = VEX_FindJSONValue(argc, argv, data);

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
