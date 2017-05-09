#include "VEX_jsonmapkeys.h"

#include "VEX/VEX_VexOp.h"

void* VEX_jsonmapkeys::init()
{
}

void VEX_jsonmapkeys::evaluate(int argc, VEX_VexOpArg argv[], void *initData)
{
	VEXint *result = reinterpret_cast<VEXint*>(argv[0].myArg);

	*result = 100;
}

void VEX_jsonmapkeys::cleanup(void*)
{
}
