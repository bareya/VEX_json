#ifndef VEX_JSONMAPKEYS_H
#define VEX_JSONMAPKEYS_H

#include "VEX_utils.h"

namespace VEX_jsonmapkeys
{
	PRV_SYM void* init();
	PRV_SYM void evaluate(int, VEX_VexOpArg[], void*);
	PRV_SYM void cleanup(void*);
};

#endif // VEX_JSONMAPKEYS_H