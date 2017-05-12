#ifndef VEX_JSONVALUE_H
#define VEX_JSONVALUE_H

#include "VEX_utils.h"

namespace VEX_jsonvalue
{
	void* init();
	void evaluate(int, VEX_VexOpArg[], void*);
	void cleanup(void*);
};

#endif // VEX_JSONVALUE_H
