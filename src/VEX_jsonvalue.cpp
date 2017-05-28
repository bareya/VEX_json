#include "VEX_jsonvalue.h"

#include "VEX/VEX_VexOp.h"
#include "UT/UT_JSONValue.h"
#include "UT/UT_JSONValueArray.h"
#include "UT/UT_JSONValueMap.h"

template<typename T>
bool nothing(const UT_JSONValue& value, VEX_VexOpArg& output)
{
//	T newValue;
//	if(value.import(T))
//	{
//		///output.myArg;
//	}

//	return false;
}

static int copyJSONValueToVex(const UT_JSONValue* value, VEX_VexOpArg* output)
{
	UT_Fpreal64Array f;
	UT_Fpreal32Array k(f);

	JSONDataType dataType = VEX_getJSONDataType(value);

	switch (dataType)
	{
		case JSONDataType::Numeric:
		{
			if(output->myType == VEX_TYPE_INTEGER)
			{
				*reinterpret_cast<VEXint*>(output->myArg) = value->getI();
			}
			else if(output->myType == VEX_TYPE_FLOAT)
			{
				*reinterpret_cast<VEXfloat*>(output->myArg) = value->getF();
			}
			else
			{
				return VEX_STATUS_FAILURE;
			}

			return VEX_STATUS_SUCCESS;
		}
		case JSONDataType::String:
		{
			if(output->myType == VEX_TYPE_STRING)
			{
				output->myArg = VEX_SetString(*output, value->getS());
			}
			else
			{
				return VEX_STATUS_FAILURE;
			}

			return VEX_STATUS_SUCCESS;
		}
		default:
		{
			return VEX_STATUS_FAILURE;
		}
	}
}


static int copyJSONArrayToVex(const UT_JSONValue* value, VEX_VexOpArg* output)
{
	const UT_JSONValueArray* arrayValue = value->getArray();

	UT_Array<const char* >* keysArray = reinterpret_cast<UT_Array<const char* >* >(output->myArg);



	return VEX_STATUS_FAILURE;
}


static int copyJSONMapKeysToVex(const UT_JSONValue* value, VEX_VexOpArg* output)
{
	UT_Array<const char* >* keysArray = reinterpret_cast<UT_Array<const char* >* >(output->myArg);
	for(auto it=keysArray->begin(); !it.atEnd(); ++it)
	{
		VEX_VexOp::stringFree(*it); // TODO Is it safe to call clear without calling free for each element ?
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
	if(!jsonFile)
	{
		*statusValue == VEX_STATUS_FAILURE;
		oerror->myArg = VEX_SetString(*oerror, std::string("File:") + "not found or is not a JSON file.");
		return;
	}

	// search for value, returns nullptr if fails to find the value at given path
	const UT_JSONValue* value = VEX_findJSONValue(jsonFile, argc, argv);

	if(value && !output->myArray) // single value
	{
		*statusValue = copyJSONValueToVex(value, output);
		if(*statusValue == VEX_STATUS_FAILURE)
		{
			oerror->myArg = VEX_SetString(*oerror, "Output value doesn't match JSON data type or reading value not supported.");
		}
	}
	else if(value && (value->getType() == UT_JSONValue::JSON_ARRAY) && output->myArray) // array of values
	{
		*statusValue = copyJSONArrayToVex(value, output);
		if(*statusValue == VEX_STATUS_FAILURE)
		{
			oerror->myArg = VEX_SetString(*oerror, "Output value doesn't match JSON data type or reading value not supported.");
		}
	}
	else if(value && (value->getType() == UT_JSONValue::JSON_MAP) && output->myArray) // keys of map
	{
		*statusValue = copyJSONMapKeysToVex(value, output);
		if(*statusValue == VEX_STATUS_FAILURE){}
	}
	else if(value) // value is found but the type is wrong
	{
		*statusValue = VEX_STATUS_FAILURE;
		oerror->myArg = VEX_SetString(*oerror, std::string("Value of type '") + VEX_jsonTypeAsString(value) + "' can't be stored to output type.");
	}
	else // value is null
	{
		*statusValue = VEX_STATUS_FAILURE;
	}
}
