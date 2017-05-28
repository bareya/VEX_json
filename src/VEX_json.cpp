#include "UT/UT_DSOVersion.h"

#include "VEX/VEX_VexOp.h"
#include "VEX/VEX_PodTypes.h"

#include "VEX_jsonvalue.h"
#include "VEX_jsonvaluetype.h"

void newVEXOp(void*)
{
//	int json(string json, string &errmsg, string &output, ...);
//	0			1			2				3			4
//	returns an array of keys based on provided path to the map
	// jsonvalue int
	new VEX_VexOp("jsonvalue@&IS&S&I+",
				VEX_jsonvalue::evaluate,
				VEX_ALL_CONTEXT,
				VEX_InitJSONStorage,
				VEX_CleanupJSONStorage,
				VEX_OPTIMIZE_2,
				true
	);

	// jsonvalue float
	new VEX_VexOp("jsonvalue@&IS&S&F+",
				VEX_jsonvalue::evaluate,
				VEX_ALL_CONTEXT,
				VEX_InitJSONStorage,
				VEX_CleanupJSONStorage,
				VEX_OPTIMIZE_2,
				true
	);

	// jsonvalue string
	new VEX_VexOp("jsonvalue@&IS&S&S+",
				VEX_jsonvalue::evaluate,
				VEX_ALL_CONTEXT,
				VEX_InitJSONStorage,
				VEX_CleanupJSONStorage,
				VEX_OPTIMIZE_2,
				true
	);

	// jsonvalue string array
	new VEX_VexOp("jsonvalue@&IS&S&[S+",
				VEX_jsonvalue::evaluate,
				VEX_ALL_CONTEXT,
				VEX_InitJSONStorage,
				VEX_CleanupJSONStorage,
				VEX_OPTIMIZE_2,
				true
	);

	// jsonvaluetype string
	new VEX_VexOp("jsonvaluetype@&IS&S&S+",
				VEX_jsonvaluetype::evaluate,
				VEX_ALL_CONTEXT,
				VEX_InitJSONStorage,
				VEX_CleanupJSONStorage,
				VEX_OPTIMIZE_2,
				true
	);
}
