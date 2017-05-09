#ifndef VEX_JSONMAPKEYS_H
#define VEX_JSONMAPKEYS_H

#define PRV_SYM __attribute__ ((visibility ("hidden")))
#define PUB_SYM __attribute__ ((visibility ("default")))

struct VEX_VexOpArg;

namespace VEX_jsonmapkeys
{
	PRV_SYM void* init();
	PRV_SYM void evaluate(int, VEX_VexOpArg[], void*);
	PRV_SYM void cleanup(void*);
};

#endif // VEX_JSONMAPKEYS_H
