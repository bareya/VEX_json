#include "VEX_jsonvalue.h"

#include "VEX/VEX_VexOp.h"
#include "UT/UT_JSONValue.h"
#include "UT/UT_JSONValueArray.h"
#include "UT/UT_JSONValueMap.h"


void VEX_jsonvalue::evaluate(int argc, VEX_VexOpArg argv[], void* data)
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
