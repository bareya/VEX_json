#include "VEX_jsonvaluetype.h"

#include "VEX/VEX_VexOp.h"
#include "UT/UT_JSONValue.h"


void VEX_jsonvaluetype::evaluate(int argc, VEX_VexOpArg argv[], void* data)
{
	VEX_VexOpArg* status = &argv[0]; // output status
	VEX_VexOpArg* oerror = &argv[2]; // output error message
	VEX_VexOpArg* output = &argv[3]; // output value
	auto statusValue = reinterpret_cast<VEXint*>(status->myArg);
	const UT_JSONValue* value = VEX_GetJSONValue(argc, argv, data);

	if(value)
	{
		switch(value->getType())
		{
			case UT_JSONValue::JSON_NULL:
			{
				output->myArg = VEX_SetString(*output, "null");
				break;
			}
			case UT_JSONValue::JSON_BOOL:
			{
				output->myArg = VEX_SetString(*output, "bool");
				break;
			}
			case UT_JSONValue::JSON_INT:
			{
				output->myArg = VEX_SetString(*output, "int");
				break;
			}
			case UT_JSONValue::JSON_REAL:
			{
				output->myArg = VEX_SetString(*output, "float");
				break;
			}
			case UT_JSONValue::JSON_STRING:
			{
				output->myArg = VEX_SetString(*output, "string");
				break;
			}
			case UT_JSONValue::JSON_KEY:
			{
				output->myArg = VEX_SetString(*output, "key");
				break;
			}
			case UT_JSONValue::JSON_ARRAY:
			{
				output->myArg = VEX_SetString(*output, "array");
				break;
			}
			case UT_JSONValue::JSON_MAP:
			{
				output->myArg = VEX_SetString(*output, "map");
				break;
			}
		}

		*statusValue = VEX_STATUS_SUCCESS;
	}
	else
	{
		oerror->myArg = VEX_SetString(*oerror, "Value path hasn't been found.");
		*statusValue = VEX_STATUS_FAILURE;
	}
}
