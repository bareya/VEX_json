#include "VEX_jsonvalue.h"

#include "VEX/VEX_VexOp.h"
#include "UT/UT_JSONValue.h"
#include "UT/UT_JSONValueArray.h"
#include "UT/UT_JSONValueMap.h"


static int copyJSONValueToVex(const UT_JSONValue* value, VEX_VexOpArg* output)
{
	if(value->getType() == UT_JSONValue::JSON_BOOL && output->myType == VEX_TYPE_INTEGER)
	{
		VEXint* v = reinterpret_cast<VEXint*>(output->myArg);
		*v = static_cast<VEXint>(value->getB());
	}
	else if(value->getType() == UT_JSONValue::JSON_INT && output->myType == VEX_TYPE_INTEGER)
	{
		VEXint* v = reinterpret_cast<VEXint*>(output->myArg);
		*v = static_cast<VEXint>(value->getI());
	}
	else if(value->getType() == UT_JSONValue::JSON_REAL && output->myType == VEX_TYPE_FLOAT)
	{
		VEXfloat* v = reinterpret_cast<VEXfloat*>(output->myArg);
		*v = static_cast<VEXfloat>(value->getF());
	}
	else if(value->getType() == UT_JSONValue::JSON_STRING && output->myType == VEX_TYPE_STRING)
	{
		output->myArg = VEX_SetString(*output, value->getS());
	}
	else
	{
		return VEX_STATUS_FAILURE;
	}

	return VEX_STATUS_SUCCESS;
}


static int copyJSONArrayToVex()
{
	return VEX_STATUS_FAILURE;
}


static int copyJSONMapKeysToVex(const UT_JSONValue* value, VEX_VexOpArg* output)
{
	auto keysArray = reinterpret_cast<UT_Array<const char* >* >(output->myArg);
	for(auto it=keysArray->begin(); !it.atEnd(); ++it)
	{
		VEX_VexOp::stringFree(*it);
	}
	keysArray->clear();

	UT_StringArray mapKeys;
	value->getMap()->getKeys(mapKeys);
	for(auto it=mapKeys.begin(); !it.atEnd(); ++it)
	{
		keysArray->append(VEX_VexOp::stringAlloc((*it).c_str()));
	}

	return VEX_STATUS_SUCCESS;
}


void VEX_jsonvalue::evaluate(int argc, VEX_VexOpArg argv[], void* data)
{
	VEX_VexOpArg* status = &argv[0]; // output status
	VEX_VexOpArg* oerror = &argv[2]; // output error message
	VEX_VexOpArg* output = &argv[3]; // output value
	auto statusValue = reinterpret_cast<VEXint*>(status->myArg);
	const UT_JSONValue* value = VEX_FindJSONValue(argc, argv, data);

	// output string
	if(value && !output->myArray) // output value
	{
		*statusValue = copyJSONValueToVex(value, output);
		if(*statusValue == VEX_STATUS_FAILURE)
		{
			oerror->myArg = VEX_SetString(*oerror, "Output value doesn't match JSON data type or reading value not supported.");
		}
	}
	else if(value && (value->getType() == UT_JSONValue::JSON_ARRAY) && output->myArray) // output array of values
	{
		*statusValue = copyJSONArrayToVex();
		if(*statusValue == VEX_STATUS_FAILURE)
		{
			oerror->myArg = VEX_SetString(*oerror, "Output value doesn't match JSON data type or reading value not supported.");
		}
	}
	else if(value && (value->getType() == UT_JSONValue::JSON_MAP) && output->myArray)
	{
		*statusValue = copyJSONMapKeysToVex(value, output);
		if(*statusValue == VEX_STATUS_FAILURE)
		{

		}
	}
	else // error
	{
		*statusValue = VEX_STATUS_FAILURE;
		// get error message
	}
}
