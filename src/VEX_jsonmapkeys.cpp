#include "VEX_jsonmapkeys.h"

#include "VEX/VEX_VexOp.h"
#include "UT/UT_JSONValue.h"
#include "UT/UT_JSONValueMap.h"
#include "UT/UT_JSONValueArray.h"
#include "UT/UT_Array.h"


void VEX_jsonmapkeys::evaluate(int argc, VEX_VexOpArg argv[], void* data)
{
	VEX_VexOpArg* status = &argv[0]; // output status
	VEX_VexOpArg* oerror = &argv[2]; // output error message
	VEX_VexOpArg* output = &argv[3]; // output value
	auto statusValue = reinterpret_cast<VEXint*>(status->myArg);
	const UT_JSONValue* value = VEX_FindJSONValue(argc, argv, data);

	// output string[]
	if(value && value->getType() == UT_JSONValue::JSON_MAP)
	{
		auto keysArray = reinterpret_cast<UT_Array<const char *>*>(output->myArg);
		keysArray->clear();

		UT_StringArray mapKeys;
		value->getMap()->getKeys(mapKeys);
		for(auto it=mapKeys.begin(); !it.atEnd(); ++it)
		{
			keysArray->append(VEX_VexOp::stringAlloc((*it).c_str()));
		}

		*statusValue = VEX_STATUS_SUCCESS;
	}
	else
	{
		oerror->myArg = VEX_SetString(*oerror, "Map hasn't been found.");
		*statusValue = VEX_STATUS_FAILURE;
	}
}
