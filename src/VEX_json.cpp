#include "UT/UT_DSOVersion.h"

#include "VEX/VEX_VexOp.h"
#include "VEX/VEX_PodTypes.h"

#include "VEX_jsonmapkeys.h"
#include "VEX_jsonvalue.h"
#include "VEX_jsonvaluetype.h"

PUB_SYM void newVEXOp(void*)
{
//	int json(string json, string &errmsg, string &output, ...);
//	0			1			2				3			4
//	returns an array of keys based on provided path to the map
	new VEX_VexOp("jsonmapkeys@&IS&S&[S+",
				VEX_jsonmapkeys::evaluate,
				VEX_ALL_CONTEXT,
				VEX_InitJSONStorage,
				VEX_CleanupJSONStorage,
				VEX_OPTIMIZE_2,
				true
	);

	new VEX_VexOp("jsonvalue@&IS&S&S+",
				VEX_jsonvalue::evaluate,
				VEX_ALL_CONTEXT,
				VEX_InitJSONStorage,
				VEX_CleanupJSONStorage,
				VEX_OPTIMIZE_2,
				true
	);

	new VEX_VexOp("jsonvaluetype@&IS&S&S+",
				VEX_jsonvaluetype::evaluate,
				VEX_ALL_CONTEXT,
				VEX_InitJSONStorage,
				VEX_CleanupJSONStorage,
				VEX_OPTIMIZE_2,
				true
	);
}
