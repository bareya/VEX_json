#ifndef VEX_JSONMAPKEYS_H
#define VEX_JSONMAPKEYS_H

#include "VEX_utils.h"

namespace VEX_jsonmapkeys
{
	void* init();
	void evaluate(int, VEX_VexOpArg[], void*);
	void cleanup(void*);
};

#endif // VEX_JSONMAPKEYS_H
