#include "VEX_jsonvalue.h"

#include "VEX/VEX_VexOp.h"
#include "UT/UT_JSONValue.h"
#include "UT/UT_JSONValueArray.h"
#include "UT/UT_JSONValueMap.h"


void VEX_jsonvalue::evaluate(int argc, VEX_VexOpArg argv[], void* data)
{
	VEX_VexOpArg* status = &argv[0]; // output status
	VEX_VexOpArg* oerror = &argv[2]; // output error message
	VEX_VexOpArg* output = &argv[3]; // output value
	auto statusValue = reinterpret_cast<VEXint*>(status->myArg);
	const UT_JSONValue* value = VEX_GetJSONValue(argc, argv, data);

	// output string
	if(value)
	{
		if(value->getType() == UT_JSONValue::JSON_BOOL && output->myType == VEX_TYPE_INTEGER && !output->myArray)
		{
			VEXint* v = reinterpret_cast<VEXint*>(output->myArg);
			*v = static_cast<VEXint>(value->getB());
			*statusValue = VEX_STATUS_SUCCESS;
		}
		else if(value->getType() == UT_JSONValue::JSON_INT && output->myType == VEX_TYPE_INTEGER && !output->myArray)
		{
			VEXint* v = reinterpret_cast<VEXint*>(output->myArg);
			*v = static_cast<VEXint>(value->getI());
			*statusValue = VEX_STATUS_SUCCESS;
		}
		else if(value->getType() == UT_JSONValue::JSON_REAL && output->myType == VEX_TYPE_FLOAT && !output->myArray)
		{
			VEXfloat* v = reinterpret_cast<VEXfloat*>(output->myArg);
			*v = static_cast<VEXfloat>(value->getF());
			*statusValue = VEX_STATUS_SUCCESS;
		}
		else if(value->getType() == UT_JSONValue::JSON_STRING && output->myType == VEX_TYPE_STRING && !output->myArray)
		{
			output->myArg = VEX_SetString(*output, value->getS());
			*statusValue = VEX_STATUS_SUCCESS;
		}
		else
		{
			oerror->myArg = VEX_SetString(*oerror, "Output value doesn't match JSON data type or reading value not supported.");
			*statusValue = VEX_STATUS_FAILURE;
		}
	}
	else // error
	{
		*statusValue = VEX_STATUS_FAILURE;
	}
}
