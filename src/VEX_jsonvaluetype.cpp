#include "VEX_jsonvaluetype.h"

#include "VEX/VEX_VexOp.h"


void VEX_jsonvaluetype::evaluate(int argc, VEX_VexOpArg argv[], void* data)
{
	VEX_VexOpArg* status = &argv[0];
	VEX_VexOpArg* inFile = &argv[1];
	VEX_VexOpArg* oerror = &argv[2];
	VEX_VexOpArg* output = &argv[3];

	auto statusValue = reinterpret_cast<VEXint*>(status->myArg);
	auto inFileValue = reinterpret_cast<const char*>(inFile->myArg);

	VEX_JSONInstanceStorage* storage = reinterpret_cast<VEX_JSONInstanceStorage*>(data);
	const UT_JSONValue* jsonFile = storage->getJSON(inFileValue);
	if(!jsonFile)
	{
		oerror->myArg = VEX_SetString(*oerror, "File not found");
		*statusValue = VEX_STATUS_FAILURE;
		return;
	}

	const UT_JSONValue* value = VEX_GetJSONValue(*jsonFile, argc, argv);

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
