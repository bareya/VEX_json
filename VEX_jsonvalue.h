#ifndef VEX_JSONVALUE_H
#define VEX_JSONVALUE_H

#include "VEX_utils.h"

namespace VEX_jsonvalue
{
	PRV_SYM void* init();
	PRV_SYM void evaluate(int, VEX_VexOpArg[], void*);
	PRV_SYM void cleanup(void*);
};

#endif // VEX_JSONVALUE_H
